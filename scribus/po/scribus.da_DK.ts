<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; liste

Returnerer en liste, som indeholder navnene på alle definerede farver i dokumentet.
Hvis ikke et dokument er åbnet, så returneres en liste over standard dokument farver.
</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Viser &quot;Ny dokument&quot; dialog boks. Laver et nyt dokument, hvis brugeren
accepterer indstillingerne. Laver ikke et dokument, hvis brugeren trykker på annullér.
Returnerer &quot;true&quot; hvis der blev lavet et nyt dokument.
</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;navn&quot;]) -&gt; streng

Returnerer navnet på fyld-farven i objektet &quot;navn&quot;.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;navn&quot;])

Flytter objektet &quot;navn&quot; med dx og dy relativt til dets nuværende position. 
Afstanden er udtrykt i dokumentets nuværende måle-enhed (se UNIT 
konstanter). Hvis &quot;navn&quot; ikke angives, så bruges det aktive element.
Hvis &quot;navn&quot; hører til en gruppe, så flyttes hele gruppen.
</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Forhindrer gentegning af siden når bool=&quot;false&quot;, ellers er gentegning aktiveret.
Denne ændring vil holde selv efter scriptet afslutter, så vær sikker på at
kalde setredraw(True) i slutningen: Forbeholdt topniveau af dit script.
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
        <translation>createRect(x, y, bredde, højde, [&quot;navn&quot;]) -&gt; streng

Laver en ny rectangle på den aktive side og returnerer dets navn. 
Koordinaterne er givet i dokumentets nuværende måle-enhed (se 
UNIT konstanter). &quot;navn&quot; skal være et unikt navn, fordi du behøver 
dette navn for at kunne referere til dette objekt senere. Hvis &quot;navn&quot;
ikke angives, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede findes.
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;farve1&quot;, mætning1, &quot;farve2&quot;, mætning2, [&quot;navn&quot;])

Sætter gradient-fyld af objektet &quot;navn&quot; til type. Farvebeskrivelsen er
den samme, som for setFillColor() og setFillShade(). Se konstanterne
for mulige typer (FILL_&lt;type&gt;).
</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;navn&quot;]) -&gt; komma-tal

Returnerer font-størrelsen i punkter for tekst-ramme &quot;navn&quot;. Hvis 
denne tekst-ramme har noget tekst markeret, så returneres værdien 
af det første tegn i den markerede tekst.
Hvis &quot;navn&quot; ikke angives, så bruges det aktive element.
</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;streng&quot;)

Skriver &quot;streng&quot; til Scribus beskedfelt (status linie). Teksten
skal være UTF8 kodet eller &apos;unicode&apos; streng (anbefalet).
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
        <translation>newPage(hvor [,&quot;masterside&quot;])

Laver en ny side. Hvis &quot;hvor&quot; er -1, så tilføjes den nye side i slutningen
af dokumentet, ellers er den ny side indsat før &quot;hvor&quot;. Side-numre er
talt fra 1 og opefter, lige meget hvad nummer den første side har fået.
Den valgfri parameter &quot;masterside&quot; angiver navnet på den master side, 
der skal bruges til den nye side.

Kan rejse IndexError hvis side-nummer er uden for tilladte værdi
</translation>
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
        <translation>newDocument(størrelse, margener, retning, førsteSideNummer,
                        måle-enhed, sideType, førsteSideOrden, antalSider) -&gt; bool

Laver et Nyt dokument og returnerer &quot;true&quot; hvis det lykkedes. Parametrene har 
følgende betydning:

størrelse = A tuple (bredde, højde) beskriver størrelsen af dokumentet. du kan
bruge predefinerede constanter med navnet PAPER_&lt;papir_type&gt; f.eks. PAPER_A4 etc.

margener = En tuple (venstre, højre, top, bund) beskriver documentets margener

retning = sidens retning - constanter PORTRAIT, LANDSCAPE

førsteSideNummer = er nummeret på den første side i dokumentet brugt for
side-nummerering. Selvom værdien oftest er 1, så er der nogen gange brug for 
et højere nummer, hvis et dokument består af flere dele.

måle-enhed: denne værdi sætter måle-enheden, som bruges i dokumentet. Brug en
predefineret constant for dette, en af: UNIT_INCHES, UNIT_MILLIMETERS,
UNIT_PICAS, UNIT_POINTS.

sideType = En af de predefinerede constanter PAGE_n. PAGE_1 er enkeltside,
PAGE_2 er for dobbeltsidet dokumenter, PAGE_3 er for 3 siders folder og
PAGE_4 er 4-fold

førsteSideOrden = Hvad er positionen af første side i dokumentet.
Indeksering fra 0 (0 = første).

antalSider = Antal sider som skal laves.

Værdierne for bredde, højde og margener er udtrykt i den givne måle-enhed
for dokumentet. PAPER_* constanter er udtrykt i punkter. Hvis dit dokument
ikke er i punkter, så sørg for at tage højde for dette.

eksempel: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3, 1)

Kan rejse ScribusError hvis førsteSideOrden er større end tilladt for sideType.
</translation>
    </message>
    <message>
        <source>placeSVG(&quot;filename&quot;, x, y)

Places the SVG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SVG placed on the page

If loading was successful, the selection contains the imported SVG
</source>
        <translation>placeSVG(&quot;filnavn&quot;, x, y)

Placerer SVG &quot;filnavn&quot; på den aktive side,
x og y specificerer coordinaten af øverste venstre hjørne af SVG&apos;en på siden

hvis indlæsning lykkedes, indeholder markeringen den importerede SVG
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
        <translation>getColor(&quot;navn&quot;) -&gt; tuple

Returnerer en tuple  (C, M, Y, K), som indeholder de fire farvekomponenter af
farven &quot;navn&quot; fra det aktive dokument. Hvis ingen dokumenter er åben, så
returneres værdien af den navngivne farve fra standard dokument farver.

Kan rejse NotFoundError hvis den navngivne farve ikke findes.
Kan rejse ValueError hvis et ugyldigt farve-navn er angivet.
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
        <translation>changeColor(&quot;navn&quot;, c, m, y, k)

Ændrer farven &quot;navn &quot; til den angivne C, M, Y, K værdi. Farveværdien er
defineret via fire komponenter c = Cyan, m = Magenta, y = Gul og k = sort.
Farve-komponenterne skal være i området fra 0 til 255.

Kan rejse NotFoundError hvis den navngivne farve ikke findes.
Kan rejse ValueError hvis et ugyldigt farve-navn er angivet.
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
        <translation>deleteColor(&quot;navn&quot;, &quot;erstat&quot;)

Sletter farven &quot;navn &quot;. Enhver forekomst af denne farve er erstattet 
af farven &quot;erstat&quot;. Hvis &quot;erstat&quot; ikke er angivet, så sættes farven til 
&quot;NONE&quot;  - transparent.

Kan rejse NotFoundError hvis den navngivne farve ikke findes.
Kan rejse ValueError hvis et ugyldigt farve-navn er angivet.
</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;navn&quot;, &quot;erstat&quot;)

Enhver forekomst af farven &quot;navn&quot; bliver erstattet af farven &quot;erstat&quot;.

Kan rejse NotFoundError hvis den navngivne farve ikke findes.
Kan rejse ValueError hvis et ugyldigt farve-navn er angivet.
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
        <translation>messageBox(&quot;titel&quot;, &quot;besked&quot;,
    ikon=ICON_NONE, knap1=BUTTON_OK|BUTTONOPT_DEFAULT,
    knap2=BUTTON_NONE, knap3=BUTTON_NONE) -&gt; integer

Viser en besked-boks med titlen &quot;titel&quot;, meddelelsen &quot;besked&quot;, og
et ikon &quot;ikon&quot; og op til 3 knapper. Som standard bruges ingen ikoner og 
en enkel knap, OK, er vist. Kun titel og besked argumenterne er påkrævet,
selvom det  anbefales meget, at sætte et ikon og passende knap(per). 
Besked-teksten kan indeholde simple HTML-lignende formatering.

Returnerer  nummeret på den knap brugeren trykkede på. Knap-numre 
starter ved 1.

For ikon og knap-parametre er der predefinerede konstanter tilgængelige
med de samme navne som i Qt Dokumentationen. Disse er BUTTON_* og
ICON_* konstanterne defineret i modulet. Der er også to ekstra konstanter der
kan binær-ORes med knap konstanter:
    BUTTONOPT_DEFAULT   Tryk enter trykker denne knap.
    BUTTONOPT_ESCAPE    Tryk escape trykker denne knap.

Eksempler på brug:
resultat = messageBox(&apos;Script fejlede&apos;,
                    &apos;Denne script fungerer kun, når der er markeret en tekst-ramme.&apos;,
                    ICON_ERROR)
resultat = messageBox(&apos;Abekatte!&apos;, &apos;Noget gik agurk! &lt;i&gt;Hvad er en abekat?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Definerede knap og ikon konstanter:
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
        <translation>valueDialog(titel, besked [,standardværdi]) -&gt; streng

Viser den almindelige &apos;spørg efter streng&apos; dialog og returnerer dens værdi som en streng
Parametre: vindue-titel, tekst i vinduet og en valgfri &apos;standard&apos; værdi.

Eksempel: valueDialog(&apos;titel&apos;, &apos;tekst i vinduet&apos;, &apos;valgfri&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Lukker det aktive dokument uden at spørge om det skal gemmes.

Kan rejse NoDocOpenError hvis der ikke er noget dokument at lukke
</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Returnerer &quot;true&quot; hvis der er et åbent dokument.
</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;navn&quot;)

Åbner dokumentet &quot;navn&quot;

Kan rejse ScribusError hvis dokumentet ikke kunne åbnes.
</translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation>saveDoc(&quot;navn&quot;)

Gemmer det aktive dokument med dets nuværende navn, returnerer &quot;true&quot; hvis 
det lykkedes. Hvis dokumentet ikke allerede har været gemt, så vil der komme 
en interaktiv &quot;gem fil&quot; dialog.

Hvis &quot;gem fil&quot; fejler, er der ikke nogen måde at fortælle det på.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;navn&quot;)

Gemmer det aktive dokument med det nye navn &quot;navn&quot; (som kan være en 
fuld eller relativ sti).

Kan rejse ScribusError hvis gemningen fejler.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Sætter dokumentets margener, Venstre(lr), Højre(rr), Top(tr) og Bund(br)
margener er angivet i dokumentets måle-enhed - se UNIT_&lt;type&gt; konstanter.
 </translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type)

Ændrer dokumentets måle-enhed. Mulige værdier for &quot;enhed&quot; er
defineret som konstanter UNIT_&lt;type&gt;.

Kan rejse ValueError hvis en ugyldig type er angivet.
</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (Scribus enheds  konstant)

Returnerer dokumentets måle-enhed. Den returnerede værdi vil være en
af UNIT_* konstanterne:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filnavn&quot;)

Indlæser afsnits-typografierne fra Scribus dokumentet &quot;filnavn&quot; 
ind i det aktive dokument.
</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(modståendeSider, førsteSideVenstre)

Sætter dokumentets type. For at få modstående sider, så sæt den første værdi 
til FACINGPAGES, for at deaktivere dette brug istedet NOFACINGPAGES.  Hvis du ønsker
at have den første side som venstre side, sæt da den anden værdi til FIRSTPAGELEFT, for
en højre side som den første brug FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;navn&quot;]) -&gt; streng

Returnerer navnet på linie-farven på objektet &quot;navn&quot;.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;navn&quot;]) -&gt; integer

Returnerer linie-bredde på på objektet &quot;navn&quot;. Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;navn&quot;]) -&gt; integer

Returnerer værdien for liniens farvemætning  på objektet &quot;navn&quot;. 
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;navn&quot;]) -&gt; integer (se kontanter)

Returnerer typen af liniesamlingen på objektet &quot;navn&quot;. Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element. Samlingstyperne er
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;navn&quot;]) -&gt; integer (se kontanter)

Returnerer typen af linieenden på objektet &quot;navn&quot; Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element. Endetyperne er
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;navn&quot;]) -&gt; integer (se kontanter)

Returnerer liniestilen på objektet &quot;navn&quot; Hvis &quot;navn&quot; ikke 
angives, så bruges det markerede element. Liniestilene er
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;navn&quot;]) -&gt; integer

Returnerer farvemætningen på fyld-farven på objektet &quot;navn&quot;.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;navn&quot;]) -&gt; (x,y) 

Returnerer en (x, y) tuple, som indeholder skaleringsværdierne for billedrammen
&quot;navn&quot;.  Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;navn&quot;]) -&gt; streng

Returnerer filnavnet for billedet i billed-rammen &quot;navn&quot;. Hvis 
&quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;navn&quot;]) -&gt; (bredde,højde) 

Returnerer en (bredde, højde) tuple med størrelsen på objektet &quot;navn&quot;.  
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. Størrelsen 
er udtrykt i den aktuelle måle-enhed for dokumentet - se  UNIT_&lt;type&gt; 
for reference.
</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;navn&quot;]) -&gt; integer

Returnerer rotationen på objektet &quot;navn&quot;. Værdien er udtrykt i grader,
og med uret giver en positiv værdi. Hvis &quot;navn&quot; ikke angives, så bruges 
det markerede element.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; liste

Returnerer en liste, som indeholder navnene på alle objekter på den aktive side.
</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [,&quot;navn&quot;]) 

Flytter objektet &quot;navn&quot; til en ny placering. Koordinaterne er udtrykt i den 
aktuelle måle-enhed for dokumentet - (se  UNIT konstanter).  Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.  Hvis objektet &quot;navn&quot;
hører til en gruppe, så flyttes hele gruppen.
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
        <translation>rotateObject(rot [, &quot;navn&quot;])

Roterer objektet &quot;navn&quot; med &quot;rot&quot; grader relativt. Objektet er roteret 
omkring den spids, som er valgt som omdrejningspunkt - standard er 
det øverste venstre ved nul rotation. Positive værdier angiver drejning 
mod uret, når standard omdrejnings-punktet bruges. Hvis &quot;navn&quot; ikke 
angives, så bruges det markerede element. 
 </translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(bredde, højde [, &quot;navn&quot;])

Ændrer størrelsen på objektet &quot;navn&quot; til den givne bredde og højde.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. 
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; streng

Returnerer navnet på det markerede objekt. Hvis &quot;nr&quot; angives, så indikerer 
det nummeret på det markerede objekt, f.eks. betyder 0 det første markerede 
objekt, 1 betyder det andet markerede objekt o.s.v. 
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Returneret antallet af markerede objekter.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;navn&quot;)

Marker objekt med det givne &quot;navn&quot;.
</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Fjerner alle markeringer i hele dokumentet.
</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(liste)

Laver en gruppe af alle objekter der er nævnt i &quot;liste&quot;. &quot;liste&quot; skal indeholde 
navnene på de objekter, som skal laves til en gruppe. Hvis &quot;liste&quot; ikke angives, 
så bruges de markerede elementer.
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;navn&quot;)

Adskiller gruppen, som objektet &quot;navn&quot; hører til. Hvis &quot;navn&quot; ikke angives, 
så bruges det markerede element.
</translation>
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
        <translation>scaleGroup(faktor [,&quot;navn&quot;])

Skalerer gruppen, som objekter &quot;navn&quot; hører til. Værdier større en 1 forstørrer
gruppen, værdier mindre end 1 laver gruppen mindre, f.eks. en værdi på 0.5
skalerer gruppen til 50 % af dens originale størrelse, en værdi på 1.5 skalerer
gruppen til 150 % af dens originale størrelse.  Værdien for &quot;faktor&quot; skal være
større end 0. Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.

kan rejse ValueError, hvis en ugyldig skalerings-faktor er angivet.
</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;filnavn&quot;, &quot;navn&quot;)

Indlæser billedet &quot;filnavn&quot; ind i billed-rammen &quot;navn&quot;.  Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.

Kan rejse WrongFrameTypeError, hvis målrammen ikke er en billed-ramme
</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;navn&quot;])

Sætter Skalerings-faktorerne for billedet i billed-rammen &quot;navn&quot;. 
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. 
Værdien 1 betyder 100 %.

kan rejse WrongFrameTypeError, hvis mål-rammen ikke er en billedramme
</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;navn&quot;]) -&gt; bool

Låser objektet &quot;navn&quot; hvis det er ulåst og låser det op hvis det er låst.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. 
Returnerer &quot;true&quot; hvis objektet er låst.
</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;navn&quot;]) -&gt; bool

Returnerer &quot;true&quot; hvis objektet &quot;navn&quot; er låst. Hvis &quot;navn&quot; ikke angives, 
så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; liste

Returnerer en liste med navnene på alle tilgængelige fonte.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; liste af tupler

Returner større mængde font-info. Det er en liste af tuplerne med:
[ (Scribus navn, Familie, Rigtige navn, substituere (1|0), indlejr PS (1|0), font-fil), (...), ... ]
</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; liste

Returnerer en liste med navnene på alle definerede lag.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;navn&quot;)

Sætter laget &quot;navn&quot; som det aktive lag.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; streng

Returnerer navnet på det aktive lag.
</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;lag&quot; [, &quot;navn&quot;])

Sender objektet &quot;navn&quot; til laget &quot;lag&quot;. Laget skal eksistere.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;lag&quot;, synlig)

Sætter laget &quot;lag&quot; til at være synligt eller usynligt. Hvis &quot;synlig&quot; sættes til 
&quot;False&quot;, så er laget usynligt.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;lag&quot;)

Sletter laget med navnet &quot;lag&quot;. Der sker ingen ting hvis laget ikke
eksisterer eller hvis det er det eneste lag i dokumentet.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(&quot;lag&quot;)

Laver et nyt lag med navnet &quot;lag&quot;.

Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; streng

Returnerer en streng med -lang værdien.
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
        <translation>createEllipse(x, y, bredde, højde, [&quot;navn&quot;]) -&gt; streng

Laver en ny ellipse på den aktive side og returnerer dets navn. 
Koordinaterne er givet i dokumentets nuværende måle-enhed (se 
UNIT konstanter). &quot;navn&quot; skal være et unikt navn, fordi du behøver 
dette navn for at kunne referere til dette objekt senere. Hvis &quot;navn&quot;
ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
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
        <translation>createImage(x, y, bredde, højde, [&quot;navn&quot;]) -&gt; streng

Laver en ny billed-ramme på den aktive side og returnerer dets navn. 
Koordinaterne er givet i dokumentets nuværende måle-enhed (se 
UNIT konstanter). &quot;navn&quot; skal være et unikt navn, fordi du behøver 
dette navn for at kunne referere til dette objekt senere. Hvis &quot;navn&quot;
ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
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
        <translation>createText(x, y, bredde, højde, [&quot;navn&quot;]) -&gt; streng

Laver en ny tekst-ramme på den aktive side og returnerer dets navn. 
Koordinaterne er givet i dokumentets nuværende måle-enhed (se 
UNIT konstanter). &quot;navn&quot; skal være et unikt navn, fordi du behøver 
dette navn for at kunne referere til dette objekt senere. Hvis &quot;navn&quot;
ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
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
        <translation>createLine(x1, y1, x2, y2, [&quot;navn&quot;]) -&gt; streng

Laver en ny linie fra punktet(x1, y1) til punktet(x2, y2) og returnerer 
dens navn. Koordinaterne er givet i dokumentets nuværende måle-enhed 
(se UNIT konstanter). &quot;navn&quot; skal være et unikt navn for objektet, fordi 
du behøver dette navn for yderligere adgang til dette objekt. Hvis &quot;navn&quot;
ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
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
        <translation>createPolyLine(liste, [&quot;navn&quot;]) -&gt; streng

Laver en ny poly-linie og returnerer dens navn. Poly-liniens punkter 
er placeret i listen &quot;liste&quot; i følgende orden: [x1, y1, x2, y2...xn, yn].
Koordinaterne er givet i dokumentets nuværende måle-enhed 
(se UNIT konstanter). &quot;navn&quot; skal være et unikt navn for objektet, fordi 
du behøver dette navn for yderligere adgang til dette objekt. Hvis &quot;navn&quot;
ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
Kan rejse ValueError hvis et utilstrækkeligt antal punkter er angivet eller hvis
antallet af værdier ikke kan danne talpar uden noget tilovers.
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
        <translation>createPolygon(liste, [&quot;navn&quot;]) -&gt; streng

Laver et nyt polygon og returnerer dets navn. Polygonets punkter 
er placeret i listen &quot;liste&quot; i følgende orden: [x1, y1, x2, y2...xn, yn].
Mindst tre punkter er påkrævet. Det er ikke nødvendigt at gentage det 
første punkt for at lukke polygonet. Polygonet er automatisk lukket ved at 
forbinde det første og det sidste punkt.  Koordinaterne er givet i dokumentets 
nuværende måle-enhed (se UNIT konstanter).  &quot;navn&quot; skal være et unikt navn 
for objektet, fordi du behøver dette navn for yderligere adgang til dette objekt. 
Hvis &quot;navn&quot; ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
Kan rejse ValueError hvis et utilstrækkeligt antal punkter er angivet eller hvis
antallet af værdier ikke kan danne talpar uden noget tilovers.
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
        <translation>createBezierLine(liste, [&quot;navn&quot;]) -&gt; streng

Laver en ny bezier kurve og returnerer dens navn. Bezier kurvens 
punkter er placeret i listen &quot;liste&quot; i følgende orden: 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn, yn, kxn, kyn].
I listen med punkter betyder x og y koordinaterne for punktet og kx og ky er 
kontrolpunktet for kurven.  Koordinaterne er givet i dokumentets nuværende 
måle-enhed (se UNIT konstanter).  &quot;navn&quot; skal være et unikt navn for objektet, 
fordi du behøver dette navn for yderligere adgang til dette objekt. Hvis &quot;navn&quot; 
ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
Kan rejse ValueError hvis et utilstrækkeligt antal punkter er angivet eller 
hvis der er nogen tal tilovers.
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
        <translation>createPathText(x, y, &quot;tekstboks&quot;, &quot;bezierkurve&quot;, [&quot;navn&quot;]) -&gt; streng

Laver en ny sti-tekst ved at forene de to objekter &quot;tekstboks&quot; og
&quot;bezierkurve&quot; og returnerer dens navn. Koordinaterne er givet i dokumentets 
nuværende måle-enhed (se UNIT konstanter). &quot;navn&quot; skal være et unikt navn 
for objektet, fordi du behøver dette navn for yderligere adgang til dette objekt. 
Hvis &quot;navn&quot; ikke er givet, så vil Scribus lave et for dig.

Kan rejse NameExistsError hvis du angiver et navn, som allerede er brugt.
Kan rejse  NotFoundError hvis en eller begge navngive basis-objekter ikke eksisterer.
</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;navn&quot;])

Sletter elementet med navnet &quot;navn&quot;. Hvis &quot;navn&quot; ikke angives, 
så slettes det markerede element.
</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;navn&quot;]) -&gt; bool

Undersøger om et objekt med navnet &quot;navn&quot; virkelig eksisterer i dokumentet.
Den valgfri paramater er objekt navnet. Hvis &quot;navn&quot; ikke angives, så returneres
&quot;True&quot; hvis et eller andet er markeret.
</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;stil&quot; [, &quot;navn&quot;])

Anvender stil på objektet &quot;navn&quot;. Hvis &quot;navn&quot; ikke angives, så 
anvendes stilen på det markerede objekt.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; liste

Returnerer en liste af navne på alle afsnits-typografier i det aktive dokument.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Returnerer nummeret på den aktive side. Sidenummeret er talt fra 1 og
opefter, uanset hvilket sidenummer den første side har fået.
</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Gentegner alle sider.
</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;navn&quot;)

Gemmer den aktive side som en EPS til filen &quot;navn&quot;

Kan rejse ScribusError hvis &quot;gem&quot; fejlede.
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

Sletter den angivne side. Ingen ting slettes, hvis dokumentet kun indeholder en side.
Sidenummeret er talt fra 1 og opefter, uanset hvilket sidenummer den første side har fået.

Kan rejse IndexError hvis sidenummeret er uden for område
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

Flytter til siden &quot;nr&quot; (d.v.s. laver siden &quot;nr&quot; til den aktive). bemærk at
gotoPage ændrer ikke (lige nu) den side som brugeren ser, den vælger
kun den side, som script-kommandoer opererer på.

Kan rejse  IndexError hvis sidenummeret er uden for område.
</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; integer

Returnerer antal sider i dokumentet.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; liste

Returnerer en liste med positionerne for de horisontale hjælpelinier. Værdierne er 
angivet i den aktuelle måle-enhed for dokumentet - se  UNIT_&lt;type&gt; konstanter.
</translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(liste)

Sætter horisontale hjælpelinier. Input-parameter skal være en liste med hlælpeliniernes
placeringer angivet i den aktuelle måle-enhed for dokumentet - se  UNIT_&lt;type&gt; konstanter.

Eksempel: setHGuides(getHGuides() + [200.0, 210.0] # tilføjer ny hjælpelinie uden nogen mistet
          setHGuides([90,250]) # Erstatter nuværende hjælpelinier fuldstændigt
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Se getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Se setHGuides.
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Returnerer en tuple med side-dimensioner målt i dokumentets aktuelle måle-enhed.
Se  UNIT_&lt;type&gt; konstanter og getPageMargins()
</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; liste

Returnerer en liste af tupler med elementer på den aktive side. Tuplen er:
(navn, objekt-type, orden) F.eks. [(&apos;Tekst1&apos;, 4, 0), (&apos;Billed1&apos;, 2, 1)].
Det betyder objektet &apos;Tekst1&apos; er en tekst-ramme (type 4) og er den 
første på siden...
</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;farve&quot;, [&quot;navn&quot;])

Sætter fyld-farven på objektet &quot;navn&quot; til farven &quot;farve&quot;. &quot;farve&quot;
er navnet på en af de definerede farver. Hvis &quot;navn&quot; ikke angives, så 
bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;farve&quot;, [&quot;navn&quot;])

Sætter linie-farven på objektet &quot;navn&quot; til farven &quot;farve&quot;. &quot;farve&quot;
er navnet på en af de definerede farver. Hvis &quot;navn&quot; ikke angives, så 
bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(bredde, [&quot;navn&quot;])

Sætter linie-bredden på objektet &quot;navn&quot; til &quot;bredde&quot;. &quot;bredde&quot; skal være i 
området fra 0.0 til 12.0 inklusivt, og er angivet i punkter. Hvis &quot;navn&quot; ikke 
angives, så bruges det markerede element.
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
        <translation>setLineShade(farvemætning, [&quot;navn&quot;]) -&gt; integer

Sætter farvemætningen på linie-farven på objektet &quot;navn&quot; til &quot;farvemætning&quot;.
&quot;farvemætning&quot; skal være en integer værdi i området fra 0 (lysest) til 100
(fuld farvemætning). Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.

Kan rejse ValueError hvis farvemætningen er uden for tilladte område.
</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(samling, [&quot;navn&quot;])

Sætter typen af liniesamling på objektet &quot;navn&quot; til typen &quot;samling&quot;. 
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. Der er
predefinerede konstanter for samlingen - JOIN_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endetype, [&quot;navn&quot;])

Sætter typen af linieende på objektet &quot;navn&quot; til typen &quot;endetype&quot; .
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. Der er
predefinerede konstanter for endetyperne - CAP_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(stil, [&quot;navn&quot;])

Sætter linie-stil for objektet &quot;navn&quot; til stilen &quot;stil&quot;. Hvis &quot;navn&quot; ikke 
angives, så bruges det markerede element. Der er predefinerede 
konstanter for &quot;stil&quot; - LINE_&lt;type&gt;.
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
        <translation>setFillShade(farvemætning, [&quot;navn&quot;])

Sætter farvemætningen på fyld-farven på objektet &quot;navn&quot; til &quot;farvemætning&quot;.
&quot;farvemætning&quot; skal være en integer værdi i området fra 0 (lysest) til 100
(fuld farvemætning). Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.

Kan rejse ValueError hvis farvemætningen er uden for tilladte område.
</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(radius, [&quot;navn&quot;])

Sætter radius på hjørnerne på objektet &quot;navn&quot;. Radius  er udtrykt 
i punkter. Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.

Kan rejse ValueError hvis hjørne-radius er negativ.
</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;navngivetStil&quot;, [&quot;navn&quot;])

sætter linie-stil på objektet &quot;navn&quot; til navngivet stil &quot;navngivetStil&quot;.
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.

Kan rejse NotFoundError hvis linie-stilen ikke eksisterer.
</translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;navn&quot;]) -&gt; streng

Returnerer font-navnet for tekst-rammen &quot;navn&quot;. Hvis denne tekst-ramme
har noget tekst markeret, så returneres værdien af det første tegn i den 
markerede tekst. Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;navn&quot;]) -&gt; integer

Returnerer længden på teksten i tekst-rammen &quot;navn&quot;.
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;navn&quot;]) -&gt; streng

Returnerer teksten fra tekst-rammen &quot;navn&quot;. Hvis denne tekst-ramme har 
noget tekst markeret, så returneres den markerede tekst. Al tekst i rammen, 
ikke kun det der lige nu ses, returneres. Hvis &quot;navn&quot; ikke er givet, så bruges 
det markerede element.
</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>getAllText([&quot;navn&quot;]) -&gt; streng

Returnerer teksten fra tekst-rammen &quot;navn&quot; og alle tekst-rammer, som er lænket 
med denne ramme. Hvis denne tekst-ramme har noget tekst markeret, så returneres 
den markerede tekst. Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;navn&quot;]) -&gt; komma-tal

Returnerer linie-afstanden (&quot;leading&quot;) i tekst-rammen &quot;navn&quot; udtrykt i punkter.
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;navn&quot;]) -&gt; komma-tal

Returnerer kolonne-afstanden i tekst-rammen &quot;navn&quot; udtrykt i punkter.
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;navn&quot;]) -&gt; integer

Henter antallet af kolonner i tekst-rammen &quot;navn&quot;. Hvis &quot;navn&quot; 
ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;tekst&quot;, [&quot;navn&quot;])

Sætter teksten i tekst-rammen &quot;navn&quot;  til teksten fra strengen &quot;tekst&quot;.
Teksten skal være UTF8 kodet - brug f.eks. unicode(tekst, &apos;iso-8859-2&apos;). se FAQ&apos;en
for flere detaljer. Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation>setFont(&quot;font&quot;, [&quot;navn&quot;])

Sætter fonten i tekst-rammen &quot;navn&quot; til &quot;font&quot;. Hvis der er markeret nogen 
tekst, vil kun den markerede tekst blive ændret.  Hvis &quot;navn&quot; ikke er givet, 
så bruges det markerede element.

Kan rejse ValueError hvis fonten ikke findes.
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
        <translation>setFontSize(størrelse, [&quot;navn&quot;])

Sætter font-størrelsen i tekst-rammen &quot;navn&quot; til &quot;størrelse&quot;. &quot;størrelse&quot;
angives i punkter. Hvis der er markeret nogen tekst, vil kun den markerede 
tekst blive ændret. &quot;størrelse&quot; skal være indenfor området 1 til 512. Hvis &quot;navn&quot; 
ikke er givet, så bruges det markerede element.

Kan rejse ValueError for en fontstørrelse der er uden for gyldigt område.
</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(størrelse, [&quot;navn&quot;])

Sætter linie-afstanden (&quot;leading&quot;) i tekst-rammen &quot;navn&quot; til &quot;størrelse&quot;. 
&quot;størrelse&quot; er angivet i punkter. Hvis &quot;navn&quot; ikke er givet, så bruges 
det markerede element.

Kan rejse ValueError hvis linie-afstanden er uden for gyldigt område.
</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(størrelse, [&quot;navn&quot;])

Sætter kolonne-afstanden i tekst-rammen &quot;navn&quot; til værdien &quot;størrelse&quot;. 
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse ValueError hvis kolonne-afstanden er uden for gyldigt område (skal være positiv).
</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(nr, [&quot;navn&quot;])

Sætter antal kolonner i tekst-rammen &quot;navn&quot; til heltallet &quot;nr&quot;. 
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse ValueError hvis antal kolonner ikke er mindst 1.
</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(justering, [&quot;navn&quot;])

Sætter tekst-justering i tekst-rammen &quot;navn&quot; til angivet justering.
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element. &quot;justering&quot;
skal være en af ALIGN_konstanterne defineret i dette modul - se dir(scribus).

Kan rejse ValueError for en ugyldig ALIGN_konstant.
</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;navn&quot;])

Sletter al tekst i tekst-rammen &quot;navn&quot;. Hvis der er markeret noget tekst, så
slettes kun den markerede tekst. Hvis &quot;navn&quot; ikke er givet, så bruges 
det markerede element.
</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;farve&quot;, [&quot;navn&quot;])

Sætter tekst-farven i tekst-rammen &quot;navn&quot; til farven &quot;farve&quot;. Hvis der 
er markeret nogen tekst, så vil kun den markerede tekst blive ændret. 
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;farve&quot;, [&quot;navn&quot;])

Sætter tekstens streg-farve. Hvis &quot;navn&quot; ikke er givet, 
så bruges det markerede element.
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
        <translation>setTextShade(farvemætning, [&quot;navn&quot;])

Sætter farvemætningen på tekst-farven på objektet &quot;navn&quot; til &quot;farvemætning&quot;.
Hvis der er markeret nogen tekst, så vil kun den markerede tekst blive ændret. 
&quot;farvemætning&quot; skal være en integer værdi i området fra 0 (lysest) til 100
(fuld farvemætning). Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;franavn&quot;, &quot;tilnavn&quot;)

Sammenkæder to tekst-rammer. Rammen &quot;franavn&quot; bliver sammenkædet
med rammen &quot;tilnavn&quot;. Mål-rammen skal være en tom tekst-ramme og må
ikke være sammenkædet med nogen anden ramme.
</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;navn&quot;)

Fjerner objektet &quot;navn&quot; fra  sammenkædede tekstrammer. Hvis rammen
var i midten af en kæde, så vil den foregående og den efterfølgende ramme
blive forbundet, f.eks. &apos;a-&gt;b-&gt;c&apos; bliver &apos;a-&gt;c&apos; når du kalder unlinkTextFrames(b).

Kan rejse ScribusException hvis sammenkædnings-regler bliver overtrådt.
</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;navn&quot;])

Konverterer fonte i tekst-rammen &quot;navn&quot; til kurver. Hvis &quot;navn&quot; ikke angives, 
så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Rydder op i Scrbus fremgangs-målererens tidligere indstillinger. Den kaldes før næste 
brug af fremgangs-måleren. Se progressSet.
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Sætter fremgangs-målerens maksimale trin-værdi til det angivne tal.
Se progressSet.
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

Sætter fremgangs-måler positionen til &quot;nr&quot;, en værdi relativ til det tidligere kald af 
progressTotal. Fremgangs-måleren bruger konceptet om trin; du giver den det totale 
antal trin og antal trin nået indtil nu, og den vil så vise den procentvise værdi af de trin 
der er gjort færdige. Du kan angive det totale antal trin med progressTotal(). De 
nuværende antal trin er angivet med progressSet(). Fremgangs-måleren kan spoles 
tilbage til begyndelsen med progressReset(). [Baseret på info fra Trolltech&apos;s Qt docs]
</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[IKKE UNDERSTØTTET!] Denne kan få tingene til at bryde sammen, så
undgå at bruge den lige nu.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Aktiverer/deaktiverer &quot;gem-ikonet&quot; i Scribus værktøjslinie og  &quot;Gem&quot; i fil-menuen. 
Det er brugbart at kalde denne procedure, når du ændrer dokumentet, fordi Scribus 
ikke automatisk opdager, når du har ændret dokumentet ved at bruge en script.
</translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation>setScaleImageToFrame(skalertilramme, proportional=None, name=&lt;markering&gt;)

Sætter skalér til ramme på den markerede eller speciferede billed-ramme til `scaletoframe&apos;.
Hvis proportional er specificeret, så sættes fast bredde/højde forholdet til `proportionelt&apos;.
Både `skalertilramme&apos; og `proportional&apos; er boolsk.

Kan rejse  WrongFrameTypeError.
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
        <translation>renderFont(&quot;navn&quot;, &quot;filenavn&quot;, &quot;eksempel&quot;, størrelse, format=&quot;PPM&quot;) -&gt; bool

Laver en billed-visning af fonten &quot;navn&quot; med teksten &quot;eksempel&quot; og størrelse.
Hvis &quot;filnavn&quot; ikke er &quot;&quot;, så bliver billedet gemt i &quot;filnavn&quot;. Ellers returneres 
billed-data som en streng. Det valgfri &quot;format&quot; argument specificerer billed-formatet, 
der skal genereres, og støtter de formater, som er tilladt af QPixmap.save().
Almindelige formater er PPM, JPEG, PNG og XPM.

Kan rejse NotFoundError hvis den angivne font ikke findes.
Kan rejse  ValueError hvis &quot;eksempel&quot; eller &quot;filnavn&quot; er tomt.
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
        <translation>isLayerPrintable(&quot;lag&quot;) -&gt; bool

Returnerer hvorvidt laget &quot;lag&quot; kan udskrives eller ej, værdien &quot;True&quot; 
betyder, at laget &quot;lag&quot; kan udskrives, værdien &quot;False&quot; betyder, at laget 
&quot;lag&quot; ikke kan udskrives.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>setPDFBookmark(&quot;til/fra&quot;, [&quot;navn&quot;]

Sætter hvorvidt (til/fra = 1) tekstrammen &quot;navn&quot; et et bogmærke eller ej.
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse WrongFrameTypeError, hvis målrammen ikke er en tekst-ramme
</translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>isPDFBookmark([&quot;navn&quot;]) -&gt; bool

Returnerer &quot;true&quot; hvis tekst-rammen &quot;navn&quot; er et PDF bogmærke.
Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse WrongFrameTypeError, hvis målrammen ikke er en tekst-ramme
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Returnerer side margener som en (top, venstre, højre, bund) tuple i dokumentets 
aktuelle måle-enhed. Se  UNIT_&lt;type&gt; konstanter og getPageSize().
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
        <translation>getColorAsRGB(&quot;navn&quot;) -&gt; tuple

Returnerer en tuple  (R, G, B), som indeholder de tre farvekomponenter 
af farven &quot;navn&quot; fra det aktive dokument. Konverteret til RGB farve rum.
Hvis ingen dokumenter er åben, så returneres værdien af den navngivne 
farve fra standard dokument farver.

Kan rejse NotFoundError hvis den navngivne farve ikke findes.
Kan rejse ValueError hvis et ugyldigt farve-navn er angivet.
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
        <translation>fileDialog(&quot;overskrift&quot;, [&quot;filter&quot;, &quot;standardnavn&quot; ,har-previsning, er-gem, er-mappe]) -&gt; streng med fil-navn

Viser en &quot;Fil-åbn&quot; dialog-boks med overskriften &quot;overskrift&quot;. Filer er filtreret
med filter-strengen &quot;filter&quot;. Et standard fil-navn eller fil-sti kan også
angives, lad denne streng være tom, når du ikke ønsker at bruge den.
Hvis har-previsning er &quot;TRUE&quot; aktiverer et lille visningsvindue i fil-valg-boksen.  
Når er-gem værdien er sat til &quot;True&quot;, så fungerer dialogboksen som en &quot;Gem som&quot; 
dialog, ellers fungerer den som en &quot;Fil åbn&quot; dialog. Når er-mappe værdien er 
&quot;True&quot;, så viser og returnerer dialogboksen kun mapper. Standardværdien for 
alle de valgfri parametre er &quot;False&quot;.

Filteret, hvis specificeret, har formen &apos;kommentar (*.type *.type2 ...)&apos;.
For eksempel &apos;billeder&apos; (*.png *.xpm *.jpg)&apos;.

Se Qt-dokumentationen over QFileDialog for detaljer om filtre.

Eksempel: fileDialog(&apos;Åbn input&apos;, &apos;CSV filer (*.csv)&apos;)
Eksempel: fileDialog(&apos;Gem report&apos;, standardnavn=&apos;report.txt&apos;, er-gem=True)
</translation>
    </message>
    <message>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation>getPropertyCType(objekt, egenskab, inkludérsuper=True)

Returnerer navnet på `egenskab&apos;ens C type i det givne `objekt&apos;. Se getProperty()

Hvis `inkludérsuper&apos; er `TRUE&apos;, søges også nedarvede egenskaber.
</translation>
    </message>
    <message>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation>getPropertyNames(objekt, inkludérsuper=True)

Returnerer en liste af egenskabs navne understøttet af `objekt&apos;.
Hvis `inkludérsuper&apos; er `TRUE&apos;, returneres også navnene på egenskaber
understøttet af forældreklassen.
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
        <translation>getProperty(objekt, egenskab)

Returnerer værdien af  egenskab `egenskab&apos;  i det givne `objekt&apos;.

Argumentet `object&apos; kan være en streng,  og der vil da blive søgt efter det 
navngivne Sideelement. Det kan også være et PyObjekt, som kan pege på 
et hvilket som helst C++ QObjekt tilfælde.

Argumentet `egenskab&apos; skal være en streng, og er navnet på den egenskab
der søges efter i `objekt&apos;.

Værdien der returneres er forskellig afhængig af egenskabens type.
</translation>
    </message>
    <message>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation>setProperty(objekt, egenskab, værdi)

Sætter `egenskab&apos; af `objekt&apos; til `værdi&apos;. Hvis `værdi&apos; ikke kan konverteres til en type
der er forenelig med typen af `egenskab&apos;, vil en indsigelse blive rejst. En indsigelse
kan også blive rejst, hvis den grundlæggende sætter mislykkes.

Se getProperty() for mere information.
</translation>
    </message>
    <message>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation>getChildren(objekt, afklasse=None, afnavn=None, regexpmatch=False, rekursiv=True)

Returnerer en liste af børn af `objekt&apos;, muligvis begrænset til børn
af klasse med navnet `afklasse&apos; eller børn med navnet `afnavn&apos;. Hvis `rekursiv&apos;
er True, søges rekursivt gennem børn, børnebørn, etc.

See QObject::children() i Qt dokumentationen for mere information.
</translation>
    </message>
    <message>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation>getChild(objekt, barnenavn, afklasse=None, rekursiv=True)

Returnerer det første barn af `objekt&apos; med navnet `barnenavn&apos;, søgningen 
er muligvis begrænset til børn af klasse med navnet `afklasse&apos;. Hvis `rekursiv&apos;
er True, søges rekursivt gennem børn, børnebørn, etc.
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
        <translation>selectText(start, antal, [&quot;navn&quot;])

Markerer &quot;antal&quot; tegn af teksten i tekst-rammen &quot;navn&quot; startende 
fra tegn &quot;start&quot;. Tegn tælling starter ved 0. Hvis &quot;antal&quot; er nul, vil 
enhver tekstmarkering blive nulstillet.  Hvis &quot;navn&quot; ikke er givet, så bruges 
det markerede element.

Kan rejse IndexError hvis hvis markeringen er sat udenfor teksten.
</translation>
    </message>
    <message>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation>textOverflows([&quot;navn&quot;, ingenlinks]) -&gt; integer

Returnerer det faktiske antal skrifttegn der overløber i tekstramme &quot;navn&quot;. 
Hvis ingenlinks er sat til andet end nul tages kun en ramme - der bruger ikke 
sammenkædning af tekstrammer. Uden denne parameter søges igennem alle 
sammenkædede tekstrammer.

Kan rejse WrongFrameTypeError hvis den givne ramme ikke er en tekstramme
</translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation>newStyleDialog() -&gt; streng

Viser &apos;Lav ny afsnits typografi&apos; vindue. Funktionen returnerer det rigtige
typografi navn eller Intet når bruger annullerer vinduet.
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
        <translation>newDoc(størrelse, margener, retning, førsteSideNummer,
                   måle-enhed, modståendeSider, førsteSideVenstre) -&gt; bool

ADVARSEL: Forældet procedure! brug newDocument i stedet for.

Laver et Nyt dokument og returnerer &quot;true&quot; hvis det lykkedes. Parametrene har 
følgende betydning:

    størrelse = A tuple (bredde, højde) beskriver størrelsen af dokumentet. du kan
    bruge predefinerede constanter med navnet PAPER_&lt;paper_type&gt; f.eks. PAPER_A4 etc.

    margener = En tuple (venstre, højre, top, bund) beskriver documentets margener

    retning = sidens retning - constanter PORTRAIT, LANDSCAPE

    førsteSideNummer = er nummeret på den første side i dokumentet brugt for
    side-nummerering. Selvom værdien oftest er 1, så er der nogen gange brug for 
    et højere nummer, når et dokument består af flere dele.

    enhed: denne værdi sætter måle-enheden, som bruges i dokumentet. Brug en
    predefineret constant for dette, en af: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    modståendeSider = FACINGPAGES, NOFACINGPAGES

    førsteSideVenstre = FIRSTPAGELEFT, FIRSTPAGERIGHT

Værdierne for bredde, højde og margener er udtrykt i den givne måle-enhed
for dokumentet. PAPER_* constanter er udtrykt i punkter. Hvis dit dokument
ikke er i punkter, så sørg for at tage højde for dette.

eksempel: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(dobbelt)

Zoom dokumentet i hoved GUI vinduet. Funktionen bruger heltals-værdier
som 20.0, 100.0, etc. Zoom så siden fylder vinduet  bruger -100 som mærke.
</translation>
    </message>
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation>closeMasterPage()

Lukker den aktive masterside, hvis der er nogen, og returnerer 
redigering til normal. Begynd redigering med editMasterPage().
</translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation>masterPageNames()

Returnerer en liste af navnene på alle mastersider i dokumentet.
 </translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation>editMasterPage(sideNavn)

Aktiverer redigering af masterside og åbner den navngivne masterside
for redigering. Afslut redigering med closeMasterPage().
</translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation>createMasterPage(sideNavn)

Laver en masterside med navnet sideNavn og åbner den for
redigering.
</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(sideNavn)

Sletter den navngivne masterside.
</translation>
    </message>
    <message>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>setInfo(&quot;forfatter&quot;, &quot;info&quot;, &quot;beskrivelse&quot;) -&gt; bool
Sætter dokument-informationen.  &quot;forfatter&quot;, &quot;info&quot;, &quot;beskrivelse&quot; er
strenge.
</translation>
    </message>
    <message>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation>duplicateObject([&quot;navn&quot;]) -&gt; string

Laver en dublet at det valgte objekt (eller valgt gruppe).
</translation>
    </message>
    <message>
        <source>getFillTransparency([&quot;name&quot;]) -&gt; float

Returns the fill transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getFillTransparency([&quot;navn&quot;]) -&gt; komma-tal

Returnerer fyld gennemsigtigheden for objektet &quot;navn&quot;.  Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getFillBlendmode([&quot;name&quot;]) -&gt; integer

Returns the fill blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getFillBlendmode([&quot;navn&quot;]) -&gt; heltal

Returnerer fyld blandindings-tilstanden af objektet &quot;navn&quot;. Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getLineTransparency([&quot;name&quot;]) -&gt; float

Returns the line transparency of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineTransparency([&quot;navn&quot;]) -&gt; komma-tal

Returnerer liniens gennemsigtighed af objektet &quot;navn&quot;. Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>getLineBlendmode([&quot;name&quot;]) -&gt; integer

Returns the line blendmode of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineBlendmode([&quot;navn&quot;]) -&gt; heltal

Returnerer linie blandindings-tilstanden af objektet &quot;navn&quot;. Hvis &quot;navn&quot; 
ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setLayerLocked(&quot;layer&quot;, locked)

Sets the layer &quot;layer&quot; to be locked or not. If locked is set to
true the layer will be locked.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerLocked(&quot;lag&quot;, låst)

Sætter laget &quot;lag&quot; til at være låst eller ej. Hvis låst
sættes til &quot;true&quot; vil laget være låst.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>isLayerLocked(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is locked or not, a value of True means
that the layer &quot;layer&quot; is editable, a value of False means that the layer
&quot;layer&quot; is locked.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerLocked(&quot;lag&quot;) -&gt; bool

Returnerer hvorvidt laget &quot;lag&quot; er låst eller ej, værdien &quot;true&quot; betyder
at laget &quot;lag&quot; er redigerbart, værdien &quot;false&quot; betyder at laget &quot;lag&quot; er låst.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>isLayerOutlined(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is outlined or not, a value of True means
that the layer &quot;layer&quot; is outlined, a value of False means that the layer
&quot;layer&quot; is normal.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerOutlined(&quot;lag&quot;) -&gt; bool

Returnerer hvorvidt laget &quot;lag&quot; er vist som omrids eller ej, værdien &quot;true&quot; betyder
at laget &quot;lag&quot; er vist som omrids, værdien &quot;false&quot; betyder at laget &quot;lag&quot; er normalt.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>isLayerFlow(&quot;layer&quot;) -&gt; bool

Returns whether text flows around objects on layer &quot;layer&quot;, a value of True means
that text flows around, a value of False means that the text does not flow around.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerFlow(&quot;lag&quot;) -&gt; bool

Returnerer hvorvidt tekst flyder omkring objekter på laget &quot;lag&quot;, Værdien &quot;true&quot; betyder, 
at tekst flyder omkring objekter, værdien &quot;false&quot; betyder, at tekst ikke flyder omkring objekter

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>getLayerBlendmode(&quot;layer&quot;) -&gt; int

Returns the &quot;layer&quot; layer blendmode,

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>getLayerBlendmode(&quot;lag&quot;) -&gt; heltal

Returnerer laget &quot;lag&apos;s&quot; blandings-tilstand.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>getLayerTransparency(&quot;layer&quot;) -&gt; float

Returns the &quot;layer&quot; layer transparency,

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>getLayerTransparency(&quot;lag&quot;) -&gt; komma-tal

Returnerer laget &quot;lag&apos;s&quot; gennemsigtighed

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
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
        <translation>textFlowMode(&quot;navn&quot; [, tilstand])

Aktiverer/deaktiverer &quot;Tekst flyder omkring ramme&quot; egenskab for objektet &quot;navn&quot;.
Kaldes med parametrene streng &quot;navn&quot; og valgfrit heltal &quot;tilstand&quot; (0 &lt;= tilstand &lt;= 3).
&quot;Tilstand&quot; sat til 0 vil deaktivere tekst flyder omkring objekt.
&quot;Tilstand&quot; sat til 1 vil få tekst til at flyde omkring objekt ramme.
&quot;Tilstand&quot; sat til 2 vil få tekst til at flyde omkring indbindings boks.
&quot;Tilstand&quot; sat til 3 vil få tekst til at flyde omkring omrids linien.
Hvis &quot;tilstand&quot; ikke er angivet vil tilstanden blive slået til eller fra.
</translation>
    </message>
    <message>
        <source>getPageType() -&gt; integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
</source>
        <translation>Returnerer sidens type, 0 betyder venstre side, 1 er en midterside og 2 er en højre side
</translation>
    </message>
    <message>
        <source>getTextLines([&quot;name&quot;]) -&gt; integer

Returns the number of lines of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLines([&quot;navn&quot;]) -&gt; heltal

Returnerer antal tekst-linier i tekst-rammen &quot;navn&quot;.
Hvis &quot;navn&quot; ikke angives,  så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation>defineColor(&quot;navn&quot;, c, m, y, k)

Definerer en ny farve &quot;navn &quot;. Farveværdien er defineret via fire komponenter:
 c = Cyan, m = Magenta, y = Gul og k = sort. Farve-komponenterne skal være 
i området fra 0 til 255.

Kan rejse ValueError hvis et ugyldigt farve-navn er angivet.
</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getCornerRadius([&quot;navn&quot;]) -&gt; integer

Returnerer radius på hjørnerne på objektet &quot;navn&quot;. Radius er 
udtrykt i punkter. Hvis &quot;navn&quot; ikke angives, så bruges 
det markerede element.
</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation>getPosition([&quot;navn&quot;]) -&gt; (x,y) 

Returnerer en (x, y) tuple med positionen på objektet &quot;navn&quot;.  
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element. 
Positionen er udtrykt i den aktuelle måle-enhed for dokumentet
- se  UNIT_&lt;type&gt; for reference.
</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>rotateObjectAbs(rot [, &quot;navn&quot;])

Sætter rotationen af objektet &quot;navn&quot; til &quot;rot&quot;. Positive værdier 
angiver drejning mod uret. Hvis &quot;navn&quot; ikke angives, så bruges 
det markerede element. 
 </translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the
printable set to false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;lag&quot;, printbar)

Sætter laget &quot;lag&quot; til at kunne udskrives eller ej. Hvis 
&quot;printbar&quot; sættes til &quot;False&quot;, så bliver laget ikke udskrivet.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>isLayerVisible(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerVisible(&quot;lag&quot;) -&gt; bool

Returnerer hvorvidt laget &quot;lag&quot; er synligt eller ej, værdien &quot;True&quot; 
betyder, at laget &quot;lag&quot; er synligt, værdien &quot;False&quot; betyder, at laget 
&quot;lag&quot; er usynligt.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting text at position -1 appends it to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation>insertText(&quot;tekst&quot;, pos, [&quot;navn&quot;])

Indsætter teksten &quot;tekst&quot; ved position &quot;pos&quot; i tekst-rammen &quot;navn&quot;. 
Teksten skal være UTF kodet (se  setText() som reference). Første tegn
har index 0. Indsættes tekst ved position -1, så tilføjes teksten i slutningen 
af  rammen. Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse IndexError for en indsætning uden for et gyldigt område.
</translation>
    </message>
    <message>
        <source>setLayerOutlined(&quot;layer&quot;, outline)

Sets the layer &quot;layer&quot; to be locked or not. If outline is set to
true the layer will be displayed outlined.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerOutlined(&quot;lag&quot;, omrids)

Sætter laget &quot;lag&quot; til at blive vist som omrids eller ej. Hvis omrids 
er sat til &quot;true&quot; vil laget blive vist som omrids.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>setLayerFlow(&quot;layer&quot;, flow)

Sets the layers &quot;layer&quot;  flowcontrol to flow. If flow is set to
true text in layers above this one will flow around objects on this layer.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerFlow(&quot;lag&quot;, flyd)

sætter laget &quot;lag&apos;s&quot; flydningskontrol til flyd. Hvis flyd sættes til &quot;true&quot;,
vil tekst i lag over dette flyde rundt om objekter i dette lag.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>setLayerBlendmode(&quot;layer&quot;, blend)

Sets the layers &quot;layer&quot;  blendmode to blend.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerBlendmode(&quot;lag&quot;, blanding)

Sætter laget &quot;lag&apos;s&quot;   blandings-tilstand til blanding. 

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis lagets navn ikke kan accepteres.
</translation>
    </message>
    <message>
        <source>setLayerTransparency(&quot;layer&quot;, trans)

Sets the layers &quot;layer&quot;  transparency to trans.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerTransparency(&quot;lag&quot;, trans)

Sætter laget &quot;lag&apos;s&quot;  gennemsigtighed til trans.

Kan rejse NotFoundError hvis laget ikke findes.
Kan rejse ValueError hvis et ugyldigt lag-navn er angivet.
</translation>
    </message>
    <message>
        <source>setFillTransparency(transparency, [&quot;name&quot;])

Sets the fill transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>setFillTransparency(gennemsigtighed, [&quot;navn&quot;])

Sætter fyld-gennemsigtigheden af objektet &quot;navn&quot; til gennemsigtighed
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setFillBlendmode(blendmode, [&quot;name&quot;])

Sets the fill blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>setFillBlendmode(blandingstilstand, [&quot;navn&quot;])

Sætter fyld-blandings-tilstanden af objektet &quot;navn&quot; til blandingstilstand
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setLineTransparency(transparency, [&quot;name&quot;])

Sets the line transparency of the object &quot;name&quot; to transparency
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>setLineTransparency(gennemsigtighed, [&quot;navn&quot;])

Sætter linie-gennemsigtigheden af objektet &quot;navn&quot; til gennemsigtighed
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.
</translation>
    </message>
    <message>
        <source>setLineBlendmode(blendmode, [&quot;name&quot;])

Sets the line blendmode of the object &quot;name&quot; to blendmode
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>setLineBlendmode(blandingstilstand, [&quot;navn&quot;])

Sætter linieblandings-tilstanden af objektet &quot;navn&quot; til blandingstilstand
Hvis &quot;navn&quot; ikke angives, så bruges det markerede element.</translation>
    </message>
    <message>
        <source>placeEPS(&quot;filename&quot;, x, y)

Places the EPS &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the EPS placed on the page

If loading was successful, the selection contains the imported EPS
</source>
        <translation>placeEPS(&quot;filnavn&quot;, x, y)

Placerer EPS &quot;filnavn&quot; på den aktive side,
x og y specificerer coordinaten af øverste venstre hjørne af EPS&apos;en på siden

hvis indlæsning lykkedes, indeholder markeringen den importerede EPS
</translation>
    </message>
    <message>
        <source>placeSXD(&quot;filename&quot;, x, y)

Places the SXD &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the SXD placed on the page

If loading was successful, the selection contains the imported SXD
</source>
        <translation>placeSXD(&quot;filnavn&quot;, x, y)

Placerer SXD &quot;filnavn&quot; på den aktive side,
x og y specificerer coordinaten af øverste venstre hjørne af SXD&apos;en på siden

hvis indlæsning lykkedes, indeholder markeringen den importerede SXD
</translation>
    </message>
    <message>
        <source>placeODG(&quot;filename&quot;, x, y)

Places the ODG &quot;filename&quot; onto the current page,
x and y specify the coordinate of the topleft corner of the ODG placed on the page

If loading was successful, the selection contains the imported ODG
</source>
        <translation>placeODG(&quot;filnavn&quot;, x, y)

Placerer ODG &quot;filnavn&quot; på den aktive side,
x og y specificerer coordinaten af øverste venstre hjørne af ODG&apos;en på siden

hvis indlæsning lykkedes, indeholder markeringen den importerede ODG
</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Contributions from:</source>
        <translation>Bidrag fra:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Om</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Forfattere</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>Over&amp;sættelser</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Internet</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Udviklings-holdet:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Officiel dokumentation:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Anden dokumentation:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hjemmeside</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Dokumentation på nettet</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Fejlreportering og anmodning om nye funktioner</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Postliste</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Officielle oversættelser og oversættere:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Tidligere oversættelse fra:</translation>
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
        <translation>%1 %2 %3</translation>
    </message>
    <message>
        <source>%3-%2-%1 %4 %5</source>
        <translation>%3-%2-%1 %4 %5</translation>
    </message>
    <message>
        <source>Using Ghostscript version %1</source>
        <translation>Bruger Ghostscript version %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ingen Ghostscript version tilgængelig</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Build ID:</translation>
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
        <translation>Tango Projekt Ikoner:</translation>
    </message>
    <message>
        <source>&amp;Updates</source>
        <translation>&amp;Opdateringer</translation>
    </message>
    <message>
        <source>Check for &amp;Updates</source>
        <translation>Tjek for &amp;Opdateringer</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected. The Windows version does not use fontconfig or CUPS libraries.</source>
        <translation>Dette panel viser versionsnummer, dato for kompilering og den indbyggede biblioteksunderstøttelse i Scribus. C-C-T-F står for C=littlecms C=CUPS T=TIFF F=Fontconfig understøttelse. Sidste bogstav viser renderingsbiblioteket C=cairo eller A=libart. Manglende biblioteksunderstøttelse er vist ved en *. Det viser også den version af Ghostscript, som Scribus har fundet. Windows versionen bruger ikke fontconfig eller CUPS biblioteker.</translation>
    </message>
    <message>
        <source>Check for updates to Scribus. No data from your machine will be transferred off it.</source>
        <translation>Tjek for opdateringer til Scribus. Der vil ikke blive sendt nogen data væk fra din maskine.</translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>OS/2&amp;#174;/eComStation&amp;#8482; Port:</translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Filename:</source>
        <translation>Filnavn:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Version:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Aktiveret:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Udgivelsesdato:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beskrivelse:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Forfatter(e):</translation>
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
        <translation>Scribus: Om Plug-ins</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
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
        <translation>&amp;Nyt</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åbn...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gem &amp;som...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Genindl&amp;æs det gemte</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Saml &amp;projekt i mappe...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Hent tekst...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Tilføj &amp;tekst...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Hent billede...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Gem &amp;tekst...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Gem som P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Dokument indstillinger...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Udskriv...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Afslut</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Fortryd</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Lav igen</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Element handlings-tilstand</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Ma&amp;rkér alt</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Fjern al markering</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søg/erstat...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Redigér billede...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Far&amp;ver...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Master sider...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>Pr&amp;æferencer...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Andet...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Venstre</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Center</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Højre</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blok</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Forceret</translation>
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
        <translation>&amp;Understreg</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Understreg &amp;ord</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Gennemstreg</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;Alt som store bogstaver</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>Små &amp;Kapitæler</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;Hævet skrift</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>S&amp;ænket skrift</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>Sk&amp;ygge</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Billed effekter</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorer...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikér</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Duplikér &amp;flere gange</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Lav til &amp;gruppe</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Bryd g&amp;ruppe</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Er &amp;låst</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>St&amp;ørrelse er låst</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Placér &amp;bagest</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Placér &amp;forrest</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Flyt bag&amp;ud</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Flyt f&amp;orud</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Placér i s&amp;crapbog</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>K&amp;valiteter...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Billede er &amp;synlig</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Opdatér billede</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Justér ramme til billede</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Udvidede billed egenskaber</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Lav Opløsning</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Normal opløsning</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Fuld opløsning</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Er PDF-&amp;Bogmærke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Er PDF-&amp;Kommentar</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Kommentar e&amp;genskaber</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Felt ege&amp;nskaber</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Re&amp;digér form...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Fastgør &amp;tekst til sti</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Frigør t&amp;ekst fra sti</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Forbind &amp;polygoner</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>&amp;Opdel polygoner</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier-kurve</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>B&amp;illed ramme</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Polygon</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Tekstramme</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Glyf...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Prøvetekst</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Indsæt...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>Im&amp;portér...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Slet...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopiér...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Flyt...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Anvend masterside...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Håndtér &amp;hjælpelinier...</translation>
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
        <translation>Miniat&amp;urer</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Vis &amp;margener</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Vis &amp;rammer</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Vis &amp;billeder</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Vis &amp;gitter</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Vis &amp;hjælpelinier</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Vis b&amp;asislinie-gitter</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Vis &amp;tekstkæde</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Vis kontrol tegn</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Klæb til gitter</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Kl&amp;æb til hjælpelinier</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Egenskaber</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Scrapbog</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Lag</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Arrangér sider</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bogmærker</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Målinger</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Handlings historie</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Preflight efterprøvning</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>Ops&amp;til på linie og fordel</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Værktøjer</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF-værktøjer</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Markér element</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abel</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>For&amp;m</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linie</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Frihånds linie</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotér objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zoom ind eller ud</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zoom ind</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zoom ud</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Redigér indhold af ramme</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Redigér tekst...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Sammenkæd tekstrammer</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Bryd sammenkædning af tekstrammer</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Farveopsnapper</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopiér element egenskaber</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Redigér teksten med den indbyggede tekstbehandler</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Indsæt tekstramme</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Indsæt billedramme</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Indsæt tabel</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Indsæt figur</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Indsæt polygon</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Indsæt linie</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Indsæt bezierkurve</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Indsæt frihånds-linie</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Håndtér &amp;billeder</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Orddel tekst</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Fjern orddeling af tekt</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Generér indholdsfortegnelse</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Om Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Om &amp;QT</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Hj&amp;ælpe-tekst</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;Manual...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>Smart &amp;orddeling</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Ikke brydbar bindestreg</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Ikke brydbar &amp;mellemrum</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Side&amp;nummer</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Ny linie</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Ramme-skift</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Kolonne-skift</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Ophavsret</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registreret varemærke</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Varemærke</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Em streg</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>En streg</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Figur tegn</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Citat tegn</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Slå paletter til/fra</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Slå hjælpelinier til/fra</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Håndtér side egenskaber...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Linialer relativt til side</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Bagved hinanden</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>Ved &amp;siden af hinanden</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>&amp;Vis udskrift</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>konvertér til masterside...</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>Om &amp;Plug-ins</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Mere Info...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>&amp;Oversigt</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Indsæt smart orddeling</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Indsæt ikke brydbar bindestreg</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Indsæt ikke brydbar mellemrum</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Indsæt sidenummer</translation>
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
        <translation>&amp;Typografier &amp;&amp; Stile...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Omrids</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Udskrivning aktiveret</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>&amp;Flip horisontalt</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>&amp;Flip vertikalt</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Kurver</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Vis linealer</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Solidus</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Prik</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>En spatie</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Em spatie</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>tynd spatie</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Tyk spatie</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Mid spatie</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Hår spatie</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Indsæt (&amp;Ubetinget)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation>Vis kolonner i tekstramme</translation>
    </message>
    <message>
        <source>Save as &amp;EPS...</source>
        <translation>Gem som &amp;EPS...</translation>
    </message>
    <message>
        <source>Patterns...</source>
        <translation>Mønstre...</translation>
    </message>
    <message>
        <source>Send to Patterns</source>
        <translation>Send til mønstre</translation>
    </message>
    <message>
        <source>&amp;Frame...</source>
        <translation>&amp;Ramme...</translation>
    </message>
    <message>
        <source>Preview Mode</source>
        <translation>Forhåndvisnings-tilstand</translation>
    </message>
    <message>
        <source>Show Bleeds</source>
        <translation>Vis Tryk til kant</translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation>Vis lag-indikatorer</translation>
    </message>
    <message>
        <source>Insert PDF Push Button</source>
        <translation>Indsæt PDF trykknap</translation>
    </message>
    <message>
        <source>Insert PDF Text Field</source>
        <translation>Indsæt PDF tekstfelt</translation>
    </message>
    <message>
        <source>Insert PDF Check Box</source>
        <translation>Indsæt PDF afkrydsnings boks</translation>
    </message>
    <message>
        <source>Insert PDF Combo Box</source>
        <translation>Indsæt PDF kombo boks</translation>
    </message>
    <message>
        <source>Insert PDF List Box</source>
        <translation>Insæt PDF liste boks</translation>
    </message>
    <message>
        <source>Insert Text Annotation</source>
        <translation>Insæt tekst kommentar</translation>
    </message>
    <message>
        <source>Insert Link Annotation</source>
        <translation>Indsæt lænke kommentar</translation>
    </message>
    <message>
        <source>&amp;Zero Width Space</source>
        <translation>&amp;Nul-bredde mellemrum</translation>
    </message>
    <message>
        <source>Zero Width NB Space</source>
        <translation>Nul-bredde IB mellemrum</translation>
    </message>
    <message>
        <source>&amp;Fit to Height</source>
        <translation>&amp;Tilpas til højde</translation>
    </message>
    <message>
        <source>Fit to Width</source>
        <translation>Tilpas til bredde</translation>
    </message>
    <message>
        <source>Sticky Tools</source>
        <translation>Klæb til værktøjer</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <comment>Unicode 0x0027</comment>
        <translation>Apostrof</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <comment>Unicode 0x0022</comment>
        <translation>Lige dobbelt</translation>
    </message>
    <message>
        <source>Single Left</source>
        <comment>Unicode 0x2018</comment>
        <translation>Enkelt venstre</translation>
    </message>
    <message>
        <source>Single Right</source>
        <comment>Unicode 0x2019</comment>
        <translation>Enkelt højre</translation>
    </message>
    <message>
        <source>Double Left</source>
        <comment>Unicode 0x201C</comment>
        <translation>Dobbelt venstre</translation>
    </message>
    <message>
        <source>Double Right</source>
        <comment>Unicode 0x201D</comment>
        <translation>Dobbelt højre</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <comment>Unicode 0x201B</comment>
        <translation>Enkelt vendt om</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <comment>Unicode 0x201F</comment>
        <translation>Dobbelt vendt om</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <comment>Unicode 0x2039</comment>
        <translation>Enkelt venstre Guillemet</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <comment>Unicode 0x203A</comment>
        <translation>Enkelt højre Guillemet</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <comment>Unicode 0x00AB</comment>
        <translation>Dobbelt venstre Guillemet</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <comment>Unicode 0x00BB</comment>
        <translation>Dobbelt højre Guillemet</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <comment>Unicode 0x201A</comment>
        <translation>Lavt enkelt komma</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <comment>Unicode 0x201E</comment>
        <translation>Lavt dobbelt komma</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <comment>Unicode 0x300C</comment>
        <translation>CJK Enkelt venstre</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <comment>Unicode 0x300D</comment>
        <translation>CJK Enkelt højre</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <comment>Unicode 0x300E</comment>
        <translation>CJK Dobbelt venstre</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <comment>Unicode 0x300F</comment>
        <translation>CJK Dobbelt højre</translation>
    </message>
</context>
<context>
    <name>AlignDistributeBase</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Opstil på linie og fordel</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Opstil på linie</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Markerede hjælpelinier:</translation>
    </message>
    <message>
        <source>&amp;Relative To:</source>
        <translation>&amp;Relativt til:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Fordel</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Afstand:</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Opstil på linie og fordel</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativt til:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Først markerede</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Sidst markerede</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Margener</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hjælpelinie</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Markering</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Stil bund-sider på linie</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Stil højre sider på linie</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Centrér omkring vertikal akse</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Stil venstre sider på linie</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Centrér omkring horisontal akse</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Stil top-sider på linie</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Markerede hjælpelinier:</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Fordel højre sider med lige stor afstand</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Fordel bund-sider med lige stor afstand</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Fordel center af objekter med lige stor afstand horisontalt </translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Fordel venstre sider med lige stor afstand</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Fordel center af objekter med lige stor afstand vertikalt</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Fordel top-sider med lige stor afstand</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Afstand:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Fordel elementerne med det med anførte mellemrum</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Intet markeret</translation>
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
        <source>Align</source>
        <translation>Opstil på linie</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Fordel</translation>
    </message>
    <message>
        <source>Align right sides of items to left side of anchor</source>
        <translation>Stil højre sider af elementer på linie efter venstre side af anker</translation>
    </message>
    <message>
        <source>Align left sides of items to right side of anchor</source>
        <translation>Stil venstre sider af elementer på linie efter højre side af anker</translation>
    </message>
    <message>
        <source>Align tops of items to bottom of anchor</source>
        <translation>Stil top-sider af elementer på linie efter bunden af anker</translation>
    </message>
    <message>
        <source>Align bottoms of items to top of anchor</source>
        <translation>Stil bund-sider af elementer på linie efter toppen af anker</translation>
    </message>
    <message>
        <source>Make horizontal gaps between items equal</source>
        <translation>Lav horisontale mellemrum mellem elementer lige store</translation>
    </message>
    <message>
        <source>Make horizontal gaps between items equal to the value specified</source>
        <translation>Lav horisontale mellemrum mellem elementer lig med anførte værdi</translation>
    </message>
    <message>
        <source>Make vertical gaps between items equal</source>
        <translation>Lav vertikale mellemrum mellem elementer lige store</translation>
    </message>
    <message>
        <source>Make vertical gaps between items equal to the value specified</source>
        <translation>Lav vertikale mellemrum mellem elementer lig med anførte værdi</translation>
    </message>
    <message>
        <source>Make horizontal gaps between items and sides of page equal</source>
        <translation>Lav horisontale mellemrum mellem elementer og sider af side ens</translation>
    </message>
    <message>
        <source>Make vertical gaps between items and the top and bottom of page equal</source>
        <translation>Lav vertikale mellemrum mellem elementer og top og bund af side ens</translation>
    </message>
    <message>
        <source>Make horizontal gaps between items and sides of page margins equal</source>
        <translation>Lav horisontale mellemrum mellem elementer og sider af side-margener ens</translation>
    </message>
    <message>
        <source>Make vertical gaps between items and the top and bottom of page margins equal</source>
        <translation>Lav vertikale mellemrum mellem elementer og top og bund af side-margener ens</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Justér mod venstre</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Centrér</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Justér mod højre</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Lige margener</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Tvunget lige margener</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Destination</source>
        <translation>Bestemmelsessted</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Tryk</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Stjerne</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tynd</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Tid</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Bred</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Sæt valutasymbol foran</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Send form</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Maksimalt</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Afkrydsning</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>kryds</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Felt har fokus</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikoner</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>indsæt</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Side:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andet</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Almindelig</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Massiv</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Liste boks</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Brug valutasymbol</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Nummerformat</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>afkrydsningsstil:</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Til fil:</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ingen udskrift</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Send til URL:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Udregn</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Rul over</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Værdi er ikke valideret</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>maksimum</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatering</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombo boks</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Bruger validerings script:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Grænse</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datoformat</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knap</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Cirkel</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farve:</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation>Felt egenskaber</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Punkteret</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brugerdefineret</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Tidsformat</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Begivenhed:</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Værdien er</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Skjult</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Bruger udregnings script:</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Indtast en komma-adskilt liste af felter her</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Bruger scripts</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertér</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Ingen visning</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normalt</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Nummer</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Firkant</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Felt er formateret som:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Felt mister fokus</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Send Data som HTML</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstfelt</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Ikon placering...</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mus-taste slippes</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Vælg...</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Markøren går ind i felt</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Brug ikoner</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Skrivebeskyttet</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Trykket ned</translation>
    </message>
    <message>
        <source>product</source>
        <translation>Produkt</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>For Rul over</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validér</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Vælg...</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Procent format</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Skrifttegn</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Adgangskode</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Synlighed:</translation>
    </message>
    <message>
        <source>average</source>
        <translation>Gennemsnit</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Med skrå kanter</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importér data</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Markøren forlader felt</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mus-taste trykkes ned</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Procentdel</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimaler:</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>af de følgende felter:</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importér data fra:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>og mindre eller lig med:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Tasteanslag:</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multi-linie</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understreg</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamand</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Redigérbar</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Udseende</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Afkrydsnings boks</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Værdi er ikke udregnet</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Redigér...</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Nulstil form</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Behøves</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ingen stavekontrol</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Indrammet</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Synlig</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Værdi skal være lig med eller større end:</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Højlys</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Standard er afkrydset</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Eksempel:</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ingen rulning</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>For knap ned</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Du behøver i det mindste normal-ikonet for at bruge ikoner på knapper</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Ændring af valg</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Pos:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Pos:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>sum</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>minimum</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Font til brug i PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Kan ikke benyttes i  PDF-1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF filer (*.pdf);;Alle filer (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Tooltip:</source>
        <translation>Værktøj-tip:</translation>
    </message>
    <message>
        <source>Do Not Export Value</source>
        <translation>Eksportér ikke værdi</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;All Files (*)</source>
        <translation>Billeder (*.tif *.png *.jpg *.xpm);;PostScript (*.eps *.epsi);;Alle filer (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Destination</source>
        <translation>Bestemmelsessted</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lænke</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Ekstern web-Lænke</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation>Kommentar egenskaber</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Dokumenter (*.pdf);; Alle filer (*)</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Ekstern Lænke</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>V&amp;ælg...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Side:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Pos</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Apply Master Page</source>
        <translation>Anvend masterside</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Masterside:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Anvend på</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Denne &amp;side</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Lige sider</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;Ulige sider</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>A&amp;lle sider</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;Fra side</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>to</source>
        <translation>til</translation>
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
        <translation>Anvend den valgte masterside på lige, ulige eller alle sider indenfor de valgte sidernumre</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>&amp;Stregkode generator...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Scribus brugerflade for ren Postscript stregkode skriver</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Fejl ved åbning af fil: %1</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 eller 13 cifret</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 cifret</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 eller 12 cifret</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 eller 8 cifret</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 cifret</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 cifret</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Variabel antal tegn, cifre og  enhver af følgende symboler -. *$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Variabel antal ASCII tegn og specielle funktions symboler, der starter med det rigtige start-tegn for det indledende tegnsæt. UCC/EAN-128s skal have et obligatorisk FNC 1 symbol umiddelbart efter start-tegnet.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Variabel antal cifre og enhver af følgende symboler -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Variabel antal cifre. En ITF-14 er på 14 tegn og har ikke et kontrol-ciffer</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Variabel antal cifre</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Variabel antal cifre og store bogstaver</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Variabel antal hexadecimal tegn</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Stregkode er mangelfuld</translation>
    </message>
    <message>
        <source>12 or 13 digits with dashes. The legacy ISBN-10 format accepts 9 or 10 digits with dashes, but this standard was depreciated for public use after 1st January 2007. (Note: To convert an old ISBN-10 to a new ISBN-13, prefix 978- to the first 9 digits, e.g. 1-56592-479-7 -&gt; 978-1-56592-479. The final check-digit will be calculated automatically.)</source>
        <translation>12 eller 13 cifre med tankestreger. Det forældede ISBN-10 format accepterer 9 eller 10 cifre med tankestreger, men denne standard er forældet efter 1. januar 2007. (Note: For at konvertere et gammel ISBN-10 til et nyt  ISBN-13, så sæt 978- foran de første 9 cifre, f.eks. 1-56592-479-7 -&gt; 978-1-56592-479. Det sidste tjekciffer bliver kalkuleret automatisk.)</translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>Stregkode fremstilling</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>stregkode</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Vælg en af de tilgængelige stregkodetyper her</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Den numeriske repræsentation af selve koden. Se hjælpeteksten nedenunder</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Nulstil stregkode prøverne</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Inkludér tekst i stregkode</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>sæt kryds for også at få tal i stregkoden</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>Besk&amp;yt mellemrum</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>tegn pile for at være sikker på mellemrum ved siden af koden</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Baggrund</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Baggrundsfarve - under kode linierne</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Linier</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Farve på linierne i stregkoden</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>Te&amp;kst</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Farve på tekst og tal</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Råd og hjælp er vist her</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Visning af resultatet. 72dpi prøve.</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>Ko&amp;de:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>I&amp;nkludér kontrolsum</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Generér og inkludér en kontrolsum i stregkode</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Inkl&amp;udér kontrolsum ciffer</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Inkludér kontrolsum-cifret i stregkode-teksten</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Delete</source>
        <translation>Slet</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbog</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Ny indgang</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Nyt navn</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.
Vælg venligst et andet.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Vælg en scrapbog-mappe</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Scrapbog  (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Vælg en scrapbog-fil der skal importeres</translation>
    </message>
    <message>
        <source>Main</source>
        <translation>Hovedbog</translation>
    </message>
    <message>
        <source>Copied Items</source>
        <translation>Kopierede elementer</translation>
    </message>
    <message>
        <source>Copy To:</source>
        <translation>Kopiér til:</translation>
    </message>
    <message>
        <source>Move To:</source>
        <translation>Flyt til:</translation>
    </message>
    <message>
        <source>Create a new scrapbook page</source>
        <translation>Lav en ny scrapbog-side</translation>
    </message>
    <message>
        <source>Load an existing scrapbook</source>
        <translation>Indlæs en eksisterende scrapbog</translation>
    </message>
    <message>
        <source>Save the selected scrapbook</source>
        <translation>Gem den valgte scrapbog</translation>
    </message>
    <message>
        <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
        <translation>Importér en scrapbog-fil fra Scribus &lt;=1.3.2</translation>
    </message>
    <message>
        <source>Close the selected scrapbook</source>
        <translation>Luk den valgte scrapbog</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Flyt bogmærke</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Bogmærker</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Indsæt bogmærke</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bogmærker</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon</source>
        <translation>Ikon</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Aldrig</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Nulstil</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Tekst oven på ikon</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skalér:</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Udformning</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Kun tekst</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Tekst under ikon</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Altid</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skalér:</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Kun ikon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Tekst til venstre for ikon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Tekst til højre for ikon</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Når ikon er for stor</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Ikke Proportionel</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proportionel</translation>
    </message>
    <message>
        <source>Icon Placement</source>
        <translation>Ikon placering</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Tekst over ikon</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Når ikon er for lille</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolorimetri</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>System profiler</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Farvetilpasningsmetoder</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Opfattelsesmæssig</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolorimetri</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Farveprofil som du har lavet eller har modtaget fra producenten.
Denne profil bør være beregnet for din skærm og ikke en generel profil (d.v.s sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Farveprofil for din printer fra producenten.
Denne profil bør være beregnet for din printer og ikke en generel profil (d.v.s sRGB).</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Sortpunkt kompensation er en metode til at forbedre kontrasten i fotografier.
Det anbefales at bruge dette, hvis der er fotografier i dokumentet.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Viser på skærmen hvordan dokumentets farver udskrives,
baseret på den valgte printerprofil.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Metode til at vise på skærmen de farver, som måske ikke udskrives rigtigt.
Dette kræver meget nøjagtige profiler og er kun ment som en advarsel.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktiver farvestyring</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Skærm:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Printer:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulér printer på skærmen</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Markér farver uden for s&amp;kalaen (gamut)</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Bru&amp;g sortpunkt kompensation</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB billeder:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK billeder:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Standard Farveprofil for importerede CMYK billeder</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Standard Farveprofil for importerede RGB billeder</translation>
    </message>
    <message>
        <source>&amp;RGB Solid Colors:</source>
        <translation>&amp;RGB massive farver:</translation>
    </message>
    <message>
        <source>&amp;CMYK Solid Colors:</source>
        <translation>&amp;CMYK massive farver:</translation>
    </message>
    <message>
        <source>Pictures:</source>
        <translation>Billeder:</translation>
    </message>
    <message>
        <source>Sol&amp;id Colors:</source>
        <translation>Mass&amp;ive farver:</translation>
    </message>
    <message>
        <source>Convert all colors to printer space</source>
        <translation>Konvertér alle farver til printer rum</translation>
    </message>
    <message>
        <source>Default color profile for solid RGB colors on the page</source>
        <translation>Standard Farveprofil for massive RGB farver på siden</translation>
    </message>
    <message>
        <source>Default color profile for solid CMYK colors on the page</source>
        <translation>Standard Farveprofil for massive CMYK farver på siden</translation>
    </message>
    <message>
        <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard gengivelse beregnet for din skærm. Hvis du ikke ved hvorfor man ændrer det,
vælg da relativ kolorimetri eller opfattelsesmæssig.</translation>
    </message>
    <message>
        <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard gengivelse beregnet for din skærm. Hvis du ikke ved hvorfor man ændrer det,
vælg da relativ kolorimetri eller opfattelsesmæssig.</translation>
    </message>
    <message>
        <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
        <translation>Simulér en fuldigstændig farvestyret omgivelse:
alle farver, rgb eller cmyk, er konverteret til printerens farverum.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>New</source>
        <translation>Ny</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Gammel</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation>Web-sikker RGB</translation>
    </message>
    <message>
        <source>Edit Color</source>
        <translation>Redigér farve</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statiske farvesøjler</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Dynamiske farvesøjler</translation>
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
        <source></source>
        <translation></translation>
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
        <translation>HSV-Farvekort</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Farve &amp;model</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Er spot-farve</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Er pasmærke-farve</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Du kan ikke lave en farve, der hedder &quot;%1&quot;.
Navnet er reserveret for gennemsigtig farve</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation>Ved at vælge dette vil aktivere udskrivning af dette på alle plader. Pasmærke-farver bruges f.eks til skæremærker, pasmærker og lignende. Disse er ikke typisk brugt i selve sidens layout.</translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation>Ved at vælge dette vil gøre denne farve til en spot-farve, og således lave endnu en spot, når der laves plader eller separationer. Det er ofte brugt, når et logo eller anden farve behøver at være helt præcis eller ikke kan laves med CMYK trykfarver. Metalliske og fluorescerende farver er gode eksempler på farver, som ikke særlig let kan laves med CMYK trykfarver.</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color may not print exactly as indicated on screen. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation>Hvis farvestyring er aktiveret, vil en trekantet advarsels-indikator betyde, at farven måske er uden for farveskalaen (gamut) i den valgte printer-profil. Hvad dette betyder er, at farven måske ikke trykkes præcist, som det er vist på skærmen. Flere råd om gamut advarsler findes i Scribus manualen under &quot;Color Management&quot;.</translation>
    </message>
    <message>
        <source>The name of the color already exists,
please choose another one.</source>
        <translation>Navnet på farven eksisterer allerede,
vælg venligst et andet navn.</translation>
    </message>
</context>
<context>
    <name>CStylePBase</name>
    <message>
        <source>Form1</source>
        <translation>Form1</translation>
    </message>
    <message>
        <source>Based On:</source>
        <translation>Baseret på:</translation>
    </message>
    <message>
        <source>Basic Formatting</source>
        <translation>Grundlæggende formatering</translation>
    </message>
    <message>
        <source>Advanced Formatting</source>
        <translation>Avanceret formatering</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
</context>
<context>
    <name>CWDialog</name>
    <message>
        <source>Merging colors</source>
        <translation>Sammenfletter farver</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Fejl: </translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Farve %1 findes allerede!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Farve %1 tilføjet.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Nu åbnes farvehåndteringen.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Farve sammenfletning</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Ikke i stand til at finde den forespurgte farve. Du har sansynligvis valgt sort, grå eller hvid. Der er ikke ingen metode til at behandle denne farve.</translation>
    </message>
</context>
<context>
    <name>CWDialogBase</name>
    <message>
        <source>Color Wheel</source>
        <translation>Farvehjul</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
        <translation>Klik på hjulet for at få grundfarven. Dens farve-model afhænger af det valgte faneblad.</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMYK</translation>
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
        <source>RGB:</source>
        <translation>RGB:</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>RGB</translation>
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
        <source>CMYK:</source>
        <translation>CMYK:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Vælg en af metoderne til at lave en farvesammensætning. Se dokumentationen for mere information.</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation>Vinkel:</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Forskel mellem de markerede værdier og de optalte. Se dokumentation for mere information.</translation>
    </message>
    <message>
        <source>Preview:</source>
        <translation>Forhåndsvisning:</translation>
    </message>
    <message>
        <source>Vision Defect Type:</source>
        <translation>Synsevne defekt-type:</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Simulér almindelige synsevne defekter her. Vælg defekt-typen.</translation>
    </message>
    <message>
        <source>Sample color scheme.</source>
        <translation>Farve-sammensætnings prøve.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme.</source>
        <translation>Farverne på din valgte farvesammensætning.</translation>
    </message>
    <message>
        <source>&amp;Merge</source>
        <translation>&amp;Sammenflet</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Sammenflet de nye farver med dokumentets farver</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Erstat</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Erstat dokumentets farver med de nye farver</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Efterlad farver urørt</translation>
    </message>
    <message>
        <source>Color Scheme Method</source>
        <translation>Farve-sammensætnings metode</translation>
    </message>
    <message>
        <source>HSV:</source>
        <translation>HSV:</translation>
    </message>
    <message>
        <source>HSV</source>
        <translation>HSV</translation>
    </message>
    <message>
        <source>Result Colors</source>
        <translation>Resultat farver</translation>
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
        <source> %</source>
        <translation> %</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Font:</source>
        <translation>Font:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Skrifttegn klasse:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Indsæt tegn ved markøren i teksten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Slet de(t) markerede område(r).</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Fuldt tegnsæt</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Grundlæggende latin</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latin-1 tillæg</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latin udvidet-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latin udvidet-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Generel tegnsætning</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Hævet- og sænket skrift</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Mønt symboler</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Bogstavlignende symboler</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Tal former</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Pile</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matematiske operatorer</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Boks tegning</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blok elementer</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometriske figurer</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Diverse symboler</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbats</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Små figur varianter</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligaturer</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Særlige</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Græsk</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Græsk udvidet</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Kyrillisk</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Kyrillisk tillæg</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabisk</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabisk udvidet A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabisk udvidet B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebraisk</translation>
    </message>
    <message>
        <source>Enhanced Palette</source>
        <translation>Udvidet palette</translation>
    </message>
    <message>
        <source>Quick Palette</source>
        <translation>Hurtig palette</translation>
    </message>
    <message>
        <source>Hide Enhanced</source>
        <translation>Skjul udvidet</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Du kan se en miniature, hvis du trykker på den højre musetaste og holder den nede. Insert-tasten indsætter en glyf i feltet for neden og delete-tasten fjerner det sidst indsatte
</translation>
    </message>
    <message>
        <source>Scribus Char Palette (*.ucp);;All Files (*)</source>
        <translation>Scribus Tegn Palette (*.ucp);;Alle Filer (*)</translation>
    </message>
    <message>
        <source>Choose a filename to open</source>
        <translation>Vælg et filnavn som skal åbnes</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fejl</translation>
    </message>
    <message>
        <source>Error reading file %1 - file is corrupted propably.</source>
        <translation>Fejl ved læsning af fil %1 - fil er sansynligvis beskadiget.</translation>
    </message>
    <message>
        <source>Choose a filename to save under</source>
        <translation>Vælg et filnavn til at gemme under</translation>
    </message>
    <message>
        <source>Cannot write file %1</source>
        <translation>Kan ikke skrive fil %1</translation>
    </message>
    <message>
        <source>Clean the Palette?</source>
        <translation>Ryd palette?</translation>
    </message>
    <message>
        <source>You will clean all characters from this palette. Are you sure?</source>
        <translation>Du vil fjerne alle tegn fra denne palette. er du sikker?</translation>
    </message>
    <message>
        <source>Character Palette</source>
        <translation>Skrifttegn palette</translation>
    </message>
</context>
<context>
    <name>CharStyleComboBox</name>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
</context>
<context>
    <name>CharTable</name>
    <message>
        <source>Delete</source>
        <translation>Slet</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Glyffer der mangler</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Tekst overløb</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objekt er ikke på en side</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Manglende billede</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objekt har gennemsigtighed</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekt er en PDF kommentar eller felt</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objekt er en anbragt PDF</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Ingen problemer fundet</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Side </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Frie objekter</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Problemer fundet</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Preflight efterprøvning</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Elementer</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Problemer</translation>
    </message>
    <message>
        <source>Current Profile:</source>
        <translation>Aktive profil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ignorér fejl</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Transparency used</source>
        <translation>Transparens der er brugt</translation>
    </message>
    <message>
        <source>Blendmode used</source>
        <translation>Blandings tilstand der er brugt</translation>
    </message>
    <message>
        <source>Layer &quot;%1&quot;</source>
        <translation>LAG &quot;%1&quot;</translation>
    </message>
    <message>
        <source>Check again</source>
        <translation>Tjek igen</translation>
    </message>
    <message>
        <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
        <translation>Billedopløsning under %1 DPI, nuværende %2 x %3 DPI</translation>
    </message>
    <message>
        <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
        <translation>Billedopløsning over %1 DPI, nuværende %2 x %3 DPI</translation>
    </message>
    <message>
        <source>Image is GIF</source>
        <translation>Billede er GIF</translation>
    </message>
    <message>
        <source>Annotation uses a non TrueType font</source>
        <translation>Kommentering bruger ikke TrueType fonte</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Vælg typografier</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Tilgængelige typografier</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Samler sammen...</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Kan ikke samle alle filer sammen for uddata for fil:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Kan ikke indsamle  filen:
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikér</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Fjern ubrugte</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Farvesæt</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktive farvesæt:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Gem farvesæt</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Vælg et farvesæt som skal indlæses</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Gem det aktive farvesæt</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Fjern ubrugte farver fra det aktive dokuments farvesæt</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importér farver til det aktive sæt fra et eksisterende dokument</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Lav en ny farve i det aktive sæt</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Redigér den valgte farve</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Lav en kopi af den valgte farve</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Slet den valgte farve</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Lav det aktive farvesæt til standard farvesættet</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Vælg et navn</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi af %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Ny farve</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation>Hvis farvestyring er aktiveret, vil en trekantet advarsels-indikator betyde, at farven måske er uden for farveskalaen (gamut) i den valgte printer-profil. Hvad dette betyder er, at farven måske ikke trykkes præcist, som det er vist på skærmen. Spot-farver er vist med en rød cirkel. Pasmærke-farver vil have et pasmærke ved siden af farven. Flere råd om gamut advarsler findes i Scribus manualen under &quot;Color Management&quot;.</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;Other Files (*.eps *.epsi *.ps *.ai);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *.scd.gz);;Andre filer (*.eps *.epsi *.ps *.ai);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importér</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <source>The file %1 does not contain colors which can be imported.
If the file was a PostScript-based, try to import it with File -&amp;gt; Import. 
Not all files have DSC conformant comments where the color descriptions are located.
 This prevents importing colors from some files.
See the Edit Colors section of the documentation for more details.</source>
        <translation>Filen %1 indeholder ikke farver, som kan importeres.
Hvis filen var PostScript-baseret, prøv at importér med Fil -&gt; Importér.
Ikke alle filer har DSC foreskrevne kommentarer hvor farve-beskrivelserne er placeret.
dette forhindre import af farver fra nogle filer.
Se Redigér farver sektionen af dokumentationen for flere detaljer.</translation>
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
        <translation>Analog</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>komplementær</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Opdelt komplementær</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triade</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Tetrade (Dobbelt komplementær)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Basis farve</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Monokromatisk lys</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Monokromatisk  mørk</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>1. Analog</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>2. Analog</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>1. Opdelt</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>2.  Opdelt</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>3.  Opdelt</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>4. Opdelt</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>1. Triade</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>2. Triade</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>1. Tetrade (basis modsætning)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>2. Tetrade (vinkel)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>3. Tetrade (vinkel modsætning)</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Farvehjul...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Farveindstilling-hjælper</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Farvevælger med farveteori inkluderet.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;nvend</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Brugerdefineret</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Enkelt side</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Dobbelt sidet</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3-Fold</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4-Fold</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nej</translation>
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
        <source>Left Page</source>
        <comment>Left page location</comment>
        <translation>Venstre side</translation>
    </message>
    <message>
        <source>Middle</source>
        <comment>Middle page location</comment>
        <translation>Midterste</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <comment>Middle Left page location</comment>
        <translation>Midterste til venstre</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <comment>Middle Right page location</comment>
        <translation>Midterste til højre</translation>
    </message>
    <message>
        <source>Right Page</source>
        <comment>Right page location</comment>
        <translation>Højre side</translation>
    </message>
    <message>
        <source>Normal</source>
        <comment>Default single master page</comment>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <comment>Default left master page</comment>
        <translation>Normal venstre</translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <comment>Default middle master page</comment>
        <translation>Normal midterste</translation>
    </message>
    <message>
        <source>Normal Right</source>
        <comment>Default right master page</comment>
        <translation>Normal højre</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation>Mandag</translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation>Tirsdag</translation>
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
        <translation>Lørdag</translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation>Søndag</translation>
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
        <translation>Marts</translation>
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
        <translation>August</translation>
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
        <translation>december</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <comment>Color Blindness - Normal Vision</comment>
        <translation>Normalt syn</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <comment>Color Blindness - Red Color Blind</comment>
        <translation>Protanopia (Rød)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <comment>Color Blindness - Greed Color Blind</comment>
        <translation>Deuteranopia (Grøn)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <comment>Color Blindness - Blue Color Blind</comment>
        <translation>Tritanopia (Blå)</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <comment>Color Blindness - Full Color Blindness</comment>
        <translation>Fuld farve-blindhed</translation>
    </message>
    <message>
        <source>Custom: </source>
        <comment>Custom Tab Fill Option</comment>
        <translation>Brugerdefineret: </translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Massiv linie</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Streg linie</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Prik linie</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Streg prik linie</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Streg prik prik linie</translation>
    </message>
    <message>
        <source>None</source>
        <comment>Optical Margin Setting</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Left Protruding</source>
        <comment>Optical Margin Setting</comment>
        <translation>Venstre udstående</translation>
    </message>
    <message>
        <source>Right Protruding</source>
        <comment>Optical Margin Setting</comment>
        <translation>Højre udstående</translation>
    </message>
    <message>
        <source>Left Hanging Punctuation</source>
        <comment>Optical Margin Setting</comment>
        <translation>Venstre hængende tegnsætning</translation>
    </message>
    <message>
        <source>Right Hanging Punctuation</source>
        <comment>Optical Margin Setting</comment>
        <translation>Højre hængende tegnsætning</translation>
    </message>
    <message>
        <source>Both sides</source>
        <comment>Optical Margin Setting</comment>
        <translation>Begge sider</translation>
    </message>
    <message>
        <source>Min. Word Tracking</source>
        <translation>Min. ord-spatiering</translation>
    </message>
    <message>
        <source>Norm. Word Tracking</source>
        <translation>Norm. ord-spatiering</translation>
    </message>
    <message>
        <source>Min. Glyph Extension</source>
        <translation>Min. glyf-udvidelse</translation>
    </message>
    <message>
        <source>Max. Glyph Extension</source>
        <translation>Maks. glyf-udvidelse</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Horisontal Gradient</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Ugennemsigtighed:</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Vertikal Gradient</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Kryds Diagonal Gradient</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonal Gradient</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Radial Gradient</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Farvemætning:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Redigér liniefarve egenskaber</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Redigér fyldfarve egenskaber</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Farvemætning</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Normal eller gradient fyldmetode</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Angiv gennemsigtigheden for den valgte farve</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Fri lineær gradient</translation>
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
        <translation> pt</translation>
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
        <source>Free radial Gradient</source>
        <translation>Fri radial gradient</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Flyt vektor</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Flyt starten på gradientvektoren med den venstre museknap trykket ned og flyt slutningen på gradientvektoren med den højre museknap trykket ned</translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation>Gennemsigtigheds indstillinger</translation>
    </message>
    <message>
        <source>Offsets</source>
        <translation>Forskydninger</translation>
    </message>
    <message>
        <source>X:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>Scaling</source>
        <translation>Skalering</translation>
    </message>
    <message>
        <source>X-Scale:</source>
        <translation>X-skalering:</translation>
    </message>
    <message>
        <source>Y-Scale:</source>
        <translation>Y-skalering:</translation>
    </message>
    <message>
        <source>Rotation</source>
        <translation>Rotation</translation>
    </message>
    <message>
        <source>Angle</source>
        <translation>Vinkel</translation>
    </message>
    <message>
        <source>Pattern</source>
        <translation>Mønster</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blandings tilstand:</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Gør mørkere</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Gør lysere</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Gang</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Skærm</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Læg over</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hårdt lys</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Blødt lys</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Forskel</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Udelukkelse</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Farve blegning</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Farve sværtning</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Farvetone</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Lysstyrke</translation>
    </message>
</context>
<context>
    <name>CreateRangeBase</name>
    <message>
        <source>Create Range</source>
        <translation>Lav omfang</translation>
    </message>
    <message>
        <source>Number of Pages in Document:</source>
        <translation>Antal sider i dokument:</translation>
    </message>
    <message>
        <source>Doc Page Range</source>
        <translation>Dok side-omfang</translation>
    </message>
    <message>
        <source>Basic Range Selection</source>
        <translation>Grundlæggende udvælgelse</translation>
    </message>
    <message>
        <source>Add a Range of Pages</source>
        <translation>Tilføj en række af sider</translation>
    </message>
    <message>
        <source>Consecutive Pages</source>
        <translation>Sammenhængende sider</translation>
    </message>
    <message>
        <source>From:</source>
        <translation>Fra:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Til:</translation>
    </message>
    <message>
        <source>Comma Separated List</source>
        <translation>Komma adskilt liste</translation>
    </message>
    <message>
        <source>Even Pages</source>
        <translation>Lige sider</translation>
    </message>
    <message>
        <source>Odd Pages</source>
        <translation>Ulige sider</translation>
    </message>
    <message>
        <source>&amp;Add To Range</source>
        <translation>&amp;Tilføj til række</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Range of Pages</source>
        <translation>Række af sider</translation>
    </message>
    <message>
        <source>Move &amp;Up</source>
        <translation>Flyt &amp;op</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Move &amp;Down</source>
        <translation>Flyt &amp;ned</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>S&amp;let</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Advanced Reordering</source>
        <translation>Avanceret omordning</translation>
    </message>
    <message>
        <source>Page Group Size:</source>
        <translation>Side-gruppe størrelse:</translation>
    </message>
    <message>
        <source>Sample Page Order:</source>
        <translation>Prøve-side orden:</translation>
    </message>
    <message>
        <source>Page Order</source>
        <translation>Side-orden</translation>
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
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+A</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>CSV import indstillinger</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Felt adskiller:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Værdi adskiller:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Første række er en overskrift</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Even Pages only</source>
        <translation>Kun lige sider</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Værdi</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Side pr ark</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskab</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Spejling</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Indstilling</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Retning</translation>
    </message>
    <message>
        <source>Printer Options</source>
        <translation>Printer instillinger</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Kun ulige sider</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portræt</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Udskriv</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Sider pr ark</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-UP udskrivning</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle sider</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Dette panel viser forskellige CUPS valgmuligheder for udskrivning. De precise parametre, som er tilgængelige afhænger af printerdriveren. Du kan se om CUPS er understøttet ved at vælge Hjælp &gt; Om Scribus. Se efter: C-C-T Disse står for C=CUPS C=littlecms T=TIFF understøttelse. Manglende biblioteksunderstøttelse er vist ved en *</translation>
    </message>
</context>
<context>
    <name>CurveWidget</name>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Curve Files (*.scu);;All Files (*)</source>
        <translation>Kurve Filer (*.scu);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Kan ikke skrive filen: 
%1</translation>
    </message>
    <message>
        <source>Inverts the curve</source>
        <translation>Vender kurven rundt</translation>
    </message>
    <message>
        <source>Resets the curve</source>
        <translation>Nulstiller kurven</translation>
    </message>
    <message>
        <source>Switches between linear and cubic interpolation of the curve</source>
        <translation>Skifter mellem lineær og  kubisk interpolation af kurven</translation>
    </message>
    <message>
        <source>Loads a curve</source>
        <translation>Indlæser en kurve</translation>
    </message>
    <message>
        <source>Saves this curve</source>
        <translation>Gemmer denne kurve</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Gå til dokument-mappe.
Dette kan indstilles i Præferencer.</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Tegnsæt:</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimer fil</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Medtag fonte</translation>
    </message>
    <message>
        <source>&amp;Include ICC Profiles</source>
        <translation>&amp;Medtag ICC-profiler</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Anulleret af bruger</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Slet farve</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Slet farve:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Erstat med:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>to:</source>
        <translation>til:</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation>Slet sider</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Slet fra:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Slet typografi</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Slet typografi:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Erstat med:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Import fejlede</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Import af word dokument fejlede
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokument information</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Forfatter:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Nøgleord:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Beskrivelse:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Forlægger:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Bidrag fra:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Da&amp;to:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ype:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>For&amp;mat:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Id&amp;entifikation:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Kildetekst:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprog:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relation:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>D&amp;ækning:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Rettig&amp;heder:</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Yderligere &amp;Information</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>En person eller organisation som er ansvarlig for at gøre dokumentet tilgængelig</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>En person eller organisation som er ansvarlig for at lave bidrag til dokumentets indhold</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>En dato forbundet med en begivenhed i dokumentets livscyklus, i ÅÅÅÅ-MM-DD format, som beskrevet i ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Dokumentets natur eller genre, f.eks. kategori, funktioner, genre, etc</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>En utvetydig reference til dokumentet inden for et givet sammenhæng som f.eks. ISBN eller URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>En reference til et relateret dokument, eventuelt med en formel identifikation så som en ISBN eller URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Dokumentets omfang eller rækkevidde, eventuelt også sted, tid og jurisdiktion</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Information om rettigheder indeholdt i og over dokumentet, f. eks. copyright, patent eller varemærke</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>En reference til et dokument fra hvilket det aktuelle dokument er udledt af, f.eks ISBN eller URI</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Personen eller organisationen hovedsalig ansvarlig for indholdet af dokumentet. Dette felt kan indlejres både i Scribus dokumentet for reference og i PDF som metadata</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>En navn på dokumentet. Dette felt kan indlejres både i Scribus dokumentet for reference og i PDF som metadata</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>En beskrivelse af dokumentets indhold. Dette felt er beregnet for en kort beskrivelse eller resumé af dokumentet. Det bliver indlejret i PDF ved export</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Emnet på dokumentets indhold. Dette felt er beregnet for dokument nøgleord, som du ønsker at indlejre i PDF, for at lette søgning og indeksering af PDF-filer</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Den fysiske eller digitale udgave af dokumentet. Medie type og format/størrelse vil være værd at nævne. RFC2045,RFC2046 for MIME typer er også brugbart her</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Sproget som indholdet af dokumentet er skrevet i, sædvanligvis en ISO-639 sprogkode eventuelt efterfulgt af en bindestreg og en ISO-3166 landekode, f.eks. en-GB, fr-CH</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Tilføj en side-nummererings sektion til dokumentet. Den nye sektion vil blive tilføjet efter den sektion, der lige nu er valgt.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Slet den valgte sektion.</translation>
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
        <translation>Sidenummer er uden for tilladt område</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Værdierne du har indtastet ligger uden for omfanget af sidenumre i det aktive dokument (%1-%2).</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation>&lt;b&gt;Navn:&lt;/b&gt; Valgfri navn for afsnit f.eks. Indeks&lt;br/&gt;&lt;b&gt;Vist:&lt;/b&gt; Vælg at vise sidenumre i dette afsnit hvis der er en eller flere tekstrammer sat til at gøre dette.&lt;br/&gt;&lt;b&gt;Fra:&lt;/b&gt; Sideindeks for dette afsnit starter ved.&lt;br/&gt;&lt;b&gt;Til:&lt;/b&gt; Sideindeks for dette afsnit stopper ved.&lt;br/&gt;&lt;b&gt;stil:&lt;/b&gt; Vælg sidenummer stilen som skal bruges.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; Indekset, inden for stilens område, starter ved. F.eks. hvis Start=2 og Stil=a,b,c, ..., vil numrene starte ved b. Dette erstatter, for det første afsnit i dokumentet, det gamle &apos;Første sidenummer&apos; i &apos;Ny fil vinduet&apos;.</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Dokument sektioner</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Fra</translation>
    </message>
    <message>
        <source>To</source>
        <translation>Til</translation>
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
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Vist</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Relaterer til</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Er forælder til</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Er barn af</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Tekstrammer</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Billedrammer</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Boolsk</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Heltal</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Streng</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Kommatal</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokument element kvaliteter</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Værdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Slægtskab</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Slægtskab med</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Auto tilføj til</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
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
        <translation>Alle</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Setup Printer</source>
        <translation>Printerindstilling</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
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
        <translation>Gul</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Sort</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Indsæt en liste af tegn adskilt med komma, hvor
et tegn kan være * for alle siderne, 1-5 for
en række sider eller et enkelt sidenummer.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Udskriv til</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Indstillinger...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fil:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Alternativ udskrivnings-k&amp;ommando</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Ko&amp;mmando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Omfang</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Udskriv a&amp;lle sider</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Udskriv &amp;denne side</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Udskriv d&amp;ette område</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Antal &amp;kopier:</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Udskriv</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Udskriv normalt</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Udskriv separationer</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Udskriv i farve hvis det er tilgængeligt</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Udskriv i gråtoner</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript niveau 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript niveau 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript niveau 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Spejl side(r) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Spejl side(r) vertikalt</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Angiv medie størrelse</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Anvend Reduktion af underliggende farve</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertér spot-farver til proces-farver</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Anvend ICC profiler</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Avancerede indstillinger</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Vis udskrift...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Sætter PostScript niveau.
Niveau 1 eller 2 kan give meget store filer</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript Filer (*.ps);;Alle Filer (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Brug en alternativ printer håndtering, som f.eks. kprinter eller gtklp, for at kunne få flere printerindstillinger</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En måde at ændre nogle af gråtonerne, som er sammensat af cyan, gul og magenta, så der i stedet for disse farver bruges sort. RUF påvirker mest de dele af billedet, som er neutrale og/eller mørke i farven, det, som er tættets på gråt. Brug af denne metode kan forbedre udskrivningen af nogle 
billeder og nogen eksperimenteren og testning er nødvendigt fra billede til billede. RUF formindsker muligheden for overmætning med CMY farverne.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Aktiverer konvertering af spot-farver til sammensatte farver. Medmindre du planlægger at trykke med spot-farver hos et kommercielt trykkeri, er det sansynligvis bedst at have aktiveret.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Tillader dig at indlejre ICC profiler i udskrift-data når farvestyring er aktiveret </translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Dette gør dig i stand til direkte at sætte medie størrelse i PostScript-filen. Anbefales ikke medmindre det kræves af dit trykkeri.</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Hente printer-indstillinger mislykkedes</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Afskær ved side margener</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Vis ikke objekter udenfor margener på den udskrevne side</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Gennemtving overprint tilstand</translation>
    </message>
    <message>
        <source>Printer Marks</source>
        <translation>Printermærker</translation>
    </message>
    <message>
        <source>Crop Marks</source>
        <translation>Skæremærker</translation>
    </message>
    <message>
        <source>Bleed Marks</source>
        <translation>Tryk til kant mærker</translation>
    </message>
    <message>
        <source>Registration Marks</source>
        <translation>Pasmærker</translation>
    </message>
    <message>
        <source>Color Bars</source>
        <translation>Farveklodser</translation>
    </message>
    <message>
        <source>Offset:</source>
        <translation>Forskydning:</translation>
    </message>
    <message>
        <source>Marks</source>
        <translation>Mærker</translation>
    </message>
    <message>
        <source>Bleed Settings</source>
        <translation>Tryk til kant indstillinger</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation>Top:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation>Bund:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation>Venstre:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Højre:</translation>
    </message>
    <message>
        <source>Use Document Bleeds</source>
        <translation>Brug dokument Tryk til kant</translation>
    </message>
    <message>
        <source>Bleeds</source>
        <translation>Tryk til kant</translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation>Inderside:</translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation>Yderside:</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Afstand for tryk til kant fra toppen af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Afstand for tryk til kant fra bunden af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Afstand for tryk til kant fra venstre af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Afstand for tryk til kant fra højre af den fysiske side</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Aktiver global overprint tilstand for dette dokument, tilsidesætter objektindstillinger</translation>
    </message>
    <message>
        <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing.</source>
        <translation>Dette laver skæremærker i PDF, som viser, hvor papiret skal skæres eller trimmes efter trykning.</translation>
    </message>
    <message>
        <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
        <translation>Dette laver tryk til kant mærker, som er vist med  _._ og viser hvormeget, der flyder ud over kant </translation>
    </message>
    <message>
        <source>Add registration marks which are added to each separation</source>
        <translation>Tilføj pasmærker, som tilføjes hver separation</translation>
    </message>
    <message>
        <source>Add color calibration bars</source>
        <translation>Tilføj farve-kalibrerings søjler</translation>
    </message>
    <message>
        <source>Indicate the distance offset for the registration marks</source>
        <translation>Indikerer afstands-forskydning af pasmærker</translation>
    </message>
    <message>
        <source>Use the existing bleed settings from the document preferences</source>
        <translation>Brug de eksisterende instillinger for tryk til kant fra dokumentets præferencer</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Import af fil:
%1
fejlede!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatal fejl</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fejl</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>Analyserer Postscript:</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Genererer elementer</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>Convertering af %1 billeder fejlede!</translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation>Lav til gruppe%1</translation>
    </message>
    <message>
        <source>Importing: %1</source>
        <translation>Importerer: %1</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Redigér typografi</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linie afstand</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Navn på  din afsnits-typografi</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Font for den markerede tekst eller objekt</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Font størrelse</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Tekstens fyldfarve</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Tekstens stregfarve</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Bestemmer højden af unicialen i antal linier</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Afstand over afsnit</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Afstand under afsnit</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorer og indrykning</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linier:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Afstande</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast linieafstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatisk linieafstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Ret ind efter basislinie-gitter</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Uncialer</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Afstand fra tekst:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Vis afsnits typografi</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Bestemmer afstanden mellem uncialerne og teksten</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Slår prøvetekst for denne afsnits typografi til/fra</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Navnet på typografien er ikke entydig</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Baggrund</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuel spatiering</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Forskydning til tegnenes basislinie</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation>Tryk for at vælge linieafstandens type</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Select for easier reading of light colored text styles</source>
        <translation>Vælg for lettere læsning af let farvede typografier</translation>
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
        <translation>&amp;Nyt</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åbn...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gem &amp;som...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Gem og afslut</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Afslut uden at gemme</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Fortryd</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Lav igen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Hent feltnavne</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;Alle filer (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Billed effekter</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Indstillinger:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farve:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Farvemætning:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Lysstyrke:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrast:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Radius:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Værdi:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posterisér:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Tilgængelige effekter</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Blødgør</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Lysstyrke</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Kulørisér</translation>
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
        <translation>Invertér</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posterisér</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Skærp</translation>
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
        <translation>Effekter i brug</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Color 1:</source>
        <translation>Farve 1:</translation>
    </message>
    <message>
        <source>Color 2:</source>
        <translation>Farve 2:</translation>
    </message>
    <message>
        <source>Color 3:</source>
        <translation>Farve 3:</translation>
    </message>
    <message>
        <source>Color 4:</source>
        <translation>Farve 4:</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation>Duotone</translation>
    </message>
    <message>
        <source>Tritone</source>
        <translation>Tritone</translation>
    </message>
    <message>
        <source>Quadtone</source>
        <translation>uadtone</translation>
    </message>
    <message>
        <source>Curves</source>
        <translation>Kurver</translation>
    </message>
</context>
<context>
    <name>ExportBitmap</name>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Filen eksisterer. Overskriv?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>eksisterer allerede. Overskriv?</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>A&amp;lle sider</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Vælg mappen for uddata</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Tilgængelige eksport-formater</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Vælg en eksportmappe</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Uddata-mappen - stedet hvor dine billeder gemmes.
Navnet på den eksporterede fil vil være &apos;dokumentnavn-sidenummer.filtype&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Eksportér kun den aktuelle side</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Eksportér til mappe:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Billed&amp;type:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Eksportér som billede(r)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Opløs&amp;ning:</translation>
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
        <translation>Omfang</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>A&amp;ktuelle side</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Omfang</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Indsæt en liste af tegn adskilt med komma, hvor
et tegn kan være * for alle siderne, 1-5 for
en række sider eller et enkelt sidenummer.</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Billedernes opløsning
Brug 72 dpi for billeder, der er beregnet til skærmen</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Kvaliteten på dine billeder - 100% er bedst, 1% den laveste kvalitet</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Eksportér nogle af siderne</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Eksportér alle sider</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Billedernes størrelse. 100% for ingen ændringer. 200% for to gange større etc.</translation>
    </message>
    <message>
        <source>Image size in Pixels</source>
        <translation>Billedstørrelse i pixler</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Udvidede billed egenskaber</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Gør mørkere</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Gør lysere</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Farvetone</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Lysstyrke</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Gang</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Skærm</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Opløs</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Læg over</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hårdt lys</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Blødt lys</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Forskel</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Udelukkelse</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Farve blegning</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Farve sværtning</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blandings tilstand:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Ugennemsigtighed:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Brug ikke nogen sti</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Stier</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Størrelse:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Ingen titel</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titel:</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Forfatter:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukendt</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Opløsning:</translation>
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
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Farverum:</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation>Duotone</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Nogle fonte brugt af dette dokument er blevet erstattet:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> er erstattet med: </translation>
    </message>
</context>
<context>
    <name>FontComboH</name>
    <message>
        <source>Face:</source>
        <translation>Fontnavn:</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Font Name</source>
        <translation>Font navn</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Erstatnings fonte</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Erstatning</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Ekstra stier</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Tilgængelige Fonte</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Tilgængelige fonte</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Erstatninger af fonte</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Ekstra &amp;Stier</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>Ti&amp;lføj...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Fjern</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Font navn</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Brug font</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Substituere</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Sti til fontfil</translation>
    </message>
    <message>
        <source>Embed in PostScript</source>
        <comment>font preview</comment>
        <translation>Indlejr i PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in File &gt; Preferences, and only when there is no document currently open. Close any open documents, then use File &gt; Preferences &gt; Fonts to change the font search path.</source>
        <translation>Font søgestier kan kun sættes i Fil &gt; Præferencer, og kun når der ikke er åbnet nogen dokumenter. Luk alle dokumenter, og brug så Fil &gt; Præferencer for at ændre font søgestien.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Tilføj den markerede font til stil, Font menu</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Afslut visning</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Start søgning</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Størrelse på valgte font</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Vævet silkepyjamas byttet med blå kvarts</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Bruger</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>System</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Indtastning af tekst her giver en hurtig søgning i fontnavne. Søgningen skelner ikke mellem store/små bogstaver. Du kan angive almindelige erstatningstegn (*,?,[...]) i dit søgeord. Eksempler: t* viser alle fonte, der starter med t eller T.  *bold* viser alle fonte med ordet bold, bolder etc. i navnet.  </translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Prøve vil blive vist når tasten slippes</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Font visning</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Hurtig søgning:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søg</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Font navn</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Dok</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Substituere</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Adgang</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Fontstørrelse:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Prøvetekst der skal vises</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>&amp;Vis</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Nulstil teksten</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
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
        <translation>&amp;Font forhåndsvisning...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Font forhåndsvisning</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Sortering, søgning og gennemsyn af tilgængelige fonte.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Erstatnings font</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Oprindelig font</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Erstatnings font</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Gør disse ændringer permanent</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Dette dokument indeholder nogle fonte, der ikke er installeret på dit system, vælg venligst en egnet erstatning for dem. Annullér vil stoppe indlæsning af dokumentet.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Annullerer disse font-erstatninger og stopper indlæsning af dokumentet.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Aktivering af dette får Scribus til at bruge disse erstatninger for manglende fonte permanent i alle fremtidige layouts. Det kan blive ført tilbage eller ændres i Redigér &gt; præferencer &gt; fonte.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Hvis du vælger OK, og derefter gem, så er disse erstatninger gjort permanent i dokumentet.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Placering:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Tilføje, ændr eller fjern farveskift her</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Edit Guide</source>
        <translation>Redigér hjælpelinie</translation>
    </message>
    <message>
        <source>Enter a position:</source>
        <translation>Indtast en placering:</translation>
    </message>
    <message>
        <source>New Guide</source>
        <translation>Ny hjælpelinie</translation>
    </message>
</context>
<context>
    <name>GuideManagerBase</name>
    <message>
        <source>Manage Guides</source>
        <translation>Håndtér hjælpelinier</translation>
    </message>
    <message>
        <source>&amp;Single</source>
        <translation>&amp;Enkel</translation>
    </message>
    <message>
        <source>Horizontals</source>
        <translation>Horisontale</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hjælpelinie</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>Sl&amp;et</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Verticals</source>
        <translation>Vertikale</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Til&amp;føj</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+F</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>S&amp;let</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Lås hjælpelinier</translation>
    </message>
    <message>
        <source>Appl&amp;y to All Pages</source>
        <translation>Anvend effekt p&amp;å alle sider</translation>
    </message>
    <message>
        <source>Alt+Y</source>
        <translation>Alt+Å</translation>
    </message>
    <message>
        <source>&amp;Column/Row</source>
        <translation>&amp;Kolonne/Række</translation>
    </message>
    <message>
        <source>&amp;Number:</source>
        <translation>&amp;Antal:</translation>
    </message>
    <message>
        <source>U&amp;se Gap:</source>
        <translation>B&amp;rug afstand:</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>Refer To</source>
        <translation>I forhold til</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Si&amp;de</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>M&amp;argins</source>
        <translation>M&amp;argener</translation>
    </message>
    <message>
        <source>S&amp;election</source>
        <translation>Mark&amp;ering</translation>
    </message>
    <message>
        <source>Nu&amp;mber:</source>
        <translation>An&amp;tal:</translation>
    </message>
    <message>
        <source>Use &amp;Gap:</source>
        <translation>Brug &amp;afstand:</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Misc</source>
        <translation>&amp;Diverse</translation>
    </message>
    <message>
        <source>Delete all guides from the current page</source>
        <translation>Slet alle hjælpelinier fra den aktive side</translation>
    </message>
    <message>
        <source>Delete all guides from the current document</source>
        <translation>Slet alle hjælpelinier fra det aktive dokument</translation>
    </message>
    <message>
        <source>Delete Guides from Current &amp;Page</source>
        <translation>Slet hjælpelinier fra den aktive &amp;side</translation>
    </message>
    <message>
        <source>Delete Guides from &amp;All Pages</source>
        <translation>Slet hjælpelinier fra &amp;alle sider</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Desværre er der ingen manual tilgængelig! Se venligst http://docs.scribus.net for nyeste dokumentation
og www.scribus.net for at hente scribus ect.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Indhold</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lænke</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus hjælp</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Indhold</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søg</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Find</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Søgeord:</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>S&amp;øg</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nyt</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>S&amp;let alt</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>Bog&amp;mærker</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Udskriv...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Afslut</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Søgning er ufølsom for brug af store/små bogstaver</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nyt bogmærke</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Titel på nyt bogmærke:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Søg...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Søg &amp;Næste</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Søg &amp;Foregående</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Tilføj bogmærke</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>Slet &amp;Alle</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bogmærker</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Relevans</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Skip</source>
        <translation>Spring over</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Acceptér</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Mulig orddeling</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Længden af det mindste ord der deles.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maksimale antal orddelinger, der følger lige efter hinanden.
Værdien 0 betyder ubegrænset antal.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprog:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Mindste ord:</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Orddel tekst &amp;automatisk under indtastning</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Et dialogvindue, der viser de mulige orddelinger for hvert ord, kommer frem når du bruger de ekstra orddelings tekst tilvalg.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Aktiverer automatisk orddeling af din tekst under indtastningen.</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Orddelings &amp;forslag</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Antal &amp;tilladte orddelinger i træk:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Billed Info</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Generel Info</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Dato / Tid:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Har indlejret profil:</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Profil Navn:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Har indlejret sti:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Har lag:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF Info</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Kunstner:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Kommentar:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Bruger kommentar:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kamera Model:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Kamera producent:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beskrivelse:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Skanner model:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Skanner producent:</translation>
    </message>
    <message>
        <source>Exposure time</source>
        <translation>Eksponeringstid</translation>
    </message>
    <message>
        <source>Aperture:</source>
        <translation>blændeåbning:</translation>
    </message>
    <message>
        <source>ISO equiv.:</source>
        <translation>ISO værdi:</translation>
    </message>
</context>
<context>
    <name>ImportDialog</name>
    <message>
        <source>Choose Styles</source>
        <translation>Vælg typografier</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Tilgængelige typografier</translation>
    </message>
    <message>
        <source>Character Styles</source>
        <translation>Skrifttegn-stile</translation>
    </message>
    <message>
        <source>Paragraph Styles</source>
        <translation>Afsnits typografier</translation>
    </message>
    <message>
        <source>Line Styles</source>
        <translation>Linie stile</translation>
    </message>
    <message>
        <source>In case of a name clash</source>
        <translation>I tilfælde af navne sammenfald</translation>
    </message>
    <message>
        <source>Rename imported style</source>
        <translation>Omdøb importerede typografi</translation>
    </message>
    <message>
        <source>Replace existing style</source>
        <translation>Erstat eksisterende typografi</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importér &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importerer EPS filer</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importerer de fleste EPS filer ind i det aktive dokument,
og konverterer deres vektordata til Scribus objekter.</translation>
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
        <source>at End</source>
        <translation>i slutningen</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>Insert Page</source>
        <translation>Indsæt side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>Efter side</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Side(r)</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Master side:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portræt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskab</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Højde:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Flyt objekter med deres side</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Master sider</translation>
    </message>
</context>
<context>
    <name>InsertAFrame</name>
    <message>
        <source>&lt;b&gt;Insert a text frame&lt;/b&gt;&lt;br/&gt;A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable format from plain text to OpenOffice.org.&lt;br/&gt;Your text may be edited and formatted on the page directly or in the simple Story Editor.</source>
        <translation>&lt;b&gt;Indsæt en tekst-ramme&lt;/b&gt;&lt;br/&gt;En tekst-ramme tillader dig at placere en hvilken som helst tekst i et afgrænset område med den formatering du vælger. Du kan vælge en tekst-fil fra tilvalgs fanebladet, hvis du med det samme ønsker at importere et dokument ind i rammen. Scribus understøtter mange forskellige importerbare formater fra almindelig tekst til OpenOffice.org.&lt;br/&gt;Din tekst kan redigeres og formateres direkte på siden eller i den enkle indbyggede tekstbehandler.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Insert an image frame&lt;/b&gt;&lt;br/&gt;An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.</source>
        <translation>&lt;b&gt;Indsæt en billed-ramme&lt;/b&gt;&lt;br/&gt;En billed-ramme tillader dig at placere et billede på din side. Forskellige billed-effekter kan anvendes eller kombiners inklusive transparens, lysstyrke og posterisation, som tillader retouchering eller fremstilling af interessante visuelle resultater. Billed skalering og formning udføres med egenskabs paletten.</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
</context>
<context>
    <name>InsertAFrameBase</name>
    <message>
        <source>Insert A Frame</source>
        <translation>Indsæt en ramme</translation>
    </message>
    <message>
        <source>T&amp;ype</source>
        <translation>T&amp;ype</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Tekstramme</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>B&amp;illed ramme</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abel</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Shape</source>
        <translation>Figur</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>&amp;Location</source>
        <translation>&amp;Sted</translation>
    </message>
    <message>
        <source>Page Placement</source>
        <translation>Side placering</translation>
    </message>
    <message>
        <source>Current Page</source>
        <translation>Denne side</translation>
    </message>
    <message>
        <source>Selecting this will place the frame only on the current page.</source>
        <translation>Ved at vælge dette, placeres rammen kun på den aktive side.</translation>
    </message>
    <message>
        <source>Range of Pages:</source>
        <translation>Område af sider:</translation>
    </message>
    <message>
        <source>Selecting this will place frame on the selected range. </source>
        <translation>Ved at vælge dette, placeres rammen på det valgte område. </translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Position of Frame</source>
        <translation>Placering af ramme</translation>
    </message>
    <message>
        <source>Top Left of Margins</source>
        <translation>Top Venstre af margener</translation>
    </message>
    <message>
        <source>Selecting this places the frame in the upper left of the page margins defined in your doc setup.</source>
        <translation>Ved at vælge dette, placeres rammen øverst til venstre mod side-margener defineret i din dok opsætning.</translation>
    </message>
    <message>
        <source>Top Left of Page</source>
        <translation>Top Venstre af side</translation>
    </message>
    <message>
        <source>Selecting this puts the frame on the top left with postion 0,0</source>
        <translation>Ved at vælge dette, placeres rammen øverst til venstre ved position 0,0</translation>
    </message>
    <message>
        <source>Custom Position:</source>
        <translation>Bruger defineret:</translation>
    </message>
    <message>
        <source>Set the dimensions wished below in the X: Y: dialog below.</source>
        <translation>Angiv placering herunder i X: Y:  vinduerne.</translation>
    </message>
    <message>
        <source>X:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>Same as the Page Margins</source>
        <translation>Det samme som sidemargener</translation>
    </message>
    <message>
        <source>Same as the Page</source>
        <translation>Det samme som siden</translation>
    </message>
    <message>
        <source>Custom Size:</source>
        <translation>Bruger defineret størrelse:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Højde:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>&amp;Options</source>
        <translation>&amp;Tilvalg</translation>
    </message>
    <message>
        <source>Source Image:</source>
        <translation>Kilde billede:</translation>
    </message>
    <message>
        <source>&amp;Select File...</source>
        <translation>&amp;Vælg fil...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>There are no options for this type of frame</source>
        <translation>Der er ikke nogen tilvalg for denne type ramme</translation>
    </message>
    <message>
        <source>Source Document:</source>
        <translation>Kilde dokument:</translation>
    </message>
    <message>
        <source>Columns:</source>
        <translation>Kolonner:</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation>Afstand:</translation>
    </message>
    <message>
        <source>Link Created Frames</source>
        <translation>Sammenkæd de nye rammer</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle sider</translation>
    </message>
    <message>
        <source>Top Left of Bleed</source>
        <translation>Top venstre af Tryk til kant</translation>
    </message>
    <message>
        <source>Selecting this places the frame in the upper left of the page bleed defined in your doc setup.</source>
        <translation>Ved at vælge dette, placeres rammen øverst til venstre i Tryk til kant området, som er defineret i din dok opsætning.</translation>
    </message>
    <message>
        <source>Same as the Bleed</source>
        <translation>Det samme som tryk til kant området</translation>
    </message>
    <message>
        <source>Same as the Imported Image</source>
        <translation>Det samme som det importerede billede</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Indsæt tabel</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Antal rækker:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Antal kolonner:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Ny script</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Redigér JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Redigér...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Tilføj...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Ny script:</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Ønsker du virkelig at slette denne script?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Tilføjer en ny Script, predefinerer en funktion med det samme navn. Hvis du ønsker at bruge denne script som en &quot;åben handling&quot;, så vær sikker på at du ikke ændrer funktionens navn.</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Slet lag</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Ønsker du også at slette alle objekter i dette lag?</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Tilføj et nyt lag</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Slet lag</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Hæv lag</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Sænk lag</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Ugennemsigtighed:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blandings tilstand:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Gør mørkere</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Gør lysere</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Gang</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Skærm</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Læg over</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hårdt lys</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Blødt lys</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Forskel</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Udelukkelse</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Farve blegning</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Farve sværtning</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Farvetone</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Lysstyrke</translation>
    </message>
    <message>
        <source>Duplicates the current layer</source>
        <translation>Duplikerer det aktive lag</translation>
    </message>
    <message>
        <source>Color of the Layer Indicator - Each layer has a color assigned to display on the canvas when layer indicators are enabled. You can double click to edit the color. </source>
        <translation>Farve på lag-indikatoren - hvert lag har en bestemt farve som vises på dokument-området når lag-indikatoren er aktiveret. Du kan dobbeltklikke for at ændre farven. </translation>
    </message>
    <message>
        <source>Make Layer Visible - Uncheck to hide the layer from the display </source>
        <translation>Gør lag synligt - Fjern afkrysning for at skjule laget </translation>
    </message>
    <message>
        <source>Print Layer - Uncheck to disable printing. </source>
        <translation>Udskriv lag - Fjern afkrysning for at deaktivere udskrivning. </translation>
    </message>
    <message>
        <source>Lock or Unlock Layer - Unchecked is unlocked </source>
        <translation>Lås eller frigør lag - Fjern afkrysning for at frigøre </translation>
    </message>
    <message>
        <source>Text flows around objects in lower Layers - Enabling this forces text frames to flow around other objects, even in layers below</source>
        <translation>Tekst flyder uden om objekter i bagved liggende lag - Aktivering af dette tvinger tekst-rammer til at flyde rundt om andre objekter, selv i lag bagved dette</translation>
    </message>
    <message>
        <source>Outline Mode - Toggles the &apos;wireframe&apos; display of objects to speed the display of very complex objects.</source>
        <translation>Omrids tilstand - Slar &apos;trådramme&apos; visning af objekter til/fra for hurtigere visning af meget komplekse objekter.</translation>
    </message>
    <message>
        <source>Name of the Layer - Double clicking on the name of a layer enabled editing</source>
        <translation>Lagets navn - Dobbeltklik på lagets navnet for at aktivere redigering</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Edit Line Styles</source>
        <translation>Redigér liniestile</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi af %1</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikér</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Ønsker du virkelig at slette denne typografi?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>LineStyleWBase</source>
        <translation>LinieStilWBase</translation>
    </message>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Liniebredde:</translation>
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
        <translation>Flade</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkantede</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spidse</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Afskårne</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Add a new line</source>
        <translation>Tilføj en ny linie</translation>
    </message>
    <message>
        <source>Remove a line</source>
        <translation>Fjern en linie</translation>
    </message>
    <message>
        <source>Line style</source>
        <translation>Liniestil</translation>
    </message>
    <message>
        <source>Line width</source>
        <translation>Liniebredde</translation>
    </message>
    <message>
        <source>End style</source>
        <translation>Ende type</translation>
    </message>
    <message>
        <source>Join style</source>
        <translation>Sammenføjnings type</translation>
    </message>
    <message>
        <source>Line color</source>
        <translation>Liniefarve</translation>
    </message>
    <message>
        <source>Line shade</source>
        <translation>Linieskygge</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filer (*)</translation>
    </message>
    <message>
        <source>No File Loader Plugins Found</source>
        <translation>Ingen fil-indlæser plugins fundet</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Vælg Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Forfatter:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Hent mere:</translation>
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
        <translation>Afsnit: </translation>
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
        <source>Standard Lorem Ipsum</source>
        <translation>Standard Lorem Ipsum</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Manage Page Properties</source>
        <translation>Håndtér side egenskaber</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portræt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskab</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Højde:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Flyt objekter med deres side</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margen hjælpelinier</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <source>Other Settings</source>
        <translation>Andre indstillinger</translation>
    </message>
    <message>
        <source>Master Page:</source>
        <translation>Master side:</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bund:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Højre:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Afstand mellem top margen-hjælpelinie og sidens kant</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Afstand mellem bund margen-hjælpelinie og sidens kant</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Inderside:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Yderside:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Forvalgte layouts:</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Anvend margen ændringerne på alle eksisterende sider i dokumentet</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Printer margener...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Importér margener for den valgte sidestørrelse fra de tilgængelige printere.</translation>
    </message>
    <message>
        <source>Apply settings to:</source>
        <translation>Anvend indstillinger på:</translation>
    </message>
    <message>
        <source>All Document Pages</source>
        <translation>Alle dokument sider</translation>
    </message>
    <message>
        <source>All Master Pages</source>
        <translation>Alle master sider</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing master pages in the document</source>
        <translation>Anvend margen ændringer på alle eksisterende mastersider i dokumentet</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margen hjælpelinier</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation>Top:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation>Bund:</translation>
    </message>
    <message>
        <source>Bleeds</source>
        <translation>Tryk til kant</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Afstand for tryk til kant fra toppen af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Afstand for tryk til kant fra bunden af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Afstand for tryk til kant fra venstre af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Afstand for tryk til kant fra højre af den fysiske side</translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation>Inderside:</translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation>Yderside:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation>Venstre:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Højre:</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem venstre margen-hjælpelinie og sidens kant. Hvis der er valgt dobbelt-sidet, 3 eller 4-fold layout, så kan denne margen bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If a double-sided, 3 or 4-fold layout is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem højre margen-hjælpelinie og sidens kant. Hvis der er valgt dobbelt-sidet, 3 eller 4-fold layout, så kan denne margen bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Redigér Master sider</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Ønsker du virkelig at slette denne masterside?</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Ny masterside</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Ny masterside</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Duplikér den valgte masterside</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Slet den valgte masterside</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Tilføj en ny masterside</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Importér mastersider fra et andet dokument</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Ny masterside %1</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation>Ikke i stand til at omdøbe masterside</translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation>Det er ikke tilladt at omdøbe normalsiden.</translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation>Omdøb masterside</translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation>Nyt navn:</translation>
    </message>
    <message>
        <source>Copy #%1 of %2</source>
        <translation>Kopi #%1 af %2</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplikér flere gange</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>Antal &amp;kopier:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horisontalt skift:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikalt skift:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Afstande</translation>
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
        <translation>Længde:</translation>
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
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importér side(r)</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> fra 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Lav side(r)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> fra %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Importér masterside</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Fra dokument:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importér side(r):</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Importér masterside</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Indsæt en liste af tegn adskilt med komma, hvor et tegn kan være * for alle siderne, 1-5 for en række sider eller et enkelt sidenummer.</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Efter side</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>I slutningen</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Manglende font</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Fonten %1 er ikke installeret.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Brug</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>i stedet</translation>
    </message>
</context>
<context>
    <name>ModeToolBar</name>
    <message>
        <source>Tools</source>
        <translation>Værktøjer</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Egenskaber...</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Page(s):</source>
        <translation>Flyt side(r):</translation>
    </message>
    <message>
        <source>Move Pages</source>
        <translation>Flyt sider</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopiér side</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Flyt side(r)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Efter side</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>I slutningen</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Til:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Antal kopier:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flade</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Afstand fra kurve:</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Farvetilpasningsmetode:</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Font størrelse</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Endepunkter</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Form:</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Venstre punkt</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkantede</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Vis kurve</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Opfattelsesmæssig</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Afskårne</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spidse</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometri</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Omdr. punkt:</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Indlæs profil:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Tekstafstand</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linieafstand</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Startforskydning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolorimetri</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolorimetri</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Det valgte objekts navn</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horisontale placering af omdrejningspunkt</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikale placering af omdrejningspunkt</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Bredde</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Højde</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotation af objekt</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Referencepunkt for mål og rotation </translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Vælg top-venstre som omdrejningspunkt</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Vælg top-højre som omdrejningspunkt</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Vælg bund-venstre som omdrejningspunkt</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Vælg bund-højre som omdrejningspunkt</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Vælg center som omdrejningspunkt</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Flip horisontalt</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Flip vertikalt</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Flyt et niveau op</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Flyt et niveau ned</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Placér forrest</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Placér bagerst</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Lås eller frigør objektet</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Lås eller frigør objektets størrelse</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Muliggør eller forhindre udskrivning af objektet</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Den font der er valgt til den markerede tekst eller objekt</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skrifttegnenes skalerings-bredde</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Mætningsgrad af tekstens stregfarve</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Mætningsgrad af tekstens fyldfarve</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Indstillinger for venstre punkt eller endepunkter</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Liniemønster</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Linietykkelse</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Typen af liniesamlinger</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Typen af linie-enden</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Liniestil af valgte objekt</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Vælg rammens form...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Rediger rammens form...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Indstil radius for runde hjørner</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Antal kolonner i tekstramme</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Afstand mellem kolonner</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Afstand af tekst fra rammens top</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Afstand af tekst fra rammens bund</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Afstand af tekst fra rammens venstre side</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Afstand af tekst fra rammens højre side</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Rediger tabulatorer for tekstramme...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Tillad billedet at have en anden størrelse en rammen</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Horisontal forskydning af billedet inden i rammen</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Vertikal forskydning af billedet inden i rammen</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Ændre billedets størrelse horisontalt</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Ændre billedets størrelse vertikalt</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Behold forholdet mellem X og Y skalering </translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Tilpas billedet til rammen størrelse</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Brug billedets højde og bredde i stedet for rammens</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Cellelinier</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Linie for oven</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linie i venstre side</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Linie i højre side</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Linie i bunden</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Bevar bredde/højde forholdet</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Billedets kildeprofil</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Farvetilpasningsmetode for billedet</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Sti-tekst-egenskaber</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Viser det niveau hvor objektet er placeret, 0 betyder objektet er nederst</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Skifter mellem afstand og kolonne-bredde</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Kolonnebredde</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>For&amp;m</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Billede</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linie</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Farver</translation>
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
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Højde:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Brug &amp;indbindingsboks</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Brug &amp;omrids linie</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>Re&amp;digér form...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>R&amp;unde
hjørner:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>To&amp;p:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bund:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Højre:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>T&amp;abulatorer...</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Fri skalering</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-sk&amp;alering:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-skal&amp;ering:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Skalér &amp;til rammestørrelse</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roportionel</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Omdr. punkt:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>T&amp;ypen af linie:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linie&amp;bredde:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Hjørner:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Ender:</translation>
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
        <source>Right to Left Writing</source>
        <translation>Skrivning fra højre til venstre</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuel spatiering</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast linieafstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatisk linieafstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Ret ind efter basislinie-gitter</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Faktiske X-DPI:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Faktiske Y-DPI:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>start pil:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Slut pil:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Forskydning til tegnenes basislinie</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Skrifttegnenes skalerings-højde</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.&lt;br/&gt;Vælg venligst et andet.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Fyld stil</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Lige-Ulige</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Ikke Nul</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Farve på tekstens streg og/eller skygge, afhængig af det valgte. hvis begge er valgt, deler de den samme farve.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Farve på markeret tekst. Hvis &quot;omrids&quot; tekst-udsmykning er aktiveret, vil denne farve være fyldfarven. Hvis tekst-skygge er aktiveret, så vil dette være den øverste farve.</translation>
    </message>
    <message>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Brug indbindingsboksen, som altid er rektangulær, i stedet for rammens form for tekst-flydning af tekstrammer bagved objektet. </translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation>Indstillinger for gennemsigtighed</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Lav til &amp;gruppe</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Ugennemsigtighed:</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blandings tilstand:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Gør mørkere</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Gør lysere</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Gang</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Skærm</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Læg over</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hårdt lys</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Blødt lys</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Forskel</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Udelukkelse</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Farve blegning</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Farve sværtning</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Farvetone</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation>Afstand:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>Text &amp;Flow Around Frame</source>
        <translation>Tekst &amp;flyder uden om ramme</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Ikke aktiv</translation>
    </message>
    <message>
        <source>Use Frame &amp;Shape</source>
        <translation>Brug ramme &amp;form</translation>
    </message>
    <message>
        <source>Image Effects</source>
        <translation>Billed effekter</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Udvidede billed egenskaber</translation>
    </message>
    <message>
        <source>Overprinting</source>
        <translation>Overprintning</translation>
    </message>
    <message>
        <source>Knockout</source>
        <translation>Udskæring</translation>
    </message>
    <message>
        <source>Overprint</source>
        <translation>Overprint</translation>
    </message>
    <message>
        <source>Group the selected objects</source>
        <translation>Lav gruppe af de valgte objekter</translation>
    </message>
    <message>
        <source>Destroys the selected group</source>
        <translation>Ødelæg den valgte gruppe</translation>
    </message>
    <message>
        <source>Disable text flow from lower frames around object</source>
        <translation>Deaktivér tekst-flydning fra bagved liggende rammer omkring objekt</translation>
    </message>
    <message>
        <source>Use the frame shape for text flow of text frames below the object.</source>
        <translation>Brug rammens form for tekst-flydning af tekstrammer bagved objektet.</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Tryk og hold nede for at vælge linieafstands tilstand.</translation>
    </message>
    <message>
        <source>When chosen, the contour line can be edited with the Edit Shape Tool on the palette further above. When edited via the shape palette, this becomes a second separate line originally based on the frame&apos;s shape for text flow of text frames below the object. T</source>
        <translation>Når dette er valgt, kan omridslinien redigeres med rediger form værktøjet på paletten længere oppe. Når linien er redigeret via formpaletten, så bliver det en anden separat linie originalt baseret på rammens form for flydning af tekst under objektet. T</translation>
    </message>
    <message>
        <source>Default</source>
        <translation>Standard</translation>
    </message>
    <message>
        <source>Stair Step</source>
        <translation>Trappetrin</translation>
    </message>
    <message>
        <source>Skew</source>
        <translation>Vrid</translation>
    </message>
    <message>
        <source>Flip Text</source>
        <translation>Flip tekst</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <source>Use Image Clip Path</source>
        <translation>Brug billed klippe-sti</translation>
    </message>
    <message>
        <source>Paragraph St&amp;yle:</source>
        <translation>Afsnits t&amp;ypografi:</translation>
    </message>
    <message>
        <source>Character St&amp;yle:</source>
        <translation>Skrifttegn-s&amp;til:</translation>
    </message>
    <message>
        <source>Optical Margins:</source>
        <translation>Optiske margener:</translation>
    </message>
    <message>
        <source>Word Tracking</source>
        <translation>Ord spatiering </translation>
    </message>
    <message>
        <source>Min:</source>
        <translation>Min:</translation>
    </message>
    <message>
        <source>Norm:</source>
        <translation>Norm:</translation>
    </message>
    <message>
        <source>Glyph Extension</source>
        <translation>Glyf udvidelse</translation>
    </message>
    <message>
        <source>Max:</source>
        <translation>Maks:</translation>
    </message>
    <message>
        <source>Use the clipping path of the image</source>
        <translation>Brug billedets klipnings-sti</translation>
    </message>
    <message>
        <source>Paragraph style of currently selected text or paragraph</source>
        <translation>Afsnits typografi fra markerede tekst eller afsnit</translation>
    </message>
    <message>
        <source>Character style of currently selected text or paragraph</source>
        <translation>Tegn-stil fra markerede tekst eller afsnit</translation>
    </message>
    <message>
        <source>Minimal width of spaces between words</source>
        <translation>Minimal bredde af mellemrum mellem ord</translation>
    </message>
    <message>
        <source>Normal width of spaces between words</source>
        <translation>Normal bredde af mellemrum mellem ord</translation>
    </message>
    <message>
        <source>Minimal shrinkage of glyphs for justification</source>
        <translation>Minimal reducering af Glyffer for justering</translation>
    </message>
    <message>
        <source>Maximal extension of glyphs for justification</source>
        <translation>Maksimal udvidelse af Glyffer for justering</translation>
    </message>
    <message>
        <source>Uses hanging punctuation and margin kerning to achieve nicer looking columns</source>
        <translation>Bruger hængende tegnsætning og margen knibning, for at opnå et pænere udseende af kolonner</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Flat Cap</source>
        <translation>Flade</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Liniebredde:</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Redigér Stil</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkantede</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Afskårne</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spidse</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Runde</translation>
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
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.&lt;br/&gt;Vælg venligst et andet.</translation>
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
        <translation>Fremgang</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Total fremgang:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
</context>
<context>
    <name>MultipleDuplicate</name>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horisontalt skift:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikalt skift:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Gap:</source>
        <translation>&amp;Horisontal afstand:</translation>
    </message>
    <message>
        <source>&amp;Vertical Gap:</source>
        <translation>&amp;Vertikal afstand:</translation>
    </message>
</context>
<context>
    <name>MultipleDuplicateBase</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplikér flere gange</translation>
    </message>
    <message>
        <source>&amp;By Number of Copies</source>
        <translation>&amp;Udfra Antal &amp;kopier</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>Antal &amp;kopier:</translation>
    </message>
    <message>
        <source>&amp;Shift Created Items By</source>
        <translation>&amp;Forskyd de nye elementer med</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+F</translation>
    </message>
    <message>
        <source>Create &amp;Gap Between Items Of</source>
        <translation>Lav &amp;afstand mellem elementer på</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horisontalt skift:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikalt skift:</translation>
    </message>
    <message>
        <source>By &amp;Rows &amp;&amp; Columns</source>
        <translation>Udfra &amp;rækker &amp;&amp; kolonner</translation>
    </message>
    <message>
        <source>Vertical Gap:</source>
        <translation>Vertikal afstand:</translation>
    </message>
    <message>
        <source>Horizontal Gap:</source>
        <translation>Horisontal afstand:</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Antal rækker:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Antal kolonner:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>Rotation:</source>
        <translation>Rotation:</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Mit &amp;Udvidelsesmodul</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Mit Udvidelsesmodul</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Udvidelsesmodulet virkede!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Nyt dokument</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskab</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portræt</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margen hjælpelinier</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Dokumentets papirstørrelse, enten standard- eller brugerdefineret størrelse</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Retning på dokumentets sider</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Bredde af dokumentets sider, kan ændres hvis du har valgt en brugerdefineret størrelse</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Højde af dokumentets sider, kan ændres hvis du har valgt en brugerdefineret størrelse</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard målenhed for dokument-redigering</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Lav tekstrammer automatisk, når der tilføjes nye sider</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Antal kolonner i tekstrammer, der laves automatisk</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Afstand mellem kolonner, der laves automatisk</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Højde:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Auto&amp;matiske tekstrammer</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Afstan&amp;d:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Standard &amp;enhed:</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Vis ikke dette vindue igen</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>De indledende antal sider i dokumentet</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Antal si&amp;der:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nyt dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Åbn &amp;eksisterende dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Åbn nyere &amp;dokument</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Første side er:</translation>
    </message>
    <message>
        <source>Show Document Settings After Creation</source>
        <translation>Vis dokument opsætning efter det er lavet</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Dokument layout</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Nyt &amp;fra skabelon...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Indlæs dokumenter med predefinerede layout</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Start et dokument fra en skabelon lavet af andre brugere eller dig selv (f.eks. for dokumenter der har et konstant udseende).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Noder</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Nulstil dette kontrolpunkt</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Slet noder</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Tilføj noder</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Flyt noder</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Flyt kontrolpunkter</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Nulstil kontrolpunkter</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>A&amp;bsolutte coordinater</translation>
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
        <translation>Redigér &amp;omrids linie</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Nu&amp;lstil omrids linie</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Afslut redigering</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Flyt kontrolpunkter en af gangen</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Flyt kontrolpunkter symmetrisk</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Åbn en polygon eller overskær en bezierkurve</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Luk denne bezier kurve</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Spejl horisontalt</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Spejl vertikalt</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Forskyd mod venstre</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Forskyd mod top</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Forskyd mod bund</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Roterer stien mod uret</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Roterer stien med uret</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Forøger størrelsen på stien med den viste %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Rotations-vinkel</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivér Redigering af omrids linie</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Nulstil omrids linien til den originale form på rammen</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Forskyd mod højre</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Sæt kryds for at få coordinater i forhold til siden, ellers er coordinaterne vist i forhold til objektet.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Reducér størrelsen på stien med viste %</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Formindsk størrelsen på stien med den viste værdi</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Forøg størrelsen på stien med den viste værdi</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>% til at forøge eller reducere med</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Værdi til at forøge eller reducere med</translation>
    </message>
    <message>
        <source>Set Contour to Image Clip</source>
        <translation>sæt kontur til billed-klipning</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Clipping Path of the Image</source>
        <translation>Nulstil kontur-linien til billedets klipnings-sti</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Dette dokument synes ikke at være er OpenOffice Tegnings-fil.</translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation>Gruppe%1</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importér &amp;OpenOffice.org Tegning...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importerer OpenOffice.org Tegnings-filer</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importerer de fleste OpenOffice.org Tegnings-filer ind i det aktive dokument, og konverterer deres vektordata til Scribus objekter.</translation>
    </message>
    <message>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation>OpenDocument 1.0 Tegnings-filer</translation>
    </message>
    <message>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation>OpenOffice.org 1.x Tegnings-filer</translation>
    </message>
    <message>
        <source>This file contains some unsupported features</source>
        <translation>Denne fil indeholder nogle ikke-understøttede egenskaber</translation>
    </message>
    <message>
        <source>The file could not be imported</source>
        <translation>Filen kunne ikke importeres</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Brug dokumentnavn som en forstavelse til afsnits typografier</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Spørg ikke igen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>OpenDokument import indstillinger</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Ved aktivering overskrives eksisterende typografier i det aktive dokument</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sammenflet afsnits typografier efter kvaliteter. Dette resulterer i færre ens afsnits typografier, vil bevare typografi kvaliteter,  selv om det originale dokuments typografier er navngivet forskelligt.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Sæt dokumentnavn foran afsnits typografiens navn i Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Gør disse indstillinger til standard værdier og spørg ikke igen ved import af et OASIS OpenDokument.</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sammenflet afsnits typografier</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Overskriv afsnits typografier</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
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
    <name>OneClick</name>
    <message>
        <source>Origin</source>
        <translation>Oprindelse</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Længde:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Højde:</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation>Vinkel:</translation>
    </message>
    <message>
        <source>Remember Values</source>
        <translation>Husk værdier</translation>
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
        <translation>Liniebredde</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Gem som PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Skriv til fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Væ&amp;lg...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Udlæs en fil for &amp;hver side</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Dette aktiverer eksport af en individuelt navngivet PDF fil for hver side i dokumentet. Sidenumre er tilføjet automatisk. Dette er mest brugbart for udskydning af PDF for kommerciel trykning.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF filer (*.pdf);;Alle filer (*)</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Gem-knappen bliver deaktiveret, hvis du prøver at eksportere PDF/X-3 og info-strengen mangler på PDF/X-3 fanebladet.</translation>
    </message>
    <message>
        <source>%1 does not exists and will be created, continue?</source>
        <translation>%1 eksisterer ikke og vil blive lavet, fortsæt?</translation>
    </message>
    <message>
        <source>Cannot create directory: 
%1</source>
        <translation>Kan ikke lave mappe: 
%1</translation>
    </message>
</context>
<context>
    <name>PDFToolBar</name>
    <message>
        <source>PDF Tools</source>
        <translation>PDF Værktøjer</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source></source>
        <translation></translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Gemmer PDF</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Eksporterer elementer på aktive side:</translation>
    </message>
    <message>
        <source>Exporting Master Page:</source>
        <translation>Eksporterer Masterside:</translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation>Eksporterer Side:</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Side:</translation>
    </message>
    <message>
        <source>Date:</source>
        <translation>Dato:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Vis udskrift</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Viser gennemsigtighed og gennemsigtige objekter i dit dokument. Ghostscript 7.07 eller nyere skal være installeret</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Giver forhåndsvisning af udskrift med simulering af CMYK farver istedet for RGB farver</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Vælger/Fravælger farven C (Cyan) </translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Vælger/Fravælger farven M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Vælger/Fravælger farven Y (Gul)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Vælger/Fravælger farven K (Sort)</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Vis trans&amp;parens</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Vis CMYK</translation>
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
        <translation>Reduktion af &amp;underliggende farve</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Navn på separation</translation>
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
        <translation>Gul</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Sort</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Skalering:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Udskriv...</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En måde at fjerne nogle af gråtonerne, som er sammensat af cyan, gul og magenta, så der i stedet for disse farver bruges sort. UCR påvirker mest de dele af billedet, som er neutrale og/eller mørke i farven, det, som er tættets på gråt. Brug af denne metode kan forbedre udskrivningen af nogle billeder og nogen eksperimenteren og testning er nødvendigt fra tilfælde til tilfælde. UCR formindsker muligheden for overmætning af CMY farverne.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Ændrer sidens målestok.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Luk</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation>Aktivér &amp;udjævning</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Instillinger for visning</translation>
    </message>
    <message>
        <source>Print Settings</source>
        <translation>Udskrivnings indstillinger</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Spejl side(r) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Spejl side(r) vertikalt</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Afskær ved sidens margener</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Udskriv i gråtoner</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Gennemtving overprint tilstand</translation>
    </message>
    <message>
        <source>Convert Spot Colors</source>
        <translation>Konvertér spot-farver</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Anvend ICC profiler</translation>
    </message>
    <message>
        <source>Fit to Width</source>
        <translation>Tilpas til bredde</translation>
    </message>
    <message>
        <source>Fit to Height</source>
        <translation>Tilpas til højde</translation>
    </message>
    <message>
        <source>Fit to Page</source>
        <translation>Tilpas til side</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Giver en mere behagelig forhåndsvisning af Type 1 fonte, TrueType fonte, OpenType fonte, EPS, PDF og vektorgrafik, men det tager lidt længere tid at vise siden</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Aktiverer konvertering af spot-farver til sammensatte farver. Medmindre du planlægger at trykke med spot-farver hos et kommercielt trykkeri, er det sansynligvis bedst at have aktiveret.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Aktiverer global overprint tilstand for dette dokument, tilsidesætter objekt indstillinger</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Tillader dig at indlejre ICC profiler i udskrift-data når farvestyring er aktiveret </translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Page:</source>
        <translation>Behandler Masterside:</translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation>Eksporterér Side:</translation>
    </message>
</context>
<context>
    <name>PStyleWBase</name>
    <message>
        <source>Form1</source>
        <translation>Form1</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorer og indrykning</translation>
    </message>
    <message>
        <source>Distances and Alignment</source>
        <translation>Afstande og placering</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Uncialer</translation>
    </message>
    <message>
        <source>Parent&apos;s Drop Cap Status</source>
        <translation>forælders Uncial status</translation>
    </message>
    <message>
        <source>Ch&amp;aracter Style</source>
        <translation>&amp;Skrifttegn-stile</translation>
    </message>
    <message>
        <source>Based On:</source>
        <translation>Baseret på:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Poly-linie</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Sti-tekst</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopi af</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Relaterer til</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Er forælder til</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Er barn af</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Side element kvaliteter</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Værdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Slægtskab</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Slægtskab med</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
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
        <translation>&amp;Annullér</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>First Page is:</source>
        <translation>Første side er:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Dokument layout</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Dobbelt sidet</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Midterste til højre</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Træk sider eller mastersider til skraldespanden for at slette dem</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Her er alle dine mastersider. For at lave en ny side, træk en masterside til sidevisningen herunder</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Arrangér sider</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Tilgængelige mastersider:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokument sider:</translation>
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
    <name>ParaStyleComboBox</name>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
</context>
<context>
    <name>PatternDialog</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Loading Patterns</source>
        <translation>Indlæser mønstre</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filer (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Size:</source>
        <translation>Størrelse:</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Opløsning:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukendt</translation>
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
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation>Duotone</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Farverum:</translation>
    </message>
</context>
<context>
    <name>PicSearchBase</name>
    <message>
        <source>Result</source>
        <translation>Resultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Søg resultater for: </translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Vis udskrift</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Select</source>
        <translation>&amp;Vælg</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+V</translation>
    </message>
</context>
<context>
    <name>PicSearchOptions</name>
    <message>
        <source>The filesystem will be searched for case insensitive file names when you check this on. Remember it is not default on most operating systems except MS Windows</source>
        <translation>Filsystemet vil blive søgt for filnavne uden at skelne mellem store/små bogstaver, når du aktiverer dette. Husk, at dette ikke er standard på de fleste operativ-systemer undtagen MS Windows</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Annullér søgning</translation>
    </message>
    <message>
        <source>Start Search</source>
        <translation>Start søgning</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Vælg en rodmappe for søgning</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - Billed-søgning</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Søgningen mislykkedes: %1</translation>
    </message>
</context>
<context>
    <name>PicSearchOptionsBase</name>
    <message>
        <source>Search Images</source>
        <translation>Søg billeder</translation>
    </message>
    <message>
        <source>Start at:</source>
        <translation>Start ved:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Vælg...</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Søg efter:</translation>
    </message>
    <message>
        <source>Case insensitive search</source>
        <translation>Søgning uden at skelne mellem store/små bogstaver</translation>
    </message>
    <message>
        <source>Search recursivly</source>
        <translation>Søg rekursivt</translation>
    </message>
    <message>
        <source>Searching</source>
        <translation>Søger</translation>
    </message>
    <message>
        <source>Start Search</source>
        <translation>Start søgning</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - Billed-søgning</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Ingen billeder med navnet &quot;%1&quot; blev fundet.</translation>
    </message>
    <message>
        <source>Not on a Page</source>
        <translation>Ikke på en side</translation>
    </message>
    <message>
        <source>JPG</source>
        <translation>JPG</translation>
    </message>
    <message>
        <source>TIFF</source>
        <translation>TIFF</translation>
    </message>
    <message>
        <source>PSD</source>
        <translation>PSD</translation>
    </message>
    <message>
        <source>EPS/PS</source>
        <translation>EPS/PS</translation>
    </message>
    <message>
        <source>PDF</source>
        <translation>PDF</translation>
    </message>
    <message>
        <source>JPG2000</source>
        <translation>JPG2000</translation>
    </message>
    <message>
        <source>emb. PSD</source>
        <translation>emb. PSD</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukendt</translation>
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
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation>Duotone</translation>
    </message>
    <message>
        <source>n/a</source>
        <translation>n/a</translation>
    </message>
</context>
<context>
    <name>PicStatusBase</name>
    <message>
        <source>Manage Pictures</source>
        <translation>Håndtér billeder</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <source>Path:</source>
        <translation>Sti:</translation>
    </message>
    <message>
        <source>Search...</source>
        <translation>Søger...</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>DPI:</source>
        <translation>DPI:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Farverum:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>Pixels:</source>
        <translation>Billedpunkter:</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skalering:</translation>
    </message>
    <message>
        <source>Printed:</source>
        <translation>Udskrevet:</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Layout</translation>
    </message>
    <message>
        <source>On Page:</source>
        <translation>På side:</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>eff. DPI:</source>
        <translation>eff. DPI:</translation>
    </message>
    <message>
        <source>Object:</source>
        <translation>Objekt:</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Vælg</translation>
    </message>
    <message>
        <source>Image Tools</source>
        <translation>Billed værktøjer</translation>
    </message>
    <message>
        <source>Layers &amp;&amp; Paths...</source>
        <translation>Lag &amp;&amp; Stier...</translation>
    </message>
    <message>
        <source>Image Visible</source>
        <translation>Billede er synlig</translation>
    </message>
    <message>
        <source>Image Effects...</source>
        <translation>Billed effekter...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Redigér billede...</translation>
    </message>
    <message>
        <source>Print Image</source>
        <translation>Udskriv billede</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Luk</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Gem som &amp;billede...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksportér som billede</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Eksporterer de valgte sider som bitmap billeder.</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Gem som billede</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Fejl ved udskrivning af uddata-fil(er).</translation>
    </message>
    <message>
        <source>Export successful</source>
        <translation>Eksporteret uden problemer</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Kan ikke finde plugin</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>Ukendt fejl</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Kan ikke finde symbol (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Plugin: indlæser %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>init fejlede</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>ukendt plugin type</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 indlæst</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 fejlede at indlæse: %2</translation>
    </message>
    <message>
        <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
        <translation>Der er problemer med at indlæse %1 af %2 plugins. %3 Dette er sansynligvis forårsaget af en slags afhængighed eller der findes gamle plugins i din installations-mappe. Hvis du sletter denne mappe og problemet stadig er der, reportér det da venligst på bugs.scribus.net.</translation>
    </message>
    <message>
        <source>Plugin: %1 initialized ok </source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 initialiserede ok </translation>
    </message>
    <message>
        <source>Plugin: %1 failed post initialization</source>
        <comment>plugin manager</comment>
        <translation>Plugin: %1 fejlede post initialisering</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Plugin håndtering</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Hvordan de køres</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Indlæs den?</translation>
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
        <source>You need to restart the application to apply the changes.</source>
        <translation>Programmet skal genstartes før ændringerne træder i kraft.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygon egenskaber</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Hjørn&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Anvend &amp;faktor</translation>
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
        <translation>Antal hjørner i polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Rotation i grader for polygoner</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Anvend convex/concav faktor for at ændre polygonens form</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Prøve polygon</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>En negativ værdi vil lave polygonen concav (eller stjerneformet), en positiv værdi vil lave den convex</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Tools</source>
        <translation>Værktøjer</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hjælpelinier</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografi</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Præferencer</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Eksterne værktøjer</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Orddeler</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Fonte</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Farvestyring</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokument element kvaliteter</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Indholdsfortegnelse og indeks</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tastatur genveje</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Diverse</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Plugins</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Preflight efterprøvning</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbog</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Printer</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>S&amp;tandardværdier</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Gem præferencer</translation>
    </message>
    <message>
        <source>Export...</source>
        <translation>Exportér...</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;nvend</translation>
    </message>
    <message>
        <source>All preferences can be reset here</source>
        <translation>Alle præferencer kan nulstilles her</translation>
    </message>
    <message>
        <source>Apply all changes without closing the dialog</source>
        <translation>Anvend alle ændringer uden at lukke vinduet</translation>
    </message>
    <message>
        <source>Export current preferences into file</source>
        <translation>Eksportér nuværende præferencer til fil</translation>
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
        <translation>Overføre gamle scribus indstillinger?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus har fundet eksisterende Scribus 1.2 præference filer.
Ønsker du at overføre dem til den nye Scribus version?</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Kunne ikke åbne præferencefil &quot;%1&quot; for skrivning: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Skrivning til præferencefil &quot;%1&quot; fejlede: QIODevice status kode %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Fejlede at åbne prefs file &quot;%1&quot;: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Fejlede at læse prefs XML fra &quot;%1&quot;: %2 linie %3, kol %4</translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation>Fejl ved skrivning af præferencer</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus var ikke i stand til at gemme sine præferencer: &lt;br&gt;%1&lt;br&gt;Check venligst fil og mappe tilladelser og tilgængelig diskplads.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Fejl ved indlæsning af præferencer</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus var ikke i stand til at indlæse sine præferencer:&lt;br&gt;%1&lt;br&gt;Standardindstillinger vil blive indlæst.</translation>
    </message>
    <message>
        <source>PDF 1.5</source>
        <translation>PDF 1.5</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>Magazine</source>
        <translation>Tidsskrift</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Gyldne snit</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Ni dele</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>You can select a predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Du kan vælge en forud defineret side layout her. &apos;Ingen&apos; lader margener være som de er, &apos;Gutenberg&apos; sætter margener klassisk. &apos;Tidsskrift&apos; sætter margener til samme værdi. Skydning er Venstre/Inderside værdi.</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åbn...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Afslut</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Kør</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Gem uddata...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Scripts</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Script konsol</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Skriv dine kommandoer her. Et markeret område køres som en script</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Udskrift fra din script</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Gem Python kommandoer i fil</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Tekst filer (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Gem aktuelle uddata</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gem &amp;som...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>&amp;Kør som konsol</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribus Python konsol</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Denne konsol er baseret på standard Python konsollen, så den indeholde nogle begrænsninger specielt m.h.t. hvidt-rum (whitespaces). Se venligst Scribus manualen for mere information.</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Åbn python script fil</translation>
    </message>
    <message>
        <source>Line: %1 Column: %2</source>
        <translation>Linie: %1 kolonne: %2</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY)</source>
        <translation>Python Scripts (*.py *.PY)</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rød:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Mætn:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Værdi:</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definér bruger farver &gt;&gt;</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Vælg farve</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Tilføj til bruger farver</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bl&amp;å:</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Grøn:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Grundlæggende farver</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Bruger farver</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lfa kanal:</translation>
    </message>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Farv&amp;etone:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Dir</source>
        <translation>Mappe</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Tilbage</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Gem</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sortér</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Åbn</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fejl</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Skriv: %1</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Sortér efter &amp;størrelse</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Sortér efter &amp;dato</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Sortér efter &amp;navn</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Ny mappe 1</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>Mappen</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Fil &amp;type:</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Fil &amp;navn:</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Slet %1</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Indlæs igen</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Ny mappe</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Usorteret</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Se &amp;i;</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Vis indhold af fil</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Ny mappe %1</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Læs-skriv</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Skrivebeskyttet</translation>
    </message>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopiér eller flyt en fil</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Er du sikker på, at du vil slette %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Find mappe</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Kvaliteter</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Vis &amp;skjulte filer</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Utilgængelig</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Fil ikke fundet.
Kontrollér sti og filnavn.</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Vis liste</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Speciel</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Skriv-kun</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>Symbol-lænken</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filer (*)</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Mapper</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symbolsk lænke til speciel</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alle filer (*.*)</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Indlæs: %1</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Nyt navn</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Mappe:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>En mappe tilbage</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Vis filinfo</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>Filen</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Lav ny mappe</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symbolsk lænke til fil</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symbolsk lænke til mappe</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Vis detaljer</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Font</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Font s&amp;til</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>&amp;Gennemstreg</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Understreg</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>Far&amp;ve</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Prøve</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Anvend</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Luk</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Vælg font</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Lav igen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Fortryd</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Markér alt</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Stil på række</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Tilpas...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Om Qt&lt;/h3&gt;&lt;p&gt;Dette program bruger Qt version %1.&lt;/p&gt;&lt;p&gt;Qt er et C++ værktøj til multiplatform GUI &amp;amp; program udvikling.&lt;/p&gt;&lt;p&gt;Qt giver enkelt-kildetekst portabilitet mellem MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, og alle de store commercielle Unix varianter.&lt;br&gt;Qt fås også til indlejrede systemer.&lt;/p&gt;&lt;p&gt;Qt er et Trolltech produkt. Se &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for mere information.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Initialiserer...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Baggrund</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Ønsker du virkelig at overskrive filen:
%1 ?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-billeder (*.svg *.svgz);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Nyhedsbreve</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brochurer</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Kataloger</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Løbesedler</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kort</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Brevhoveder</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Kuverter</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visitkort</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendere</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Reklamer</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Labels</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menuer</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programmer</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF-formularer</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-præsentationer</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Tidsskrifter</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Plakater</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Meddelelser</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Tekst-dokumenter</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Foldere</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Egne skabeloner</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filer (*)</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importerer tekst</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle understøttede formater</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML filer</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Tekst filer</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Komma adskilt data Filer</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV_data</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV_overskrift</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Ekstern Lænker</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Font %1 indeholder fejl, kan ikke bruges</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Tekst filtre</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Medie bokse</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albansk</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskisk</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgarsk</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalansk</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Kinesisk</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tjekkisk</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dansk</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Hollandsk</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Engelsk</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Engelsk (Britisk)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Tysk</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finsk</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Fransk</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galicisk</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Græsk</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungarsk</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesisk</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiensk</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreansk</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litaurisk</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norsk (Nynorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norsk</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polsk</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisk</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Svensk</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spansk</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Spansk (Latin)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovakisk</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovensk</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbisk</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente en farve med et tomt navn.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke ændre en farve med et tomt navn.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke oprette en farve med et tomt navn.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke slette en farve med et tomt navn.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke erstatte en farve med et tomt navn.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Måle-enhed er uden for tilladt område. Brug en af scribus.UNIT_* konstanter.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Målet er ikke en billedramme.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Hjørne-radius skal være et positivt number.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente font-størrelse fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente font fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente tekst-størrelse fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente antal kolonner fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente linie-afstand fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente kolonne-afstand fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente tekst fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke placere tekst i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke indsætte tekst i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Justering uden for tilladt område. Brug en af scribus.ALIGN* konstanter.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Markerings-index uden for tilladt område</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Tekst dokumenter</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Walisisk</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Farve ikke fundet - python fejl</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Bruger (valgfri) indstilling:</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standard indstilling:</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Behandler korte ord. Vent venligst...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Behandling af korte ord. Færdig.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Tyrkisk</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrainsk</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Filnavnet skal være en streng.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Kan ikke slette billed-type indstillinger.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Billed-typen skal være en streng.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>&apos;allTypes&apos; kvalitet er READ-ONL</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Eksport af billede mislykkedes</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Farve ikke fundet.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Farve ikke fundet i dokument.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Farve ikke fundet i standard farver.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Kan ikke skalere med 0%.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Specificerede element er ikke en billedramme.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Font ikke fundet.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Kan ikke gengive et tomt indhold.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Kan ikke have et lag uden navn.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Lag ikke fundet.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Kan ikke fjerne det sidste lag.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Kan ikke oprette et lag uden navn.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Indsæt index - uden for tilladte værdier.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte tekst-justering i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Font-størrelse uden for tilladt område - skal være 1 &lt;= størrelse &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte font-størrelse i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte font i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Linie-afstand uden for tilladt område - skal være &gt;= 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte linie-afstand i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Kolonne-afstand uden for tilladt område - skal være positiv.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte kolonne-afstand i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Kolonne-antal uden for tilladt område - skal være &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte kolonne-antal i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Kan ikke markere tekst i andet end tekst-ramme</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke slette tekst i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte tekst-fyld i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte tekst-streg i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte tekst-farvemætning i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Kan kun sammenkæde tekst-rammer.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Mål-rammen har lænke til en anden ramme.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Mål-rammen har lænke fra en anden ramme.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Kilde og mål er det samme objekt.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke bryde lænke for andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Kan ikke konvertere fonte til kurver i andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte bogmærke i andet end tekst-ramme</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Kan ikke hente info fra andet end tekst-ramme</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument Tekst Dokumenter</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisk</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugisisk</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugisisk (BR)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus nedbrud</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus nedbrud p.g.a Signal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Master side </translation>
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
        <translation>Kunne ikke åbne uddata-fil %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Kan ikke skrive til Uddata-strøm</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Efterprøvning af instillinger fejlede: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>kunne ikke åbne input-fil %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Ude af stand til at læse indstillinger ZML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (line %2 col %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Ude af stand til at læse indstillinger ZML: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>null rod node</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; ugyldig</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>fandt %1 &lt;%2&gt; noder, behøver 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>uventet null &lt;%2&gt; node</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>node &lt;%1&gt; ikke et element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;%1&gt; mangler `værdi&apos; kvalitet</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;%1&gt; værdi skal være `true&apos; eller `false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;lpiSettingsEntry&gt; mangler `navn&apos; kvalitet</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Freetype2 bibliotek ikke tilgængelig</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Font %1 indeholder fejl (læse strøm), ingen indlejring</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Font %1 indeholder fejl (ingen fontnavn), kan ikke bruges</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Font %1 indeholder fejl i glyf %2 (tegnkode %3)</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Font %1 kan ikke læses, ingen indlejring</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Kunne ikke indlæse font %1 - font type ukendt</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Font %1 indlæst fra %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Font %1(%2) er en kopi af %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Indlæser font %1 (fundet ved brug af fontconfig)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Kunne ikke indlæse en font - freetype2 kunne ikke finde font-filen</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>Udpakker fontnavn %1 fra font %2 (forskydning=%3, nTabeller=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>memcpy header: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabel &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>memcpy tabel: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy forskydning: %1 %2 %3</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Scribus Udviklings Version</translation>
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
        <translation>Punkter (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Tommer (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimeter (cm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Fil eksisterer</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Erstat</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Dokument skabelon: </translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Åbn dokument mislykkedes.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Gem dokument mislykkedes.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argument skal være et sideelement navn, eller PyCObjekt tilfælde</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Egenskab ikke fundet</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Barn ikke fundet</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Kunne ikke konverterpe &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Egenskab type &apos;%1&apos; ikke understøttet</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Kunne ikke konvertere &apos;%1&apos; til egenskab type &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Typer stemmer overens, men sætte egenskab mislykkedes.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Kan lave gruppe af mindre end to elementer</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Kan lave gruppe af mindre end to elementer</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Behøver markering eller argument liste af elementer, der skal laves til gruppe</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Ude af stand til at gemme pixmap</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Der eksisterer allerede et objekt med det navn der efterspørges.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Punktliste skal indeholde mindst to punkter (fire værdier).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Punktliste skal indeholde et lige antal værdier.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Punktliste skal indeholde mindst tre punkter (seks værdier).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Punktliste skal indeholde mindst fire punkter (otte værdier).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Punktliste skal have et multiple af seks værdier.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objekt ikke fundet.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Stil ikke fundet.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte stil på andet end tekstrammer.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Gem EPS mislykkedes.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Sidenummer er uden for tilladt område.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>Argument er ikke en liste: skal være en liste af komma-tal.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>Argument indeholder andet end numeriske værdier: skal være en liste af komma-tal.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>Argument indeholder andet end numeriske værdier: skal være en liste af komma-tal.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Linie-bredde uden for tilladt område, skal være 0 &lt;= linie_bredde &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Linie-farvemætning uden for tilladt område, skal være 0 &lt;= farvemætning &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Fyld-farvemætning uden for tilladt område, skal være 0 &lt;= farvemætning &lt;= 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Liniestil ikke fundet.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekt er ikke en sammkædet tekst-ramme, kan ikke bryde lænke.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Kun tekstrammer kan kontrolleres for overløb</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Filnavnet må ikke være en tom streng.</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Kør Script...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Vis konsol</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;Om script...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus scripts</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Nyeste scripts</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Om script</translation>
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
        <translation>Scribus Python grænseflade modul

Dette modul er Python grænseflade for Scribus. Det indeholder funktioner
til at kontrollere scribus og til at manipulere objekter på canvas. Hver
funktion er dokumenteret individuelt herunder.

Nogle få ting er fælles for det meste i grænsefladen.

De fleste funktioner opererer på rammer. Rammer indentificeres ved deres navn,
en streng - de er ikke virkelige Python objekter. Mange funktioner tager en
valgfri (non-keyword) parameter, et ramme-navn.
Mange indsigelser er også fælles for de fleste functioner. Disse er
endnu ikke dokumenteret i docstrengen for hver function.
- Mange funktioner vil rejse en NoDocOpenError hvis du prøver at bruge dem
 uden at der er et dokument at operere på.
- Hvis du ikke giver et ramme-navn til en funktion, der behøver det, så 
vil funktionen bruge den aktive ramme, hvis der er nogen, eller rejse 
en NoValidObjectError hvis den ikke kan finde noget at operere på.
- Mange funktioner vil rejse en WrongFrameTypeError hvis du prøver at bruge dem
på en rammetype, hvor det ikke giver mening at bruge dem. For eksempel, sætte
tekstfarve på en billedramme giver ikke mening, og vil resultere
i at denne indsigelse rejses.
- Fejl der kommer fra kald til de underliggende Python API vil blive
sendt videre uændret. Som sådan, er listen af indsigelser, der rejses af
en funktion som nævnt her og i dens docstreng, ikke komplet.

Detaljer om hvilke indsigelser hver funktion kan rejse er givet i 
funktionens dokumentation, selvom, som med det meste Python kode 
er denne liste ikke udtømmende, på grund af indsiglser fra kaldte funktioner.
</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>side</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Sort</translation>
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
        <translation>Gul</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Farvehjul</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Font forhåndsvisning</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Mit Udvidelsesmodul</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Nyt fra skabelon</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS Importfilter</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Gem som skabelon</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Scripter</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Korte ord</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVG Eksport</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>SVG Import</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Tegning importfilter</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Laver Font-cache</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Ny font fundet, undersøger...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Ændret font fundet, undersøger...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Indlæser Font-cache</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Gemmer opdateret Font-cache</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Leder efter fonte</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Ændringerne i dit dokument er ikke gemt og du har valgt at fortryde dem. Ønsker du at fortsætte?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>førsteSideOrden er større end tilladt.</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus nedbrud p.g.a følgende indsigelse : %1</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>En fil med navnet &apos;%1&apos; eksisterer allerede.&lt;br/&gt;Ønsker du at erstatte den med filen du er ved at gemme?</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Gammelt .sla format understøttelse</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Tysk (Trad.)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Eksporterer PostScript filer</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Udskriver fil</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Du prøver at importere flere sider end der er tilgængelig i det aktuelle dokument talt fra den aktive side.&lt;/p&gt;Vælg et af følgende:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Lav&lt;/b&gt; manglende sider&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importér&lt;/b&gt; sider indtil den sidste side&lt;/li&gt;&lt;li&gt;&lt;b&gt;Annullér&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Lav</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Thai</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Stregkode generator</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Tegning (*.sxd *.odg);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Luxembourgsk</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word Dokumenter</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB Dokumenter</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB_data</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>PDB Import</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>kunne ikke åbne fil %1</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>Japansk</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabisk</translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation>Estisk</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Den givne master side stemmer ikke overens med nogen eksisterende.</translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation>Islandsk</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation>%1 er måske beskadiget: manglende opløsnings-mærker</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>Denne fil er ikke genkendt som et PDB dokument. Reportér venligst dette som en fejl, hvis du er sikker på det er det.</translation>
    </message>
    <message>
        <source>Breton</source>
        <translation>Bretonsk</translation>
    </message>
    <message>
        <source>English (American)</source>
        <translation>Engelsk (Amerikansk)</translation>
    </message>
    <message>
        <source>English (Australian)</source>
        <translation>Engelsk (Australsk)</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing or wrong resolution tags</source>
        <translation>%1 er måske beskadiget: manglende eller forkerte opløsnings-mærker</translation>
    </message>
    <message>
        <source>Dzongkha</source>
        <translation>Dzongkha</translation>
    </message>
    <message>
        <source>Latin</source>
        <translation>Latinsk</translation>
    </message>
    <message encoding="UTF-8">
        <source>Norwegian (Bokmål)</source>
        <translation>Norsk (Bokmål)</translation>
    </message>
    <message>
        <source>Romanian</source>
        <translation>Rumænsk</translation>
    </message>
    <message>
        <source>Quarto</source>
        <translation>Quarto</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation>Folioark</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Letter</translation>
    </message>
    <message>
        <source>Govt. Letter</source>
        <translation>Govt. Letter</translation>
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
    <message>
        <source>The Font(s):
%1 are not available.
They have been replaced by &quot;Courier&quot;
Therefore the image may be not correct</source>
        <translation>Fonten(e):
%1 er ikke tilgængelig.
De er blevet erstattet af &quot;Courier&quot;
Derfor er billedet måske ikke korrekt</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</source>
        <translation>Du kører en udviklings version af Scribus 1.3.x. Det dokument du arbejder med, er oprindeligt lavet i Scribus 1.2.x.  Når du gemmer dokumentet i 1.3.x, kan det ikke længere redigeres i Scribus 1.2.x versioner. For at bevare muligheden for at kunne redigere i 1.2.x, gem denne fil under et andet navn og redigér den nye fil, originalen vil da være urørt. Er du sikker på, at du ønsker at fortsætte med denne handling?</translation>
    </message>
    <message>
        <source>Font %1(%2) is broken</source>
        <translation>Font %1(%2) indeholder fejl</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2</source>
        <translation>Font %1 indeholder fejl i glyf %2</translation>
    </message>
    <message>
        <source>font %1 </source>
        <translation>ont %1 </translation>
    </message>
    <message>
        <source>size %1 </source>
        <translation>størrelse %1 </translation>
    </message>
    <message>
        <source>+style </source>
        <translation>+stil </translation>
    </message>
    <message>
        <source>+color </source>
        <translation>+farve </translation>
    </message>
    <message>
        <source>+underline </source>
        <translation>+understreg </translation>
    </message>
    <message>
        <source>-underline </source>
        <translation>-understreg </translation>
    </message>
    <message>
        <source>+strikeout </source>
        <translation>+overstreg </translation>
    </message>
    <message>
        <source>-strikeout </source>
        <translation>-overstreg </translation>
    </message>
    <message>
        <source>+shadow </source>
        <translation>+skygge </translation>
    </message>
    <message>
        <source>-shadow </source>
        <translation>-skygge </translation>
    </message>
    <message>
        <source>+outline </source>
        <translation>+omrids </translation>
    </message>
    <message>
        <source>-outline </source>
        <translation>-omrids </translation>
    </message>
    <message>
        <source>+tracking %1 </source>
        <translation>+spatiering %1 </translation>
    </message>
    <message>
        <source>-tracking </source>
        <translation>-spatiering </translation>
    </message>
    <message>
        <source>+baseline %1 </source>
        <translation>+basislinie %1 </translation>
    </message>
    <message>
        <source>+stretch </source>
        <translation>+udvid </translation>
    </message>
    <message>
        <source>parent= %1</source>
        <translation>forælder= %1</translation>
    </message>
    <message>
        <source>Transparency out of bounds, must be 0 &lt;= transparency &lt;= 1.</source>
        <comment>python error</comment>
        <translation>Gennemsigtighed er uden for tilladt område, skal være 0 &lt;= gennemsigtighed &lt;= 1.</translation>
    </message>
    <message>
        <source>Blendmode out of bounds, must be 0 &lt;= blendmode &lt;= 15.</source>
        <comment>python error</comment>
        <translation>Blandings-tilstand er uden for tilladt område, skal være 0 &lt;= blandingstilstand &lt;= 15.</translation>
    </message>
    <message>
        <source>Cannot get number of lines of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente antal linier fra andet end tekst-ramme.</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Support</source>
        <translation>Scribus 1.2.x Understøttelse</translation>
    </message>
    <message>
        <source>Scribus 1.3.4 Support</source>
        <translation>Scribus 1.3.4 understøttelse</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.epsi *.EPSI *.ps *.PS);;</source>
        <translation>Alle understøttede formater (*.eps *.EPS *.epsi *.EPSI *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Font %1 has broken metrics in file %2, ignoring metrics</source>
        <translation>Font %1 indeholder fejl i målene i fil %2, ignorerer målene</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>PDF Push Button</source>
        <translation>PDF trykknap</translation>
    </message>
    <message>
        <source>PDF Text Field</source>
        <translation>PDF tekstfelt</translation>
    </message>
    <message>
        <source>PDF Check Box</source>
        <translation>PDF afkrydsningsboks</translation>
    </message>
    <message>
        <source>PDF Combo Box</source>
        <translation>PDF kombo-boks</translation>
    </message>
    <message>
        <source>PDF List Box</source>
        <translation>PDF liste-boks</translation>
    </message>
    <message>
        <source>PDF Text Annotation</source>
        <translation>PDF kommentar</translation>
    </message>
    <message>
        <source>PDF Link Annotation</source>
        <translation>PDF lænke-kommentar</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Poly-linie</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Sti-tekst</translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation>Tysk (Schweizisk)</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebraisk</translation>
    </message>
    <message>
        <source>Copy of %1 (%2)</source>
        <translation>Kopi af %1 (%2) </translation>
    </message>
    <message>
        <source>Scribus 1.3.0-&gt;1.3.3.7 Support</source>
        <translation>Scribus 1.3.0-&gt;1.3.3.7 understøttelse</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Lav igen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Fortryd</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Markér alt</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>System Menu</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Rul op</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Rul ned</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalisér</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimér</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maksimér</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Luk</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Flyt</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Luk</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimér</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
    </message>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Gendan</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimér</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Mak&amp;simér</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Gendan ned</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Forbliv &amp;Øverst</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Rul &amp;op</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>Rul &amp;ned</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Dokument indstillinger</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokument information</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hjælpelinier</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Værktøjer</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Orddeler</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Fonte</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokument element kvaliteter</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Indholdsfortegnelse og indeks</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Farvestyring</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Preflight efterprøvning</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Tilpasser farver</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Sektioner</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Kør som udvidelses script</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY);; All Files (*)</source>
        <translation>Python Scripts (*.py *.PY);;Alle filer (*)</translation>
    </message>
</context>
<context>
    <name>SMAlignSelect</name>
    <message>
        <source>P</source>
        <comment>P as in Parent</comment>
        <translation>F</translation>
    </message>
    <message>
        <source>Use parent style&apos;s alignment instead of overriding it</source>
        <translation>Brug forælder typografiers justering i stedet for at tilsidesætte dem</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Håndtér typografier &amp;&amp; stile</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&lt;&lt; &amp;Done</source>
        <translation>&lt;&lt; &amp;Færdig</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;nvend</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation>&amp;Klon</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Please select a unique name for the style</source>
        <translation>Vælg venligst et unikt navn for typografien</translation>
    </message>
</context>
<context>
    <name>SMCStylePage</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Parent style</source>
        <translation>Forælder stil</translation>
    </message>
    <message>
        <source>Font face</source>
        <translation>Fontnavn</translation>
    </message>
    <message>
        <source>Font size</source>
        <translation>Fontstørrelse</translation>
    </message>
    <message>
        <source>Tracking</source>
        <translation>Spatiering </translation>
    </message>
    <message>
        <source>Baseline offset</source>
        <translation>Basislinie forskydning</translation>
    </message>
    <message>
        <source>Horizontal scaling</source>
        <translation>Horisontal skalering</translation>
    </message>
    <message>
        <source>Vertical scaling</source>
        <translation>Vertikal skalering</translation>
    </message>
    <message>
        <source>Language</source>
        <translation>Sprog</translation>
    </message>
    <message>
        <source>Fill color</source>
        <translation>Fyldfarve</translation>
    </message>
    <message>
        <source>Fill shade</source>
        <translation>Fyld Farvemætning</translation>
    </message>
    <message>
        <source>Stroke color</source>
        <translation>Stregfarve</translation>
    </message>
    <message>
        <source>Stroke shade</source>
        <translation>streg Farvemætning</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Nuance</translation>
    </message>
    <message>
        <source>Based On:</source>
        <translation>Baseret på:</translation>
    </message>
    <message>
        <source>Language:</source>
        <translation>Sprog:</translation>
    </message>
</context>
<context>
    <name>SMCharacterStyle</name>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Character Styles</source>
        <translation>Skrifttegn-stile</translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation>Skrifttegn-stil</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Clone of %1</source>
        <translation>Klon af %1</translation>
    </message>
    <message>
        <source>%1 (%2)</source>
        <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
        <translation>%1 (%2)</translation>
    </message>
</context>
<context>
    <name>SMColorCombo</name>
    <message>
        <source>Use Parent Value</source>
        <translation>Brug forælder værdi</translation>
    </message>
</context>
<context>
    <name>SMFontComboH</name>
    <message>
        <source>Use Parent Font</source>
        <translation>Brug forælder font</translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Line Styles</source>
        <translation>Liniestile</translation>
    </message>
    <message>
        <source>Line Style</source>
        <translation>Liniestil</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Clone of %1</source>
        <translation>Klon af %1</translation>
    </message>
    <message>
        <source>%1 (%2)</source>
        <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
        <translation>%1 (%2)</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Massiv linie</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Streg linie</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Prik linie</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Streg prik linie</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Streg prik prik linie</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt </translation>
    </message>
</context>
<context>
    <name>SMPStyleWidget</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast linieafstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatisk linieafstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Ret ind efter basislinie-gitter</translation>
    </message>
    <message>
        <source>Alignment</source>
        <translation>Justering</translation>
    </message>
    <message>
        <source>Distances and Alignment</source>
        <translation>Afstande og placering</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Uncialer</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorer og indrykning</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation>Skrifttegn-stile</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linier:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Afstand fra tekst:</translation>
    </message>
    <message>
        <source>Parent Style</source>
        <translation>Forælder stil</translation>
    </message>
    <message>
        <source>Line Spacing Mode</source>
        <translation>Linieafstand tilstand</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linie afstand</translation>
    </message>
    <message>
        <source>Space Above</source>
        <translation>Afstand over</translation>
    </message>
    <message>
        <source>Space Below</source>
        <translation>Afstand under</translation>
    </message>
    <message>
        <source>Drop Cap Lines</source>
        <translation>Uncial linier</translation>
    </message>
    <message>
        <source>Drop Cap Offset</source>
        <translation>Uncial forskydning</translation>
    </message>
    <message>
        <source>First Line Indent</source>
        <translation>Første linie indrykning</translation>
    </message>
    <message>
        <source>Left Indent</source>
        <translation>Venstre indrykning</translation>
    </message>
    <message>
        <source>Right Indent</source>
        <translation>Højre indrykning</translation>
    </message>
    <message>
        <source>Based On:</source>
        <translation>Baseret på:</translation>
    </message>
</context>
<context>
    <name>SMParagraphStyle</name>
    <message>
        <source>Paragraph Styles</source>
        <translation>Afsnits typografier</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Afsnits typografi</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny typografi</translation>
    </message>
    <message>
        <source>Clone of %1</source>
        <translation>Klon af %1</translation>
    </message>
    <message>
        <source>%1 (%2)</source>
        <comment>This for unique name when creating a new character style. %1 will be the name of the style and %2 will be a number forming a style name like: New Style (2)</comment>
        <translation>%1 (%2)</translation>
    </message>
</context>
<context>
    <name>SMReplaceDia</name>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>Replace with</source>
        <translation>Erstat med</translation>
    </message>
</context>
<context>
    <name>SMReplaceDiaBase</name>
    <message>
        <source>Delete Styles</source>
        <translation>Slet stile</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>An&amp;nullér</translation>
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
        <translation>Ingen typografi</translation>
    </message>
</context>
<context>
    <name>SMScComboBox</name>
    <message>
        <source>Use Parent Value</source>
        <translation>Brug forælder værdi</translation>
    </message>
</context>
<context>
    <name>SMShadeButton</name>
    <message>
        <source>Use Parent Value</source>
        <translation>Brug forælder værdi</translation>
    </message>
</context>
<context>
    <name>SMStyleSelect</name>
    <message>
        <source>P</source>
        <comment>P as in Parent</comment>
        <translation>F</translation>
    </message>
    <message>
        <source>Use parent style&apos;s effects instead of overriding them</source>
        <translation>Brug forælder typografiers effekter i stedet for at tilsidesætte dem</translation>
    </message>
</context>
<context>
    <name>SMTabruler</name>
    <message>
        <source> Parent Tabs </source>
        <translation> Forælder faneblade </translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Typografi Indstillinger</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Typografi i nuværende afsnit</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Fyldfarve indstillinger</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Tekstens fyldfarve</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Mætningsgrad af tekstens fyldfarve</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Stregfarve indstillinger</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Tekstens stregfarve</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Mætningsgrad af tekstens stregfarve</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Font indstillinger</translation>
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
        <translation>Font for den markerede tekst</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Fontstørrelse</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skalerer skrifttegnenes bredde</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Skalerer skrifttegnenes højde</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Skrifttegn indstillinger</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuel spatiering</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Exports SVG Files</source>
        <translation>Eksporterer SVG filer</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Eksporterer den aktive side til en SVG fil.</translation>
    </message>
    <message>
        <source>Save as &amp;SVG...</source>
        <translation>Gem som &amp;SVG...</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importér &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importerer SVG filer</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importerer de fleste SVG filer ind i det aktive dokument,
og konverterer deres vektordata til Scribus objekter.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Skalérbar Vektor-grafik</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG-fil indeholder nogle ikke-understøttede egenskaber</translation>
    </message>
    <message>
        <source>The file could not be imported</source>
        <translation>Filen kunne ikke importeres</translation>
    </message>
</context>
<context>
    <name>SVGPlug</name>
    <message>
        <source>Group%1</source>
        <translation>Gruppe%1</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Korte ord</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Anvend mellemrum, der ikke kan brydes, på: </translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Markerede rammer</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Aktiv &amp;side</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>Alle &amp;elementer</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Kun markerede rammer behandlet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Kun aktive side behandlet.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Alle elementer i dokumentet behandlet.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Bruger indstillinger</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Den indbyggede konfiguration</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Gem bruger konfiguration</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Genindlæs den indbyggede konfiguration og fjern den brugerdefinerede</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Redigér bruger konfiguration. Hvis du gemmer den, vil den blive brugt i stedet for den indbyggede konfiguration</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Korte ord</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Bruger konfiguration findes allerede. Ønsker du virkelig at overskrive den?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Kan ikke skrive fil %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Bruger indstillinger gemt</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Den indbyggede konfiguration er genindlæst</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Kan ikke åbne fil %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Gem som skabel&amp;on...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Gem et dokument som en skabelon</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Gem et dokument som en skabelon. God måde at gøre det indledende arbejde lettere for dokumenter med konstant udseende</translation>
    </message>
</context>
<context>
    <name>ScGTFileDialog</name>
    <message>
        <source>Select a file to import</source>
        <translation>Vælg en fil for importering</translation>
    </message>
    <message>
        <source>Append</source>
        <translation>Tilføj</translation>
    </message>
    <message>
        <source>Show options</source>
        <translation>Vis indstillinger</translation>
    </message>
</context>
<context>
    <name>ScInputDialog</name>
    <message>
        <source>Input Dialog</source>
        <translation>Inddata vindue</translation>
    </message>
    <message>
        <source>InputDialog</source>
        <translation>Inddata vindue</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Vedvarende</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Indlæs/Gem/Importér/Exportér</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukendt</translation>
    </message>
</context>
<context>
    <name>ScProgressBar</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 of %2</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Top</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Højre</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Bund</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Tillad placering mod...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontalt</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikalt</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Bjælkens retning...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Udskrivning...</translation>
    </message>
</context>
<context>
    <name>Scribus12Format</name>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Baggrund</translation>
    </message>
</context>
<context>
    <name>Scribus134Format</name>
    <message>
        <source>Scribus 1.3.4 Document</source>
        <translation>Scribus 1.3.4 dokument</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
</context>
<context>
    <name>Scribus13Format</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
    <message>
        <source>Scribus 1.3.0-&gt;1.3.3.7 Document</source>
        <translation>Scribus 1.3.0-&gt;1.3.3.7 Dokument</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source></source>
        <translation></translation>
    </message>
</context>
<context>
    <name>ScribusCore</name>
    <message>
        <source>Initializing Plugins</source>
        <translation>Initialiserer Plugins</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Initialiserer tastatur-genveje</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Indlæser præferencer</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Indlæser ICC-Profiler</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Leder efter fonte</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Der er ikke fundet nogen fonte på dit system.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Afslutter nu.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatal fejl</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Font-system initialiseret</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Nyt lag</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Baggrund</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Ønsker du virkelig at slette al din tekst?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Kan ikke slette element, der i brug</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Elementet %1 redigeres  i den indbyggede tekstbehandler. Slet funktionen vil blive annulleret</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation>Der opstod en fejl under indlæsning af ICC-profiler, farvestyring er ikke aktiveret.</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Tilpasser farver</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Nogle objekter er låst.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Frigør alt</translation>
    </message>
    <message>
        <source>&amp;Skip locked objects</source>
        <translation>&amp;Spring over låste objekter</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
        <translation>Antal kopier: %1
Horisontal afstand: %2
Vertikal afstand: %3</translation>
    </message>
    <message>
        <source>Default Paragraph Style</source>
        <translation>Standard afsnits-typografi</translation>
    </message>
    <message>
        <source>Default Character Style</source>
        <translation>Standard Skrifttegn-stil</translation>
    </message>
    <message>
        <source>remove manual paragraphstyle</source>
        <translation>fjern manuel afsnits typografi</translation>
    </message>
    <message>
        <source>remove manual charstyle</source>
        <translation>fjern manuel tegn-stil</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3
Rotation: %4</source>
        <translation>Antal kopier: %1
Horisontal forskydning: %2
Vertikal forskydning: %3
Rotation: %4</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Initialiserer den indbyggede tekstbehandler</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Initialiserer orddeler</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Indlæser scrapbog</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Laver genveje</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Redigér</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatal fejl</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Åbn n&amp;yeste</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportér</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>S&amp;til</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>Far&amp;ve</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Farve&amp;mætning</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Font</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekter</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Vis udskrift indstillinger</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Niveau</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Placér i la&amp;g</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF Indstillinger</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>For&amp;m</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>K&amp;onvertér til</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Citat</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellemrum</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Si&amp;de</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>Vi&amp;s</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Vi&amp;nduer</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjælp</translation>
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
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importerer sider...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importér side(r)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Import er færdig</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Fandt ingen ting at importere</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Fil %1 er ikke i et acceptabelt forat</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Indlæser...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Nogle ICC profiler brugt i dette dokument er ikke installeret:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> var erstattet med: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(konverteret)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Kan ikke skrive filen: 
%1</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Gemmer...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus har opdaget nogle fejl. Overvej at bruge Preflight efterprøvning  for at rette dem</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorér</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Afbryd</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Udskrivning...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Udskrivning fejlede!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Kan ikke Fjerne element, der i brug</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Elementet %1 redigeres  i den indbyggede tekstbehandler. Klip funktionen vil blive annulleret</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Om Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus manual</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfiler (*.txt);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Konvertér side til masterside</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Farvemætning:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Farvemætning</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Følgende programmer mangler:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript : Du kan ikke bruge EPS billeder eller vis udskrift</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus har opdaget nogle fejl.
Overvej at bruge Preflight efterprøvning  for at rette dem.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS Filer (*.eps);;Alle Filer (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Opdaget nogle fejl.
Overvej at bruge Preflight efterprøvning for at rette dem</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Side%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Nogle objekter er låst.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Lås alt</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Frigør alt</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Programmet %1 kører allerede!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Programmet %1 findes ikke!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Den valgte farve findes ikke i dokumentets farvesæt. Indtast venligst et navn for denne nye farve.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Farve ikke fundet</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Navnet du har valgt eksisterer allerede. Indtast venligst et andet navn for denne nye farve.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Niveau</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Send til lag</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>&amp;Vis udskrift indstillinger</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Værktøjer</translation>
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
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript : Du kan ikke bruge EPS billeder eller Postscript forhåndsvisning af udskrift</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript mangler: Postscript forhåndsvisning af udskrift er ikke tilgængelig</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Ny masterside %1</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Ønsker du virkelig at erstatte det eksisterende billede?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Indhold</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>Liga&amp;turer</translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation>Indsæt nyere</translation>
    </message>
    <message>
        <source>&amp;Character</source>
        <translation>&amp;Skrifttegn</translation>
    </message>
    <message>
        <source>&amp;Quote</source>
        <translation>&amp;Citat</translation>
    </message>
    <message>
        <source>S&amp;paces &amp;&amp; Breaks</source>
        <translation>&amp;Mellemrum &amp;&amp; Skift</translation>
    </message>
    <message>
        <source>Updating Pictures</source>
        <translation>Opdaterer billeder</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation>Gruppe%1</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Ønsker du virkelig at slette al din tekst?</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Ugyldigt argument: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Fil %1 eksisterer ikke, afslutter.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Brug: scribus [tilvalg ... ] [fil]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Tilvalg:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Udskriv hjælp (denne besked) og afslut</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Bruger xx som en genvej for et sprog, f.eks.  `en&apos; or `de&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Lav en liste over de aktuelle installerede brugerflade sprog</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Vis information i konsollen når fonte indlæses</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Vis ikke &apos;reklame-billedet&apos; ved opstart</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Vis versions information og afslut</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Brug højre til venstre dialog knap orden (d.v.s. Annullér/Nej/Ja i stedet for Ja/Nej/Annullér)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>filnavn</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Brug filnavn som sti for bruger-angivet præferencer</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>De installerede brugerflade-sprog for Scribus er følgende:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>For at tilsidesætte standard sprogvalg:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx eller scribus --lang xx, hvor xx er det sprog du ønsker at bruge.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribus Version</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Åben Kildetekst &apos;Desktop Publishing&apos;</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hjemmeside</translation>
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
        <translation>Problemer/Sager</translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation>Åbn et konsol windue</translation>
    </message>
    <message>
        <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
        <translation>Stop visning af reklame-billede ved opstart. Skriver en tom fil med navnet .neversplash i ~/.scribus.</translation>
    </message>
    <message>
        <source>Download a file from the Scribus website and show the latest available version.</source>
        <translation>Hent en fil fra Scribus Hjemmesiden og vis den nyest tilgængelige version.</translation>
    </message>
    <message>
        <source>Show location ICC profile information on console while starting</source>
        <translation>Vis placering af ICC profil-information i konsolen under start</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopiér her</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Flyt her</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fil: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Original PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Faktiske PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Sammenkædet tekst</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Tekst på en sti</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Afsnit: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tegn: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Udskriv: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Aktiv</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Deaktiveret</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Vis udskrift indstillinger</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF Indstillinger</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Placér i la&amp;g</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;veau</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Konve&amp;rtér til</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Sammenkæder tekstrammer</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Side %1 til %2</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Kan ikke konvertere et element, der i brug</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Elementet %1 er under redigering i den indbyggede tekstbehandler. Konvertering til kurver springes over for dette element</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Farverum: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukendt</translation>
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
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Indhold</translation>
    </message>
    <message>
        <source>Switches Color Management on or off</source>
        <translation>Kobler Farve-styring til og fra</translation>
    </message>
    <message>
        <source>Enables the Preview Mode</source>
        <translation>Aktiverer forhåndsvisning</translation>
    </message>
    <message>
        <source>Here you can select the visual appearance of the display
You can choose between normal and several color blindness forms</source>
        <translation>Her kan du vælge den visuelle fremtoning af skærmen.
Du kan vælge mellem normal og adskillige former for farveblindhed</translation>
    </message>
    <message>
        <source>Preview Mode</source>
        <translation>Visnings-tilstand</translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation>Indsæt nyere</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation>Duotone</translation>
    </message>
    <message>
        <source>Lines: </source>
        <translation>Linier: </translation>
    </message>
    <message>
        <source>Group%1</source>
        <translation>Gruppe%1</translation>
    </message>
    <message>
        <source>CMS is active. Therefore the color display may not match the perception by visually impaired</source>
        <translation>CMS er aktiv. Derfor vil farve-visningen måske ikke være korrekt for den synshæmmede</translation>
    </message>
    <message>
        <source>Enter Object Size</source>
        <translation>Indtast objekt-størrelse</translation>
    </message>
    <message>
        <source>No Image Loaded</source>
        <translation>Intet billede indlæst</translation>
    </message>
    <message>
        <source>You are trying to link a frame to itself.</source>
        <translation>Du prøver at lænke en ramme til sig selv.</translation>
    </message>
    <message>
        <source>You are trying to link a frame which is already linked.</source>
        <translation>Du prøver at lænke en ramme, som allerede er lænket.</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>has been changed since the last save.</source>
        <translation>er ændret siden sidste gang det blev gemt.</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Kassér</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Indlejret Python script understøttelse.</translation>
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
        <translation>Script fejl</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Hvis du kører en officiel script, så meddel det venligst til &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Denne besked er også i din udklipsholder. Brug Ctrl+V for at indsætte den i fejlreportering.</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Undersøg script</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Der opstod en intern fejl i forsøget på at udføre den kommando du indtastede. Detaljer blev udskrevet til stderr.</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Opsætning af Python plugin mislykkedes. Detaljer om fejl blev udskrevet til stderr.</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentation for:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Script</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> indeholder ikke nogen doc-streng!</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY);;All Files (*)</source>
        <translation>Python Scripts (*.py *.PY);;Alle filer (*)</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Scripter præferencer</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Aktivér udvidelses scripts</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Udvidelser</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Konsol</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Opstarts Script:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Fejl:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Kommentarer:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Nøgleord:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Tegn:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Tal:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>strenge:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Basis tekster:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Vælg farve</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Vælg...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>find opstarts Script</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Vis Sidevisninger</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Font</source>
        <translation>Font</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blok</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Højre</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Afsnits typografi</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Stregfarve</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation> - farvemætning</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Søg/erstat</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrér</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Tvunget</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Fontstørrelse</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Søgning afsluttet</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Søg efter:</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Fyldfarve</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation> - farvemætning</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Fonteffekter</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Erstat med:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Hele ord</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorér store/små bogstaver</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søg</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Erstat</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>E&amp;rstat alle</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Søgning afsluttet, fundet %1 der matcher</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Available Fields</source>
        <translation>Tilgængelige felter</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Markerede felter</translation>
    </message>
    <message>
        <source>Select Fields</source>
        <translation>Markér felter</translation>
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
        <source>Shade</source>
        <translation>Farvemætning</translation>
    </message>
    <message>
        <source>Other...</source>
        <translation>Andet...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Farvemætning:</translation>
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
        <translation>X-forskydning</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-forskydning</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Korte ord...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Korte ord</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation>Speciel plug-in for tilføjelse af  &quot;ikke brydbar&quot; mellemrum før eller efter såkaldte korte ord. Tilgængelig for følgende sprog: </translation>
    </message>
</context>
<context>
    <name>ShortcutWidget</name>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen taste</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Brugerdefineret taste</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Aflæs &amp;taste</translation>
    </message>
    <message>
        <source>No shortcut for the style</source>
        <translation>Ingen genvejstaste for stilen</translation>
    </message>
    <message>
        <source>Style has user defined shortcut</source>
        <translation>Stil har brugerdefineret genvejstaste</translation>
    </message>
    <message>
        <source>Assign a shortcut for the style</source>
        <translation>Fastsæt en genvejstaste for stilen</translation>
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
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Redigér typografier...</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Edit Styles</source>
        <translation>Redigér typografier</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny typografi</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi af %1</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikér</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Redigering af tekst</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Nuværende afsnit:</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tegn: </translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfiler (*.txt);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Ialt:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Afsnit: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord: </translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Ønsker du at gemme dine ændringer?</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nyt</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>Genindlæs tekst fra &amp;ramme</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Gem til Fil...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Indlæs fra Fil...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Gem &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Opdatér tekstramme og afslut</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Afslut &amp;uden at opdatere tekstramme</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Markér &amp;alt</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;p</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiér</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Opdatér tekstramme</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søg/erstat...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Redigér &amp;typografier...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Baggrund...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Vælg Font...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Ind&amp;stillinger</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Fontvisning...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Smart tekst markering</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Indsæt glyf...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Slet al tekst</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Indbygget tekstbehandler - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Ønsker du virkelig at miste alle ændringer?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Ønsker du virkelig at slette al din tekst?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Citat</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellemrum</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligatur</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mellemrum &amp;&amp; Skift</translation>
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
        <translation>Forskydning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Liniebredde</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation>Genvej</translation>
    </message>
    <message>
        <source>Apply all changes and exit edit mode</source>
        <translation>Anvend alle ændringer og afslut redigering</translation>
    </message>
    <message>
        <source>Edit styles</source>
        <translation>Redigér typografier</translation>
    </message>
    <message>
        <source>Name of the selected style</source>
        <translation>Navn på den valgte typografi</translation>
    </message>
    <message>
        <source>Reset all changes</source>
        <translation>Nulstil alle ændringer</translation>
    </message>
    <message>
        <source>Apply all changes</source>
        <translation>Anvend alle ændringer</translation>
    </message>
    <message>
        <source>Create a new style</source>
        <translation>Lav en ny typografi</translation>
    </message>
    <message>
        <source>Import styles from another document</source>
        <translation>Importér typografier fra et andet dokument</translation>
    </message>
    <message>
        <source>Clone selected style</source>
        <translation>Klon valgte typografi</translation>
    </message>
    <message>
        <source>Delete selected styles</source>
        <translation>Slet valgte typografi</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;nvend</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importér</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation>&amp;Klon</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Ny</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importér</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Redigér</translation>
    </message>
    <message>
        <source>Clone</source>
        <translation>klon</translation>
    </message>
    <message>
        <source>Send to Scrapbook</source>
        <translation>Placér i scrapbog</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slet</translation>
    </message>
    <message>
        <source>New %1</source>
        <translation>Ny %1</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Redigér</translation>
    </message>
    <message>
        <source>&amp;Done</source>
        <translation>&amp;Færdig</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>Denne taste-sekvens er allerede i brug</translation>
    </message>
    <message>
        <source>More than one style selected</source>
        <translation>Der er valgt mere end en typografi</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>dokumenter (*.sla *.sla.gz *.scd *.scd.gz);;Alle Filer (*)</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Small Caps</source>
        <translation>Små kapitæler</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Sænket skrift</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hævet skrift</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Alt som store bogstaver</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Understreg tekst. Tryk på knappen et øjeblik for at indstille stregbredde og forskydning.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Understreg kun ord. Tryk på knappen et øjeblik for at indstille stregbredde og forskydning.</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Overstreg. Tryk på knappen et øjeblik for at indstille stregbredde og forskydning.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Skyggetekst. Tryk på knappen et øjeblik for at indstille forskydning.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <comment>Text Style Selector</comment>
        <translation>Omrids. Tryk på knappen et øjeblik for at indstille omrids stregbredde.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Brug dokumentnavn som en forstavelse til afsnits typografier</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Spørg ikke igen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Tekst import indstillinger</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Ved aktivering overskrives eksisterende typografier i det aktive dokument</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sammenflet afsnits typografier</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Sammenflet afsnits typografier efter kvaliteter. Dette resulterer i færre ens afsnits typografier, vil bevare typografi kvaliteter,  selv om det originale dokuments typografier er navngivet forskelligt.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Sæt dokumentnavn foran afsnits typografiens navn i Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Gør disse indstillinger til standard værdier og spørg ikke igen ved import af et OpenOffice.org 1.x dokument.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Overskriv afsnits typografier</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>I starten</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>I slutningen</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Ikke vist</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Indholdsfortegnelse og indeks</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Indholdsfortegnelse</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rammen, som indholdsfortegnelsen bliver placeret i</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Sidenumre placeret:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Element kvalitetsnavn:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Element kvaliteten som bliver brugt på rammer som grundlag for at lave indgangene</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Placér indgangenes sidenumre i begyndelse eller slutningen af linien, eller slet ikke</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>List ikke-skrivbare indgange</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Medtag også rammer, som er sat til ikke at blive udskrevet</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Afsnits typografien som er brugt på indgangslinierne</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Afsnits typografi:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Bestemmelses-ramme:</translation>
    </message>
    <message>
        <source>Table of Contents %1</source>
        <translation>Indholdsfortegnelse %1</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Indholdsfortegnelse og indeks</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Indholdsfortegnelse</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Rammen, som indholdsfortegnelsen bliver placeret i</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Sidenumre placeret:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Element kvalitetsnavn:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Element kvaliteten som bliver brugt på rammer som grundlag for at lave indgangene</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Placér indgangenes sidenumre i begyndelse eller slutningen af linien, eller slet ikke</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>List ikke-skrivbare indgange</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Medtag også rammer, som er sat til ikke at blive udskrevet</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Afsnits typografien brugt på indgangslinierne</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Afsnits typografi:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Bestemmelses-ramme:</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Ignorér alle fejl</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Automatisk kontrol før udskrivning eller eksport</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Kontrollér for manglende glyffer</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Kontrollér for overløb i tekstrammer</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Kontrollér for manglende billeder</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Kontrollér billedopløsning</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Laveste tilladte opløsning</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Kontrollér for placerede PDF-filer</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Kontrollér PDF kommentarer og felter</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Tilføj profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Fjern profil</translation>
    </message>
    <message>
        <source>Highest allowed resolution</source>
        <translation>Højeste tilladte opløsning</translation>
    </message>
    <message>
        <source>Check for GIF images</source>
        <translation>Kontrollér for GIF billeder</translation>
    </message>
    <message>
        <source>Ignore non-printable Layers</source>
        <translation>Ignorér ikke-skrivbare lag</translation>
    </message>
    <message>
        <source>Check for items not on a page</source>
        <translation>Kontrollér for elementer der ikke er på siden</translation>
    </message>
    <message>
        <source>Check for used transparencies</source>
        <translation>Kontrollér for brugte gennemsigtigheder</translation>
    </message>
</context>
<context>
    <name>TabDisplay</name>
    <message>
        <source>Color for paper</source>
        <translation>Farve på papir</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vis området uden for margener i margenfarve</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Slår visningen  af lænkede rammer til eller fra.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Vis ikke-printbare tegn som for eksempel afsnits-markører i tekstrammer</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Slår visning af rammer til eller fra</translation>
    </message>
    <message>
        <source>Turns the display of layer indicators on or off</source>
        <translation>Slår visning af lag-indikatorer til eller fra</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Slår visning af billeder til eller fra</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definerer mængden af plads til venstre for dokument-området tilgængeligt som et midlertidigt arbejdssted til at lave og ændre elementer og derefter trække dem ind på den aktive side</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definerer mængden af plads til højre for dokument-området tilgængeligt som et midlertidigt arbejdssted til at lave og ændre elementer og derefter trække dem ind på den aktive side</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definerer mængden af plads over dokument-området tilgængeligt som et midlertidigt arbejdssted til at lave og ændre elementer og derefter trække dem ind på den aktive side</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Definerer mængden af plads under dokument-området tilgængeligt som et midlertidigt arbejdssted til at lave og ændre elementer og derefter trække dem ind på den aktive side</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Vælg standard zoom-niveau</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Placer en linial mod din skærm og træk glideren for at sætte zoom-niveauet, så Scribus viser dine sider og objekter på dem i den rigtige størrelse</translation>
    </message>
</context>
<context>
    <name>TabDisplayBase</name>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Side visning</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Vis billeder</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Vis Text kontrol tegn</translation>
    </message>
    <message>
        <source>Rulers Relative to Page</source>
        <translation>Linialer relativt til side</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Vis tekstkæder</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Vis rammer</translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation>Vis lag-indikatorer</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vis område, der ikke kan &amp;udskrives i margenfarve</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Bleed Area</source>
        <translation>Vis område for Tryk til kant</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Plads til noter</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Højre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bund:</translation>
    </message>
    <message>
        <source>Gaps Between Pages</source>
        <translation>Afstand mellem sider</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Horisontalt:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Vertikalt:</translation>
    </message>
    <message>
        <source>Adjust Display Size</source>
        <translation>Indstil størrelse af skærmvisning</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Indstil hvor meget 100% visning fylder på skærmen.</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
    <message>
        <source>Pages:</source>
        <translation>Sider:</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Fyldfarve:</translation>
    </message>
    <message>
        <source>Selected Page Border:</source>
        <translation>Valgte sideramme:</translation>
    </message>
    <message>
        <source>Frames</source>
        <translation>Rammer</translation>
    </message>
    <message>
        <source>Locked:</source>
        <translation>Låst:</translation>
    </message>
    <message>
        <source>Normal:</source>
        <translation>Normalt:</translation>
    </message>
    <message>
        <source>Selected:</source>
        <translation>Valgte:</translation>
    </message>
    <message>
        <source>Linked:</source>
        <translation>Sammenkædet:</translation>
    </message>
    <message>
        <source>Grouped:</source>
        <translation>Grupperet:</translation>
    </message>
    <message>
        <source>Annotation:</source>
        <translation>Kommentar:</translation>
    </message>
    <message>
        <source>Text:</source>
        <translation>Tekst:</translation>
    </message>
    <message>
        <source>Control Characters:</source>
        <translation>Kontrol-tegn:</translation>
    </message>
    <message>
        <source>TabDisplayBase</source>
        <translation>TabDisplayBase</translation>
    </message>
    <message>
        <source>Scale%</source>
        <translation>Skalering%</translation>
    </message>
</context>
<context>
    <name>TabDocument</name>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portræt</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskab</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Enheder:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Højde:</translation>
    </message>
    <message>
        <source>Apply settings to:</source>
        <translation>Anvend indstillinger på:</translation>
    </message>
    <message>
        <source>All Document Pages</source>
        <translation>Alle dokumentsider</translation>
    </message>
    <message>
        <source>All Master Pages</source>
        <translation>Alle mastersider</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margen hjælpelinier</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Gem automatisk</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>I&amp;nterval:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Fortryd/Lav igen</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Handlings historie længde</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Bredde af dokument-sider, redigérbar hvis du har valgt en brugerdefineret sidestørrelse</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Højde af dokument-sider, redigérbar hvis du har valgt en brugerdefineret sidestørrelse</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standard sidestørrelse, enten en standard-størrelse eller en bruger-defineret</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standard retning på dokumentsider</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard målenhed for dokument-redigering</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Når dette er valgt, så gemmer Scribus en backup-kopi af din fil med .bak tilføjet filnavnet, hver gang det valgte tids-interval udløber</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Tid-interval for den automatiske gemning</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Sæt antallet af handlings-historien i trin. Hvis værdien er 0, vil et uendelig antal gemmes.</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Anvend side-størrelse ændringerne på alle eksisterende sider i dokumentet</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing master pages in the document</source>
        <translation>Anvend side-størrelse ændringer på alle eksisterende master-sider i dokumentet</translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidget</name>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Find Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Find dit billedprogram</translation>
    </message>
    <message>
        <source>Locate your web browser</source>
        <translation>Vælg din netlæser</translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidgetBase</name>
    <message>
        <source>External Tools</source>
        <translation>Eksterne værktøjer</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>PostScript fortolker</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Navn på program:</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Tilføj stien til Ghostscript fortolkeren. Vær venligst opmærksom på, at du på Windows skal bruge programmet der hedder gswin32c.exe - IKKE gswin32.exe. Ellers kan det måske forårsage at Windows hænger, når Scribus startes.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Change..</source>
        <translation>&amp;Vælg..</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Antialias &amp;Tekst</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Antialias tekst for EPS og PDF skærmvisning</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Antialias &amp;Grafik</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Antialias grafik for EPS og PDF skærmvisning</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Opløsning:</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Program til Billedbehandling</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Navn på &amp;program:</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for graphics editor. If you use gimp and your distribution includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Placering af grafikprogram i filsystemet. Hvis du bruger gimp og din distribution inkluderer &apos;gimp-remote&apos;, så anbefaler vi, at du bruger dette, da det tillader dig at redigere billedet i en allerede kørende gimp.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>Web Browser</source>
        <translation>Netlæser</translation>
    </message>
    <message>
        <source>Web Browser to launch with links from the Help system</source>
        <translation>Netlæser, som skal startes ved net-henvisninger fra hjælpe-systemet</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for your web browser. This is used for external links from the Help system.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Placering af netlæser i filsystemet. Dette bruges for eksterne henvisninger fra hjælpe-systemet.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Rescan</source>
        <translation>&amp;Søg igen</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Rescan for the external tools if they do not exist in the already specified location</source>
        <translation>Søg igen efter eksterne værktøjer hvis de ikke eksisterer på det specificerede sted</translation>
    </message>
</context>
<context>
    <name>TabGeneral</name>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Vælg standardværdi for det sprog Scribus skal køres med. Hvis feltet er tomt, vælges sproget fra miljø-variablerne. Du kan stadig tilsidesætte dette ved en kommandolinie option under start af Scribus</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Antal nyeste dokumenter, som vises i fil-menuen</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Antal linier der rulles for hver bevægelse af muse-hjulet</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Vælg standard vinduesdekoration og udseende. Scribus arver alle tilgængelige KDE eller Qt temaer, hvis Qt er konfigureret til at søge KDE plugins.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Standard fontstørrelse for menuer og vinduer</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Standard fontstørrelse for værktøjsvinduer</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standard dokumentmappe</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Standard ICC-profilmappe. Denne kan ikke ændres, når der er åbnede dokumenter. Som standard vil Scribus se i system-mapperne under Mac OSX og Windows. På Linux og Unix, søger Scribus i $HOME/.color/icc, /usr/share/color/icc og /usr/local/share/color/icc </translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standard Scripter scriptmappe</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Ekstra mappe for dokument skabeloner</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
</context>
<context>
    <name>TabGeneralBase</name>
    <message>
        <source>User Interface</source>
        <translation>Bruger grænseflade</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprog:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source> ms</source>
        <translation> ms</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Time before a Move or Resize starts:</source>
        <translation>Tid før en flytning eller ændring af størrelse begynder:</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Fontstørrelse (&amp;Paletter):</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>&amp;Fontstørrelse (Menuer):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Hjul &amp;hop:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nyeste dokumenter:</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Vis opstarts-vindue</translation>
    </message>
    <message>
        <source>Show Splashscreen on Startup</source>
        <translation>Vis reklame-billede ved opstart</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Stier</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Væ&amp;lg...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Document &amp;Templates:</source>
        <translation>Dokument &amp;skabeloner:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenter:</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-profiler:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Væ&amp;lg...</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>Alt+H</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>TabGeneralBase</source>
        <translation>TabGenerelBase</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Fælles indstillinger</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Placeres i dokumenter</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>I baggrunden</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>I forgrunden</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Klæbning</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Klæbe afstand:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Gribe radius:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Vis Hjælpelinier</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farve:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Vis margener</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Vis side-gitter</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Hoved gitter</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Afstand:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Gitter</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Vis basislinie-gitter</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Basislinie instillinger</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Basislinie &amp;gitter:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Basislinie forsk&amp;ydning:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Hjælpelinier er ikke synlige gennem objekter på siden</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Hjælpelinier er synlige over alle objekter på siden</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Afstand mellem linierne</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Afstand mellem hovedlinierne</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Den afstand indenfor hvor et objekt vil placere sig på en hjælpelinie</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Størrelsen af det område, hvor du kan tage om et objekts håndtag</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Farve på linierne</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Farve på hovedlinierne</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Farve på de hjælpelinier du indsætter</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farve på margenlinier</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Farve på basislinie-gitter</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Slår basislinie-gitter til eller fra</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Afstand mellem linier i basislinie-gitteret</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Afstand fra toppen af siden til den første basislinie</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Slår gitterlinierne til eller fra</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Slår hjælpelinierne til eller fra</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Slår margenerne til eller fra </translation>
    </message>
    <message>
        <source>px</source>
        <translation>px</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Taste sæt XML Filer (*.ksxml)</translation>
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
        <translation>Denne taste-sekvens er allerede i brug</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidgetBase</name>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tastatur genveje</translation>
    </message>
    <message>
        <source>Search:</source>
        <translation>Søg:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation>Genvej</translation>
    </message>
    <message>
        <source>Shortcut for Selected Action</source>
        <translation>Genvej for valgte handling</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen taste</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Brugerdefineret taste</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Aflæs &amp;taste</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>CTRL+ALT+SHIFT+W</source>
        <translation>CTRL+ALT+SHIFT+W</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Indlæsbar sæt af genveje</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Sæt af tastaturgenveje tilgængelig for indlæsning</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Indlæs</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Indlæs det valgte sæt af genveje</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importér...</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importér et sæt af genveje ind i den aktuelle konfiguration</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Exportér...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Eksportér de aktuelle genveje til en importérbar fil</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Genindlæs standard Scribus genveje</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Håndtér tabulatorer</translation>
    </message>
</context>
<context>
    <name>TabMiscellaneousBase</name>
    <message>
        <source>Form1</source>
        <translation>Form1</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Spørg altid før fonte bliver erstattet, når et dokument indlæses</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Visning af aktuelle afsnits typografi er synlig under redigering af typografier</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Brug altid standard Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Antal afsnit:</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Eksport område</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>All&amp;e sider</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Vælg si&amp;der</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>fil indstillinger</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Kompatibilitet:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Indbinding:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Venstre margen</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Højre margen</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Lav miniat&amp;urer</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Gem k&amp;ædede tekstrammer som PDF-indlæg</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Medtag &amp;bogmærker</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Opløsning for EPS grafik:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>Kom&amp;primér tekst og vektor grafik</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maksimum</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Høj</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Medium</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Lav</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimum</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>Ge&amp;nerelt</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Indlejrer</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Tilgængelige fonte:</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Fonte til indlejring:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Fonte</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Anvend pr&amp;æsentationseffekter</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Vis side&amp;miniaturer</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>Varigh&amp;ed af visning:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Varighe&amp;d af effekt:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Effektt&amp;ype:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Bevægelige linier:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>F&amp;ra:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>Re&amp;tning:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sec</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Ingen effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Gardiner</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Boks</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Opløs</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Opdel</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Udvisk</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontalt</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikalt</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Inderside</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Yderside</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Venstre til højre</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>top til bund</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Bund til top</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Højre til venstre</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>top-venstre til bund-højre</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Anvend effekt p&amp;å alle sider</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Brug kr&amp;yptering</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Adgangskode</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>Br&amp;uger:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Ejer:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Tillad u&amp;dskrivning af dokument</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Tillad &amp;ændring af dokument</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Tillad ko&amp;piering af tekst og grafik</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Tillad at der tilf&amp;øjes kommentarer og felter</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>Sikker&amp;hed</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Udskrift &amp;beregnet for:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Skærm / Net</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Printer</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråtone</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>Brug br&amp;uger indstillinger for gengivelse</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Indstillinger for gengivelse</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>F&amp;rekvens:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>Vink&amp;el:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Prik funktion:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Enkel prik</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Massive farver:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Brug ICC profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Farvetilpasningsmetode:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Opfattelsesmæssig</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolorimetri</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolorimetri</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Billeder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Brug ikke indlejrede ICC-profiler</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Far&amp;ve</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Hensigt med PDF/X-3 udskrift</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Info streng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Udskrivnings &amp;profil:</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Viser en miniature af hver side, som er vist her ovenover.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Skærm-effektens type.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Retning på bevægelige liner for opdelt og gardin effekter.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Start position for boks og opdelt effekter.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Retning på glitter eller udvisk effekter.
</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Anvend den valgte effekt på alle sider.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Eksportér alle sider til PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Eksportér nogle af siderne til PDF</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Lav PDF-indlæg, som er anvendelig for navigering i sammenkædede indlæg i en PDF.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Punkter pr tomme) for eksport af billede.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Vælg en adgangskode for læsere af PDF-Filen.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Tillad udskrivning af PDF-Filen. Udskrivning er ikke muligt uden afkryds.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Tillad ændring af PDF-Filen. Ændring af PDF-Filen er ikke muligt uden afkryds.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Indlejr en farve-profil for massive farver</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Farve-profil for massive farver</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Farvetilpasningsmetode for massive farver</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Indlejr en farve-profil for billeder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Brug ikke farve-profiler, som er indlejret i kilde-billeder</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Farve-profil for billeder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Farvetilpasningsmetode for billeder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Uddata-profil for udskrivning. Hvis det er muligt, så få hjælp fra din trykker til valg af profil.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Afstand for tryk til kant fra toppen af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Afstand for tryk til kant fra bunden af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Afstand for tryk til kant fra venstre af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Afstand for tryk til kant fra højre af den fysiske side</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Spejl side(r) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Spejl side(r) vertikalt</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertér spot-farver til proces-farver</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Aktiverer konvertering af spot-farver til sammensatte farver. Medmindre du planlægger at trykke med spot-farver hos et kommercielt trykkeri, er det sansynligvis bedst at have aktiveret.</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Komprimering Kvali&amp;tet:</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Tillad kopiering af tekst eller grafik fra PDF-Filen. Tekst og grafik kan ikke kopieres uden afkryds.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Tillad at kommentarer og felter kan tilføjes PDF-Filen. Redigering af kommentarer og felter er ikke muligt uden afkryds.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Indlejr fonte i PDF-filen. Dette vil bevare opsætning og udseende af dokumentet.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Den tid effekten varer. En kortere tid vil øge farten af effekten, en længere tid vil nedsætte tempoet.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Indsæt en liste af tegn adskilt med komma, hvor et tegn kan være * for alle siderne, 1-5 for en række sider eller et enkelt sidenummer.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Bestemmer sidernes indbinding i PDF. Behold standardværdien - venstre, medmindre du ved, at du behøver at ændre det.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Laver miniaturer af hver side i PDF-filen. Nogle PDF-læsere kan bruge disse til navigering.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Indlejr bogmærkerne, som du lavede i dit dokument. Disse er anvendelig for navigering i lange PDF dokumenter.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Opløsning af eksporteret tekst og vektorgrafik. Dette påvirker ikke opløsningen af bitmap billeder som fotografier.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Aktiverer Tabsfri komprimering af tekst og grafik. Medmindre du har grund til andet, så lad dette være aktiveret, Det reducerer størrelsen af PDF-filen.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Aktivér sikkerheds-unktionerne i det eksporterede PDF-dokument. Hvis du valgte PDF 1.3, så vil PDF være beskyttet af 40 bit kryptering. Hvis du valgte PDF 1.4, så vil PDF være beskyttet af 128 bit kryptering. OBS: PDF kryptering er ikke så pålidelig som GPG eller PGP kryptering og har desuden en del begrænsninger.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Vælg en ejer adgangskode, som aktiverer eller deaktiverer alle sikkerheds-egenskaber i det eksporterede PDF-dokument</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Dette er en anvanceret indstilling, som ikke er aktiv som standard. Denne bør kun aktiveres, når trykkeriet specifikt har anmodet om det og opgivet de nøjagtige detaljer om indstillingerne. Ellers vil den eksporterede PDF fil ikke kunne udskrives ordentligt og ikke portabel på tværs af systemer.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Obligatorisk streng for PDF/X-3. Skal udfyldes for at overholde reglerne for PDF/X-3. Vi anbefaler at bruge titlen på dokumentet. </translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Medtag &amp;lag</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Komprimering &amp;Metode:</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Instillinger for visning</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Enkelt side</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Kontinuerligt</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Dobbelt side venstre</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Dobbelt side højre</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Visuelt udseende</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Brug PDF-fremvisers standard-indstillinger</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Brug fuldskærm</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Vis fane for bogmærker</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Vis miniaturer</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Vis fane for lag</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Skjul fremvisers værktøjsbjælke</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Skjul fremvisers menubjælke</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Zoom sider, så de passer til fremviser-vinduet</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Specielle handlinger</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Ingen script</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>PDF-fremviser</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Lossy - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Tabsfri -Zip</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Afskær ved side margener</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Billed komprimeringsmetode</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript der skal udføres
når PDF dokument åbnes:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Aktiverer præsentations-effekter, når der bruges Adobe&amp;#174; Reader&amp;#174; og andre PDF læsere som understøtter dette i fuldskærm tilstand.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Lag i dokumentet bliver eksporteret til PDF-filen. Kun tilgængelig hvis du har valgt PDF 1.5.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Farvemodel for uddata af PDF-dokumentet. Vælg Skærm/Web for PDF-filer, som bruges til visning på skærm og til udskrivning på almindelige Blæk-printere. Vælg Printer, når der udskrives til en rigtig 4 farvet CMYK-printer. Vælg gråskala når du ønsker gråskala PDF.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Vis ikke objekter udenfor margener i den eksporterede fil</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Den tid siden er vist, før præsentationen starter på den valgte side. Værdien 0 deaktiverer automatisk sideskift.</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Gennemtving overprint tilstand</translation>
    </message>
    <message>
        <source>Printer Marks</source>
        <translation>Pasmærker</translation>
    </message>
    <message>
        <source>Crop Marks</source>
        <translation>Skæremærker</translation>
    </message>
    <message>
        <source>Bleed Marks</source>
        <translation>Mærker for tryk til kant</translation>
    </message>
    <message>
        <source>Registration Marks</source>
        <translation>Pasmærker</translation>
    </message>
    <message>
        <source>Color Bars</source>
        <translation>Farveklodser</translation>
    </message>
    <message>
        <source>Page Information</source>
        <translation>Side-information</translation>
    </message>
    <message>
        <source>Offset:</source>
        <translation>Forskydning:</translation>
    </message>
    <message>
        <source>Bleed Settings</source>
        <translation>Indstillinger for tryk til kant</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation>Top:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation>Bund:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation>Venstre:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Højre:</translation>
    </message>
    <message>
        <source>Use Document Bleeds</source>
        <translation>Brug dokument tryk til kant</translation>
    </message>
    <message>
        <source>Pre-Press</source>
        <translation>Prepress</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.&lt;br/&gt;The default is &lt;b&gt;PDF 1.3&lt;/b&gt; which gives the widest compatibility.&lt;br/&gt;Choose &lt;b&gt;PDF 1.4&lt;/b&gt; if your file uses features such as transparency or you require 128 bit encryption.&lt;br/&gt;&lt;b&gt;PDF 1.5&lt;/b&gt; is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/&gt;&lt;b&gt;PDF/X-3&lt;/b&gt; is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Bestemmer PDF kompatibiliteten.&lt;br/&gt;Standarden er &lt;b&gt;PDF 1.3&lt;/b&gt;, som giver den største kompatibilitet.&lt;br/&gt;Vælg &lt;b&gt;PDF 1.4&lt;/b&gt;, hvis filen har egenskaber som f.eks. gennemsigtighed, eller hvis du ønsker 128 bit kryptering.&lt;br/&gt;&lt;b&gt;PDF 1.5&lt;/b&gt; er nødvendig, når du ønsker at bevare objekter i separate lag inden i PDF filen.&lt;br/&gt;&lt;b&gt;PDF/X-3&lt;/b&gt; bruges til eksport til PDF, når du ønsker farvestyring af RGB for kommerciel udskrivning og kan vælges, når du har aktiveret farvestyring. Bruges kun når dit trykkeri ønsker det, eller i nogle tilfælde, når der udskrives til en 4 farve digital farve laserprinter.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Komprimerings-metode for billeder. Tillader Scribus automatisk at vælge den bedste metode. ZIP er tabsløs og god til billeder med massive farver. JPEG er bedre til at lave PDF-filerne mindre, når der er mange fotografier (muligvis med et lille tab af billedkvalitet). Lad indstillingen blive ved automatisk, medmindre du har brug for specielle komprimerings-indstillinger.</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>Komprimering kvalitets-niveauer for komprimeringsmetoder med tab: Minimum (25%), Lav (50%), Medium (75%), Høj (85%), Maksimum (95%). Bemærk at kvalitetsniveauet bestemmer ikke direkte størrelsen på det resulterende billede - både størrelse og kvalitetstab varierer fra billede til billede ved et givet kvalitetsniveau. Selvom du vælger Maksimum, er der altid nogen kvalitetstab ved jpeg.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Aktiverer global overprint tilstand for dette dokument, tilsidesætter objekt indstillinger</translation>
    </message>
    <message>
        <source>Inside:</source>
        <translation>Inderside:</translation>
    </message>
    <message>
        <source>Outside:</source>
        <translation>Yderside:</translation>
    </message>
    <message>
        <source>&amp;Embed all</source>
        <translation>&amp;Indlejr alle</translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation>Fonte der skal laves til kurver:</translation>
    </message>
    <message>
        <source>&amp;Outline all</source>
        <translation>Lav &amp;kurver af alle</translation>
    </message>
    <message>
        <source>Convert all glyphs in the document to outlines.</source>
        <translation>Konvertér alle glyffer i dokumentet til kurver.</translation>
    </message>
    <message>
        <source>Maximum Image Resolution:</source>
        <translation>Maksimum  billed-opløsning:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Dokument layout</translation>
    </message>
    <message>
        <source>Show the document in single page mode</source>
        <translation>Vis dokumentet i enkelt-side tilstand</translation>
    </message>
    <message>
        <source>Show the document in single page mode with the pages displayed continuously end to end like a scroll</source>
        <translation>Vis dokumentet i enkelt-side tilstand med siderne vist kontinuerligt, ende mod ende, som en rulle</translation>
    </message>
    <message>
        <source>Show the document with facing pages, starting with the first page displayed on the left</source>
        <translation>Vis dokumentet med modstående sider, startende med den første side vist til venstre</translation>
    </message>
    <message>
        <source>Show the document with facing pages, starting with the first page displayed on the right</source>
        <translation>Vis dokumentet med modstående sider, startende med den første side vist til højre</translation>
    </message>
    <message>
        <source>Use the viewer&apos;s defaults or the user&apos;s preferences if set differently from the viewer defaults</source>
        <translation>Brug fremviserens standard-værdier eller brugerens præferencer, hvis de er sat forskelligt fra fremviserens standardværdier</translation>
    </message>
    <message>
        <source>Enables viewing the document in full screen</source>
        <translation>Aktiverer visning af dokumentet i fuld-skærm</translation>
    </message>
    <message>
        <source>Display the bookmarks upon opening</source>
        <translation>Vis bogmærkerne ved åbning</translation>
    </message>
    <message>
        <source>Display the page thumbnails upon opening</source>
        <translation>Vis side-miniaturerne ved åbning</translation>
    </message>
    <message>
        <source>Forces the displaying of layers. Useful only for PDF 1.5+.</source>
        <translation>Gennemtving visning af lag. Kun brugbart for PDF 1.5+.</translation>
    </message>
    <message>
        <source>Hides the Tool Bar which has selection and other editing capabilities</source>
        <translation>Skjuler værktøjs-bjælken, som har markering og andre redigerings muligheder</translation>
    </message>
    <message>
        <source>Hides the Menu Bar for the viewer, the PDF will display in a plain window. </source>
        <translation>Skjuler menu-bjælken for fremviseren, PDF-filen vil blive vist i et almindeligt vindue. </translation>
    </message>
    <message>
        <source>Fit the document page or pages to the available space in the viewer window.</source>
        <translation>Tilpas dokument side eller sider til den tilgængelige plads i fremviserens vindue.</translation>
    </message>
    <message>
        <source>Limits the resolution of your bitmap images to the selected DPI. Images with a lower resolution will be left untouched. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Begrænser opløsningen af dit bitmap billede til valgte DPI. Billeder med en lavere opløsning vil forblive urørt. Hvis dette ikke vælges, så bruges billedernes oprindelige opløsning. Aktivering af dette vil øget brug af hukommelse og forlænge eksport-tiden.</translation>
    </message>
    <message>
        <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing.</source>
        <translation>Dette laver skæremærker i PDF, som viser, hvor papiret skal skæres eller trimmes efter trykning.</translation>
    </message>
    <message>
        <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
        <translation>Dette laver tryk til kant mærker, som er vist med  _._ og viser hvormeget, der flyder ud over kant </translation>
    </message>
    <message>
        <source>Add registration marks which are added to each separation</source>
        <translation>Tilføj pasmærker, som tilføjes hver separation</translation>
    </message>
    <message>
        <source>Add color calibration bars</source>
        <translation>Tilføj farve-kalibrerings søjler</translation>
    </message>
    <message>
        <source>Add document information which includes the document title and page numbers</source>
        <translation>Tilføj dokument information, som inkluderer dokument titel og side numre</translation>
    </message>
    <message>
        <source>Indicate the distance offset for the registration marks</source>
        <translation>Indikerer afstands-forskydning af pasmærker</translation>
    </message>
    <message>
        <source>Use the existing bleed settings from the document preferences</source>
        <translation>Brug de eksisterende instillinger for tryk til kant fra dokumentets præferencer</translation>
    </message>
</context>
<context>
    <name>TabPrinter</name>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Afstand for tryk til kant fra toppen af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Afstand for tryk til kant fra bunden af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Afstand for tryk til kant fra venstre af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Afstand for tryk til kant fra højre af den fysiske side</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Vis ikke objekter udenfor margener på den udskrevne side</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Brug en alternativ printer håndtering, som f.eks. kprinter eller gtklp, for at kunne få flere printerindstillinger</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Sætter PostScript niveau.
Niveau 1 eller 2 kan give meget store filer</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En måde at ændre nogle af gråtonerne, som er sammensat af cyan, gul og magenta, så der i stedet for disse farver bruges sort. RUF påvirker mest de dele af billedet, som er neutrale og/eller mørke i farven, det, som er tættets på gråt. Brug af denne metode kan forbedre udskrivningen af nogle billeder og nogen eksperimenteren og testning er nødvendigt fra billede til billede. RUF formindsker muligheden for overmætning med CMY farverne.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Aktiverer konvertering af spot-farver til sammensatte farver. Medmindre du planlægger at trykke med spot-farver hos et kommercielt trykkeri, er det sansynligvis bedst at have aktiveret.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation>Aktiverer global overprint tilstand for dette dokument, tilsidesætter objekt indstillinger</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Tillader dig at indlejre ICC profiler i udskrift-data når farvestyring er aktiveret </translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Dette gør dig i stand til direkte at sætte medie størrelse i PostScript-filen. Anbefales ikke medmindre det kræves af dit trykkeri.</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
</context>
<context>
    <name>TabPrinterBase</name>
    <message>
        <source>Print Destination</source>
        <translation>Udskriv til</translation>
    </message>
    <message>
        <source>Alternative Printer Command</source>
        <translation>Alternativ udskrivnings-kommando</translation>
    </message>
    <message>
        <source>Command:</source>
        <translation>Kommando:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>Postscript Options</source>
        <translation>Postscript indstillinger</translation>
    </message>
    <message>
        <source>Level 1</source>
        <translation>Niveau 1</translation>
    </message>
    <message>
        <source>Level 2</source>
        <translation>Niveau 3</translation>
    </message>
    <message>
        <source>Level 3</source>
        <translation>Niveau 3</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Udskriv i farve hvis det er tilgængeligt</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Udskriv i gråtoner</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Spejl side(r) horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Spejl side(r) vertikalt</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Angiv medie størrelse</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Afskær ved side margener</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Anvend Reduktion af underliggende farve</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertér spot-farver til proces-farver</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation>Gennemtving overprint tilstand</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Anvend ICC profiler</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Udskriv normalt</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Udskriv separationer</translation>
    </message>
    <message>
        <source>Marks &amp;&amp; Bleeds</source>
        <translation>Mærker &amp;&amp; Tryk til kant</translation>
    </message>
    <message>
        <source>Printer Marks</source>
        <translation>Printermærker</translation>
    </message>
    <message>
        <source>Crop Marks</source>
        <translation>Skæremærker</translation>
    </message>
    <message>
        <source>Bleed Marks</source>
        <translation>Tryk til kant mærker</translation>
    </message>
    <message>
        <source>Registration Marks</source>
        <translation>Pasmærker</translation>
    </message>
    <message>
        <source>Offset:</source>
        <translation>Forskydning:</translation>
    </message>
    <message>
        <source>Color Bars</source>
        <translation>Farveklodser</translation>
    </message>
    <message>
        <source>Bleed Settings</source>
        <translation>Indstillinger for tryk til kant</translation>
    </message>
    <message>
        <source>Top:</source>
        <translation>Top:</translation>
    </message>
    <message>
        <source>Bottom:</source>
        <translation>Bund:</translation>
    </message>
    <message>
        <source>Left:</source>
        <translation>Venstre:</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Højre:</translation>
    </message>
    <message>
        <source>TabPrinterBase</source>
        <translation>TabPrinterBase</translation>
    </message>
    <message>
        <source>This creates crop marks in the PDF indicating where the paper should be cut or trimmed after printing</source>
        <translation>Dette laver skæremærker i PDF, som viser, hvor papiret skal skæres eller trimmes efter trykning</translation>
    </message>
    <message>
        <source>This creates bleed marks which are indicated by  _ . _ and show the bleed limit</source>
        <translation>Dette laver tryk til kant mærker, som er vist med  _._ og viser hvormeget, der flyder ud over kant </translation>
    </message>
    <message>
        <source>Add registration marks which are added to each separation</source>
        <translation>Tilføj pasmærker, som tilføjes hver separation</translation>
    </message>
    <message>
        <source>ndicate the distance offset for the registration marks</source>
        <translation>Indikerer afstands-forskydning af pasmærker</translation>
    </message>
    <message>
        <source>Add color calibration bars</source>
        <translation>Tilføj farve-kalibrerings søjler</translation>
    </message>
</context>
<context>
    <name>TabScrapbookBase</name>
    <message>
        <source>Send Copied Items Automatically to Scrapbook</source>
        <translation>Send automatisk kopierede elementer til scrapbogen</translation>
    </message>
    <message>
        <source>This enables the scrapbook to be used an extension to the copy/paste buffers. Simply copying an object or grouped object will send this to the Scrapbook automatically</source>
        <translation>Dette gør det muligt at bruge scrapbogen som en udvidelse til kopi/indsæt bufferne. Bare ved at kopiere et objekt eller grupperet objekt, vil automatisk sende dette til scrapbogen</translation>
    </message>
    <message>
        <source>Keep Copied Items Permanently Across Sessions</source>
        <translation>Behold kopierede elementer permanent til næste sessioner</translation>
    </message>
    <message>
        <source>This enables copied items to be kept permanently in the scrapbook.</source>
        <translation>Dette aktiverer kopierede elementer til at blive permanent i scrapbogen.</translation>
    </message>
    <message>
        <source>Number of Copied Items to Keep in Scrapbook:</source>
        <translation>Antal kopierede elementer, der skal beholdes i scrapbogen:</translation>
    </message>
    <message>
        <source>The minimum number is 1; the maximum us 100.</source>
        <translation>Det mindste antal er 1; det største er 100.</translation>
    </message>
    <message>
        <source>TabScrapbookBase</source>
        <translation>TabScrapbookBase</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Font:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Størrelse:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Fyldfarve:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Stregfarve:</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Tab udfyldnings-tegn:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Tabulerings bredde:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Afsta&amp;nd:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Vævet silkepyjamas byttet med blå kvarts</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Liniefarve:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>Farvem&amp;ætning:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Fyldfarve:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>Farvem&amp;ætning:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Liniestil:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linie&amp;bredde:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Linies&amp;til:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Pile:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Start:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Slutning:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Fri skalering</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Horisontal skalering:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Vertikal skalering:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Skalér billede til rammestørrelse</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>&amp;Behold bredde/højde forhold</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>F&amp;yldfarve:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Brug indlejret klipnings-sti</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Visning på skærm</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Vis i fuld opløsning</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Vis i normal opløsning</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Vis i lav opløsning</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Mak&amp;simum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Trin:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Tekstramme - egenskaber</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Billedramme - egenskaber</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Tegn forskellige figurer - egenskaber</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Forstørrelses niveau - egenskaber</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Tegn linier - egenskaber</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Tegn polygoner - egenskaber</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Font i nye tekstrammer</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Størrelse af font i nye tekstrammer</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Farve på font</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Antal kolonner i en tekstramme</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Afstand mellem kolonner i tekstramme</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Font-prøve</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Billedramme tillader billeder at blive skaleret til en hvilken som helst størrelse</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Horisontal skalering af billeder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Vertikal skalering af billeder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Behold den samme skalering både horisontalt og vertikalt</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Billeder i billedrammer er skaleret til rammens størrelse</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automatisk skalerede billeder beholder deres originale proportioner</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Billedrammers fyldfarve</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Fyldfarvens mætningsgrad</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Figurers liniefarve</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Liniefarvens mætningsgrad</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Figurers fyldfarve</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Figurers liniestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Figurers liniebredde</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Mindste tilladte forstørrelse</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Største tilladte forstørrelse</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Ændring af forstørrelsen for hver zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Liniernes farve</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Farvemætning</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Liniernes stil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Bredde på linie</translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Tekstfarve:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Farvemætning:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Tekststrøg:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Prik</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Bindestreg</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Underlinie</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brugerdefineret</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Shapes</source>
        <translation>Figurer</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation>Linier</translation>
    </message>
    <message>
        <source>Images</source>
        <translation>Billeder</translation>
    </message>
    <message>
        <source>Regular Polygons</source>
        <translation>Almindelige polygoner</translation>
    </message>
    <message>
        <source>Zoom</source>
        <translation>Zoom</translation>
    </message>
    <message>
        <source>Rotation Tool</source>
        <translation>Rotations værktøj</translation>
    </message>
    <message>
        <source>Constrain to:</source>
        <translation>Begrænse til:</translation>
    </message>
    <message>
        <source>Other Properties</source>
        <translation>Andre egenskaber</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Miscellaneous Settings</source>
        <translation>Diverse indstillinger</translation>
    </message>
    <message>
        <source>Item Duplicate</source>
        <translation>Element kopiering</translation>
    </message>
    <message>
        <source>X Displacement</source>
        <translation>X Forskydning</translation>
    </message>
    <message>
        <source>Y Displacement</source>
        <translation>Y Forskydning</translation>
    </message>
    <message>
        <source>Horizontal displacement of page items</source>
        <translation>Horisontal  forskydning af side-elementer</translation>
    </message>
    <message>
        <source>Vertical displacement of page items</source>
        <translation>Vertikal forskydning af side-elementer</translation>
    </message>
    <message>
        <source>Constrain value for the rotation tool when the Control key is pressed</source>
        <translation>Indskrænke-værdi for rotations værktøjet, når Kontrol-tasten er trykket ned</translation>
    </message>
    <message>
        <source>Degrees</source>
        <translation>Grader</translation>
    </message>
    <message>
        <source>Use the embedded clipping paths in images when importing them. JPEG, PSD and TIFF are the image formats which can embedded clipping paths.</source>
        <translation>Brug de indlejrede klipnings-stier i billeder, når de importeres. JPEG, PSD og TIFF  er de billed-formater, som kan indeholde indlejrede klipnings-stier.</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Sænket skrift</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Forskydning:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Skalering:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hævet skrift</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>F&amp;orskydning:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>S&amp;kalering:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understreg</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Forskydning:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Liniebredde:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Gennemstreg</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Små kapitæler</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Sk&amp;alering:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automatisk &amp;linieafstand</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Linie afstand:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Forskydning over basislinien af fonten på en linie</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relativ størrelse af den hævede skrift sammenlignet med den normale font</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Forskydning under basislinien af den normale font på en linie</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relativ størrelse af den sænkede skrift sammenlignet med den normale font</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relativ størrelse af de små kapitæler sammenlignet med den normale font</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Procentvis forøgelse over fontstørrelsen af linieafstanden</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Forskydning under basislinien af den normale font udtrykt som en procentdel af fontens nedadgående højde</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Liniebredde udtrykt som en procentdel af fontens størrelse</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Forskydning over basislinien af den normale font udtrykt som en procentdel af fontens opadgående højde</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Højre</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Punktum</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrér</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Slet alle</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Indrykning af den første linie i et afsnit</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Indrykning fra venstre af hele afsnittet</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Slet alle tabulatorer</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Position:</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Fyld tegn:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Prik</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Bindestreg</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Underlinie</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brugerdefineret</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Indentation from the right for the whole paragraph</source>
        <translation>Indrykning fra højre af hele afsnittet</translation>
    </message>
    <message>
        <source>Fill Character of Tab</source>
        <translation>Tab fyld tegn</translation>
    </message>
    <message>
        <source>Type/Orientation of Tab</source>
        <translation>Type/Orientering af tab</translation>
    </message>
    <message>
        <source>Position of Tab</source>
        <translation>Placering af tab</translation>
    </message>
</context>
<context>
    <name>TextBrowser</name>
    <message>
        <source>Locate your web browser</source>
        <translation>Vælg din netlæser</translation>
    </message>
    <message>
        <source>External Web Browser Failed to Start</source>
        <translation>Ekstern netlæser kunne ikke starte</translation>
    </message>
    <message>
        <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences</source>
        <translation>Scribus var ikke i stand til at starte den eksterne netlæser %1. Tjek venligst instillingen i Præferencer</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Oversigt</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Lav til gruppe </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Frie objekter</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Side </translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fil: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Original PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Faktiske PPI: </translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Farverum: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukendt</translation>
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
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation>Duotone</translation>
    </message>
    <message>
        <source>No Image Loaded</source>
        <translation>Intet billede indlæst</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Sammenkædet tekst</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Tekst på en sti</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Afsnit: </translation>
    </message>
    <message>
        <source>Lines: </source>
        <translation>Linier: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Ord: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tegn: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Udskriv: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Aktiv</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Deaktiveret</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Indstillinger for vis udskrift</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF Indstillinger</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Placér i la&amp;g</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;veau</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Konve&amp;rtér til</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Omdøb</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Indhold</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.&lt;br/&gt;Vælg venligst et andet.</translation>
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
        <translation>Forskydning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Liniebredde</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Tilføj vertikale hjælpelinier</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Tilføj horisontale hjælpelinier</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Fjern vertikale hjælpelinier</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Fjern horisontale hjælpelinier</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Flyt vertikale hjælpelinier</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Flyt horisontale hjælpelinier</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Lås hjælpelinier</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Frigør hjælpelinier</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Flyt</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Ændr størrelse</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Rotér</translation>
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
        <translation>Markering</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Lav til gruppe</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Markering/Gruppe</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Lav</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Opstil på linie/Fordel</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Elementer involveret</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Angiv fyldfarve</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Farve1: %1, Farve2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Angiv fyldfarve-mætning</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Angiv liniefarve</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Angiv liniefarve-mætning</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Flip horisontalt</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Flip vertikalt</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Lås</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Lås op</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Lås størrelse</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Frigør størrelse</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Bryd gruppe</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slet</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Nyt navn</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Fra %1
til %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Anvend masterside</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Indsæt</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Klip</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Angiv fyldfarvens gennemsigtighed</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Angiv liniefarve-gennemsigtighed</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Angiv liniestil</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Angiv linie-endens stil</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Vælg af liniesamling</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Vælg liniebredde</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Vælg bruger liniestil</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Brug ikke bruger liniestil</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Vælg startpil</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Vælg slutpil</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Lav tabel</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Rækker: %1, Kolonner: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Vælg font</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Vælg fontstørrelse</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Vælg fontbredde</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Vælg fonthøjde</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Vælg font fyldfarve</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Vælg font stregfarve</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Vælg font fyldfarve-mætning</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Vælg font stregfarve-mætning</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Sæt knibning</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Vælg linieafstand</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Vælg afsnits typografi</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Vælg sprog</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Justér tekst</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Vælg fonteffekter</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Billedramme</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bezier-kurve</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Poly-linie</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Konvertér til</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Importér SVG billede</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importér EPS billede</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Importér OpenOffice.org Tegning</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Plads til noter</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Tekst flyder uden om ramme</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Tekst flyder uden om indbindingsboks</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Tekst flyder uden om konturlinie</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Ingen tekstflydning</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Ingen indbindingsboks</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Ingen konturlinie</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Side %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Vælg billedskalering</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Rammestørrelse</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Fri skalering</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Behold bredde/højde forhold</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Bryd bredde/højde forhold</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Redigér konturlinie</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Redigér form</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Nulstil konturlinie</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Tilføj side</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Tilføj sider</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Slet side</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Slet sider</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>tilføj lag</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Slet lag</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Omdøb lag</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Hæv lag</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Sænk lag</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Flyt til lag</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Aktivér udskrivning af lag</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Deaktivér udskrivning af lag</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Ændr lagets navn</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Hent billede</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Tekst på en sti</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Ændr billed-forskydning</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Ændr billed-skalering</translation>
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
        <source>Enable Item Printing</source>
        <translation>Aktivér element udskrivning</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Deaktivér element udskrivning</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Duplikér flere gange</translation>
    </message>
    <message>
        <source>Reset control point</source>
        <translation>Nulstil kontrolpunkt</translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation>Nulstil kontrolpunkter</translation>
    </message>
    <message>
        <source>No object frame</source>
        <translation>Ingen objekt-ramme</translation>
    </message>
    <message>
        <source>Apply text style</source>
        <translation>Anvend tekst-stil</translation>
    </message>
    <message>
        <source>&amp;Undo: %1</source>
        <comment>f.e. Undo: Move</comment>
        <translation>&amp;Fortryd: %1</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Fortryd</translation>
    </message>
    <message>
        <source>&amp;Redo: %1</source>
        <comment>f.e. Redo: Move</comment>
        <translation>&amp;Lav igen: %1</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Lav igen</translation>
    </message>
    <message>
        <source>Apply image effects</source>
        <translation>Anvend billed-effekter</translation>
    </message>
    <message>
        <source>Insert frame</source>
        <translation>Indsæt ramme</translation>
    </message>
    <message>
        <source>Adjust frame to the image size</source>
        <translation>Justér ramme til billed-størrelsen</translation>
    </message>
    <message>
        <source>Set start and end arrows</source>
        <translation>Sæt start og slut pile</translation>
    </message>
    <message>
        <source>Text flows around image clipping path</source>
        <translation>Tekst flyder uden om billed klipnings-sti</translation>
    </message>
    <message>
        <source>Remove all guides</source>
        <translation>Fjern alle hjælpelinier</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopiér</translation>
    </message>
    <message>
        <source>Copy page</source>
        <translation>Kopiér side</translation>
    </message>
    <message>
        <source>Remove vertical auto guide</source>
        <translation>Fjern vertikal auto hjælpelinie</translation>
    </message>
    <message>
        <source>Remove horizontal auto guide</source>
        <translation>Fjern horisontal auto hjælpelinie</translation>
    </message>
    <message>
        <source>Convert to outlines</source>
        <translation>konvertér til kurver</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Indledende tilstand</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Handlings historie</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Vis kun det markerede objekt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Fortryd</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Lav igen</translation>
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
    <name>UnicodeChooseButton</name>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søg</translation>
    </message>
</context>
<context>
    <name>UnicodeSearchBase</name>
    <message>
        <source>Unicode Search</source>
        <translation>Unikode søgning</translation>
    </message>
    <message>
        <source>&amp;Search:</source>
        <translation>&amp;Søg:</translation>
    </message>
    <message>
        <source>Hex</source>
        <translation>Hex</translation>
    </message>
    <message>
        <source>Meaning</source>
        <translation>Betydning</translation>
    </message>
</context>
<context>
    <name>UpgradeChecker</name>
    <message>
        <source>Attempting to get the Scribus version update file</source>
        <translation>Forsøger at hente Scribus versionens opdaterings-fil</translation>
    </message>
    <message>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation>(Der vil ikke blive sendt nogen data fra din computer til et andet sted)</translation>
    </message>
    <message>
        <source>Timed out when attempting to get update file.</source>
        <translation>Tidsudløb i forsøget på at hente opdaterings-fil.</translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation>Fejl i forsøget på at hente opdaterings-fil: %1</translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation>Fil findes ikke på server</translation>
    </message>
    <message>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation>Kunne ikke åbne versions-fil: %1
Fejl:%2 på linie: %3, række: %4</translation>
    </message>
    <message>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation>En fejl opstod ved søgning efter opdateringer for Scribus, tjek venligst din internet-forbindelse.</translation>
    </message>
    <message>
        <source>No updates are available for your version of Scribus %1</source>
        <translation>Der er ingen opdateringer tilgængelig for din version af Scribus %1</translation>
    </message>
    <message>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation>der er en eller flere opdateringer for din version af Scribus (%1) tilgængelig:</translation>
    </message>
    <message>
        <source>This list may contain development versions.</source>
        <translation>Denne liste kan indeholde udviklings versioner.</translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation>Besøg venligst www.scribus.net for detaljer.</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Minimum margener for sidestørrelse %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Brug printer margener</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Vælg &amp;Printer:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Margener</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Højre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bund:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
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
        <translation>&amp;Annullér</translation>
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
        <translation>Indsæt værdi</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Indtast en værdi og tryk OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Indtast en værdi og tryk OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Send din værdi til scripten</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vælg importfilter</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importér tekst uden nogen formatering</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importfilter:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Tegnsæt:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importér kun tekst</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Vælg importfilter</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Husk tilknytning</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Husker fil-endelsen - importfilter-tilknytning og spørger ikke igen om at vælge et importfilter for denne filtype.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Nyt fra skabelon</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Brug</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>oprettet med</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Forfatter</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Fjern</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Åbn</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Hent skabeloner fra nettet</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Installering af skabeloner</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Udpak filen i skabelonmappen ~/.scribus/templates for den enkelte bruger eller PREFIX/share/scribus/templates for alle brugere af systemet.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Fremstilling af en skabelon</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Fjerner en skabelon</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Oversættelse af template.xml</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Dokument skabeloner kan hentes fra http://www.scribus.net i &quot;Downloads&quot; afsnittet.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Vær sikker på at billeder og fonte du bruger frit kan bruges. Hvis fonte ikke kan deles så medtag dem ikke, når du gemmer som skabelon. </translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Den der fremstiller en skabelon skal også sikre sig, at den kan installeres som beskrevet i sektionen &apos;installering af skabeloner&apos; oven for. Hvilket betyder, at en bruger kan nedhente en pakke med skabeloner og være i stand til at udpakke den i skabelonmappen og derefter begynde at bruge dem.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Når du fjerner en skabelon fra &apos;Nyt Fra skabelon&apos; vinduet slettes kun linien fra template.xml. Dokumentfilerne bliver ikke slettet. Pop op menuen vil kun vise &quot;Fjern&quot; hvis du har skrive rettigheder til template.xml filen.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopiér en eksisterende template.xml til en fil med navnet template.lang_COUNTRY.xml (brug den samme kode, som bruges for qm filen for dit sprog), f.eks. template.fi.xml for den finske template.xml. Kopien skal være placeret i den samme mappe, som den originale template.xml så Scribus kan indlæse den.</translation>
    </message>
</context>
<context>
    <name>patternDialogBase</name>
    <message>
        <source>Patterns</source>
        <translation>Mønstre</translation>
    </message>
    <message>
        <source>Load</source>
        <translation>Indlæs</translation>
    </message>
    <message>
        <source>Load Set</source>
        <translation>Indlæs sæt</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Remove All</source>
        <translation>Fjern alle</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Gem som skabelon</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategori</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Brug</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Forfatter</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Email</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Flere detaljer</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Færre detaljer</translation>
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
        <translation>Landskab</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>portræt</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>brugerdefineret</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Lav filter</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Nu&amp;lstil</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Vælg et tidligere gemt filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Giv dette filter et navn, så det kan gemmes</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Filtrets navn</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Deaktivér eller aktivér denne filter række</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Fjern denne filter række</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Tilføj en ny filter række</translation>
    </message>
    <message>
        <source>to</source>
        <translation>til</translation>
    </message>
    <message>
        <source>and</source>
        <translation>og</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>fjern søgeord</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>fjern ikke søgeord</translation>
    </message>
    <message>
        <source>words</source>
        <translation>ord</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Erstat</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Anvend</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Værdien til venstre er et regulært udtryk</translation>
    </message>
    <message>
        <source>with</source>
        <translation>med</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>afsnits typografi</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>alle tilfælde af</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>alle afsnit</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>afsnit der starter med</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>afsnit med mindre end</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>afsnit med mere end</translation>
    </message>
</context>
</TS>
