<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Gir en liste med navn på alle fargene som er angitt i dokumentet. Hvis ikke et dokument er åpnet, så vil du få en liste med standard dokumentfarger.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Viser dialogvinduet «Nytt Dokument». Lager et nytt dokument hvis brukeren godtar
innstillingene. Lager ikke et nytt dokument hvis brukeren velger «Avbryt». Gir
«sann» hvis et nytt dokument ble opprettet.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;navn&quot;]) -&gt; streng

Gir navnet på fyllfargen til objektet som heter «navn».
Hvis «navn» ikke er oppgitt, så vil det objektet som er valgt bli brukt.</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;navn&quot;])

Flytter objektet «navn» avstanden «dy» og «dx» i forhold til dets nåværende posisjon.
De måleenhetene som som brukes, er de som er valgt for dokumentet. Hvis navnet («navn») ikke er angitt, så brukes det objektet som er valgt. Hvis objektet «navn» tilhører en gruppe, så vil hele gruppen bli flyttet.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Skrur av gjenopptegning av siden når «bool = False», i motsatt fall er gjenopptegning på.
Denne endringa vil vare ved selv når skriptet er ferdig, så pass på at du kjører
«setRedraw(True)» på begynnelsen av skriptet ditt.</translation>
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
        <translation>createRect(x, y, bredde, høyde, [&quot;navn&quot;]) -&gt; streng

Lager et nytt rektangel på gjeldende side og gir rektangelets navn. Koordinatene oppgis i det som er standard måleenheten for dokumentet (se konstanverdien UNIT). Navnet («Navn») må være unikt for dette objektet fordi du trenger det for å kunne referere til objektet senere. Hvis «navn» ikke er angitt, vil Scribus lage et navn for deg.

Vil gi feilmeldinga «NameExistsError» hvis du ber om et navn som allerede er i bruk.</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;førsteFarge&quot;, metningsgrad, &quot;andreFarge&quot;, metningsgrad, [&quot;navn&quot;])

Velger gradientfyllgraden på objektet «navn». Fargebeskrivelsene er de samme som for «setFillColor()» og «setFillShade()». Se lista over konstanter for å få en liste over hvilke typer som er tilgjengelige (FILL_&lt;type&gt;).</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;navn&quot;]) -&gt; float

Gir størrelsen på skrifttypene i punkter for tekstramma «navn».
Hvis noe av teksten i denne tekstramma er valgt, så gis det første tegnet i utvalget.
Hvis «navn» ikke er angitt, så brukes det valgte elementet.</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;streng&quot;)

Skriver «streng» i meldingslinja til Scribus (statuslinja). Teksten må være i UTF8 eller unicode (det siste er anbefalt).</translation>
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
        <translation>newPage(where [,&quot;malside&quot;])

Oppretter en ny side. Hvis «where» er «-1» vil den nye siden bli lagt til dokumentet, ellers vil den nye siden settes før «where». Sidetall telles fra 1 og oppover, uavhengig av hvilke sidetall som er satt inn på sidene. Tilleggskommandoen «malside» er eventuelt navnet på den malsiden du vil bruke på den nye siden.

Du kan få feilmeldinga «IndexError» hvis siden ikke finnes (for høyt eller lavt sidetall).</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>importSVG(&quot;streng&quot;)

«Streng» må være et gyldig filnavn for et SVG-bilde. Teksten må være kodet som UTF-8 eller unicode (anbefalt).</translation>
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
        <translation>newDocument(størrelse, marger, retning, førsteSideTall,
                        måleenhet, sideType, førsteSideOrden) -&gt; bool

Oppretter et nytt dokument og gir «true» (sann) hvis det lykkes. Parametrene har 
følgende betydning:

størrelse = En tuppel (bredde, høyde) beskriver størrelsen av dokumentet. du kan
bruke forhåndsvalgte konstanter med navnet PAPER_&lt;papir_type&gt; f.eks. PAPER_A4 osv.

marger = En tuppel (venstre, høyre, øverst, nederst) angir margene i dokumentet

retning = sideretningen – konstanter PORTRAIT, LANDSCAPE

førsteSideNummer = er nummeret på den første side i dokumentet slik 
sidenummerering skal er. Selv om verdien oftest er 1, så er der noen ganger bruk for 
et høyere tall, f.eks. hvis et dokument består av flere deler.

måleenhet: denne verdien angir måleenheten, som skal brukes i dokumentet. Bruk en
forhåndsvalgt konstant for dette, mulige konstanter er: UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.

sideType = En av de forhåndsvalgte konstantene: PAGE_n. PAGE_1 er enkeltside,
PAGE_2 er for dobbeltsidige dokumenter, PAGE_3 er for tredelt folder og
PAGE_4 er firedelt

førsteSideOrden= Hvilken plassering har første side i dokumentet.
Indeksering fra 0 (0 = første).

Verdiene for bredde, høyde og marger angis i den valgte måleenheten
for dokumentet. «PAPER_* konstanter» er angitt i punkter. Hvis dokumentet ditt
ikke er i punkter, så sørg for å ta høyde for dette.

eksempel: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3)

Kan gi feilmeldinga «ScribusError» hvis førsteSideOrden er større enn tillatt av «pagesType».
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
        <translation>getColor(«navn») -&gt; tuppel 

Gir en tuppel (C, M, Y, K) som inneholder de fire fargekomponentene i det gjeldende dokumentet. Hvis ingen dokumenter er åpne gir verdien på den navngitte fargen fra standardfargene i det gjeldende dokumentet. 

Kan gi feilmeldinga «NotFoundError» hvis den navngitte fargen ikke blir funnet. 
Kan gi feilmeldinga «ValueError » hvis fargenavnet er ugyldig.</translation>
    </message>
    <message>
        <source>changeColor(&quot;name&quot;, c, m, y, k)

Changes the color &quot;name&quot; to the specified CMYK value. The color value is
defined via four components c = Cyan, m = Magenta, y = Yellow and k = Black.
Color components should be in the range from 0 to 255.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>changeColor(&quot;fargenavn&quot;, c, m, y, k)

Endrer fargen «fargenavn» til den angitte CMYK-verdien. Fargeverdien angis ved hjelp av fire komponenter: c = cyan, m = magenta, y = gul og k = svart. 
Hver farge må angis med et tall mellom 0 og 255.

Kan gi feilmeldinga «NotFoundError» hvis den navngitte fargen ikke blir funnet. 
Kan gi feilmeldinga «ValueError » hvis fargenavnet er ugyldig.</translation>
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
        <translation>deleteColor(&quot;fargenavn&quot;, &quot;nyttFargenavn&quot;) 

Alle forekomstene av fargen «fargenavn» blir slettet og erstattet av fargen «nyttFargenavn». Hvis «nyttFargenavn» ikke er angitt, så brukes standardverdien &quot;Ingen&quot;, gjennomsiktig.

Kan gi feilmeldinga «NotFoundError» hvis den navngitte fargen ikke blir funnet. 
Kan gi feilmeldinga «ValueError » hvis fargenavnet er ugyldig.</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;fargenavn&quot;, &quot;nyttFargenavn&quot;) 

Alle forekomstene av fargen «fargenavn» blir erstattet av fargen «nyttFargenavn». 

Kan gi feilmeldinga «NotFoundError» hvis den navngitte fargen ikke blir funnet. 
Kan gi feilmeldinga «ValueError » hvis fargenavnet er ugyldig.</translation>
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
        <translation>valueDialog(tittel, beskjed [,standardverdi]) -&gt; streng

Viser det vanlige dialogvinduet «spør etter en streng» og gir dens verdi som en streng
Parametre: vindustittelen, teksten i vinduet og en valgfri «standard»-verdi.

Eksempel: valueDialog(&apos;tittel&apos;, &apos;tekst i vinduet&apos;, &apos;valgfri&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Lukker det gjeldende dokument uten å spørre om det skal lagres.

Kan gi feilmeldinga «NoDocOpenError» hvis det ikke er noe dokument å lukke
</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Gir «true» hvis det er et åpent dokument.
</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;navn&quot;) 

Åpner dokumentet «navn». 

Kan gi feilmeldinga «ScribusError» hvis dokumentet ikke kan åpnes.</translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation>saveDoc(\&quot;navn\&quot;)

Lagrer det gjeldende dokument med dets nåværende navn, gir «true» hvis
det lykkes. Hvis dokumentet ikke allerede har blitt lagret, så kan det komme fram
et interaktiv dialogvindu som ber deg lagre.

Hvis skriptet ikke klarer å lagre, er det ingen måte at fortelle det på.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;navn&quot;) 

Lagrer det gjeldende dokumentet under navnet «navn» (som også kan innehlde en relativ eller en fullstendig sti). 

Kan gi feilmeldinga «ScribusError» hvis dokumentet ikke kan åpnes.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Angir margene i dokumentet, Venstre(lr), Højre(rr), Øvre(tr) og Nedre(br).
Margene angis i måleenheten for dokumentet – se UNIT_&lt;type&gt; konstanter.</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type) 

Endrer måleenheten i dokumentet. Mulige verdier for «unit» er angitt som konstanterene UNIT_&lt;type&gt;. 

Kan gi feilmeldinga «ValueError» hvis du angir en ugyldig verdi.</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; heltall (Scribus enhetskonstant)

Gir måleenheten for dokumentet. Verdien vil være en av UNIT_* konstantene:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filnavn&quot;)

Henter inn avsnittsstiler fra Scribus-dokumentet «filenavn» inn i det gjeldende dokumentet.</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(facingPages, firstPageLeft)

Velger dokumenttypen. For å få motstående sider, sett den første verdien
til «FACINGPAGES», for skru av dette bruk isteden «NOFACINGPAGES». Hvis du ønsker
å ha den første side som venstre side, sett da den andre verdien til «FIRSTPAGELEFT», hvis du vil ah en høyreside som den første, bruk «FIRSTPAGERIGHT».
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;navn&quot;]) -&gt; streng 

Gir navnet på strekfargen til objektet «navn». Hvis «navn» ikke er angitt, så blir det gjeldende objektet valgt.</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;navn&quot;]) -&gt; heltall

Gir strekbredden på objektet «navn» hvis «navn» ikke er oppgitt så blir det gjeldende objektet valgt.</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;navn&quot;]) -&gt; heltall 

Gir metningsgraden på strekfargen til objektet «navn». 
Hvis «navn» ikke er angitt blir det gjeldende objektet valgt.&gt;</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;navn&quot;]) -&gt; heltall (se lista over konstanter) 

Gir stilen for sammenslåtte linjer til objektet «navn». Hvis «navn» ikke er angitt, brukes det gjeldende objektet. Stilene for sammenslåtte linjer er: 
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND (henholdsvis: gjæring, skrå og snitt.)</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([«navn»]) -&gt; heltall (se lista over konstanter) 

Gir linjeslutt-stilen til objektet «navn». Hvis «navn» ikke er oppgitt, så brukes det valgte objektet. Linjestilen kan være: 
CAP_FLAT, CAP_ROUND, CAP_SQUARE</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([«navn»]) -&gt; heltall (se lista over konstanter) 

Gir linjestilen til objektet «navn». Hvis «navn» ikke er oppgitt, så brukes det valgte objektet. Linjestilen kan være: 
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([«navn»]) -&gt; heltall 

Gir gjennomsiktighetsgraden på fyllfargen til objektet «navn». Hvis «navn» ikke er oppgitt, så brukes det valgte objektet.
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([«navn»]) -&gt; (x,y)

Gir en a (x, y)-tuppel med skaleringsverdien til bilderamma «navn».
Hvis «navn» ikke er oppgitt, så brukes det valgte objektet</translation>
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
        <translation>getAllObjects() -&gt; liste

Gir en liste, som inneholder navnene på alle objektene på den gjeldende siden.
</translation>
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
        <translation>selectionCount() -&gt; heltall

Gir antall markerte objekter.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;navn&quot;) 

Velger objektet som heter «navn».</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Fjerner alle markeringene i hele dokumentet.
</translation>
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
        <translation>getFontNames() -&gt; liste

Gir en liste med navnene på alle de tilgjengelige skrifttypene.
</translation>
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
        <translation>getLayers() -&gt; liste

Gir en liste med navnene på alle angitte lag.
</translation>
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
        <translation>getActiveLayer() -&gt; streng

Gir navnet på det gjeldende laget.
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

Sender objektet «navn» til laget «lag». Laget må finnes.
If «navn» ikke er angitt brukes det gjeldende objektet.

Kan gi feilmeldinga «NotFoundError» hvis laget ikke kan bli funnet.
Kan gi feilmeldinga «ValueError» hvis navnet på laget ikke blir godkjent.</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;lag&quot;, synlig) 

Velg om laget «lag» skal være synlig eller ikke. Hvis «synlig» er satt til «false» vil laget være usynlig.

Kan gi feilmeldinga «NotFoundError» hvis laget ikke kan bli funnet.
Kan gi feilmeldinga «ValueError» hvis navnet på laget ikke blir godkjent.</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;lag&quot;, kanSkrivesUt) 

Velg om laget «lag» skal kunne skrives ut eller ikke. Hvis «kanSkrivesUt» er satt til «false» vil ikke laget bli skrevet ut.

Kan gi feilmeldinga «NotFoundError» hvis laget ikke kan bli funnet.
Kan gi feilmeldinga «ValueError» hvis navnet på laget ikke blir godkjent.</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;lag&quot;) 

Slett laget som heter «lag». Hvis det ikke finnes noe lag som heter dette, eller det er det eneste laget i dokumentet, så vil det ikke skje noe.

Kan gi feilmeldinga «NotFoundError» hvis laget ikke kan bli funnet.
Kan gi feilmeldinga «ValueError» hvis navnet på laget ikke blir godkjent.</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(&quot;lag&quot;) 

Opprett laget som heter «lag».

Kan gi feilmeldinga «ValueError» hvis navnet på laget ikke blir godkjent.</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; streng

Gir en streng med verdien for «lang» (valgt språk).
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
        <translation>deleteObject([&quot;navn&quot;])

Sletter objektet som heter «navn». Hvis «navn» ikke er angitt, så slettes det valgte objektet.</translation>
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
        <translation>getAllStyles() -&gt; liste

Gir en liste over navnene på alle avsnittsstilene i det gjeldende dokument.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; heltall

Gir nummeret på den gjeldende siden. Sidetallet starter på 1 og
går oppover, uansett hvilket sidetall den første siden har fått.</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Gjenoppfrisker visningen av alle sidene.</translation>
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
        <translation>deletePage(nr)

Sletter den angitte siden. Ingen ting blir slettet, hvis dokumentet bare inneholder en side.
Sidetallet telles fra 1 og oppover, uansett hvilket sidetall den første siden har fått.

Kan gi feilmeldinga «IndexError» hvis sidetallet er utenfor det gyldige området.
</translation>
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
        <translation>pageCount() -&gt; heltall

Gir antall sider i dokumentet.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; liste

Gir en liste med posisjonene på de vannrette hjelpelinjene. Verdiene er
angitt i måleenheten for dokumentet – se UNIT_&lt;type&gt; konstanter.
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
Oppretter vannrette hjelpelinjer. Inn-parameterne skal være en liste med plasseringen av hjelpelinjene, angivet i måleenheten som er valgt for dokumentet – se  UNIT_&lt;type&gt; konstanter.

Eksempel: setHGuides(getHGuides() + [200.0, 210.0] # lager en ny hjelpelinje uten at de tidligere hjelpelinjene går tapt.
          setHGuides([90,250]) # Erstatter de nåværende hjelpelinjene fullstendig
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Se «getHGuides».
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Se «setHGuides».
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuppel

Gir en tuppel med sidestørrelsene, målt i dokumentets angitte måleenhed.
Se UNIT_&lt;type&gt; konstanter og getPageMargins()
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

Gir en liste med tuppler med elementene på den gjeldende siden. Tuppelen er:
(navn, objekttype, rekkefølge) F.eks. [(&apos;Tekst1&apos;, 4, 0), (&apos;Bilde1&apos;, 2, 1)].
Det betyr at objektet «Tekst1» er en tekstramme (type 4) og er den
første på siden ...
</translation>
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
        <translation>setLineWidth(width, [&quot;navn&quot;])

Angir at strekbredden på objektet «navn» er «width». «Width» må være mellom 0.0 og 12 punkt. Hvis «navn» ikke er angitt vil det gjeldende objektet bli valgt.

Kan gi feilmeldinga «ValueError» hvis strekbredden ikke er mellom 0 og 12 punkt.</translation>
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
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float 

Gir linjeavstanden på teksten i tekstramma, målt i punkt. Hvis «navn» ikke er angitt så brukes det gjeldende objektet.</translation>
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
        <translation>setLineSpacing(størrelse, [&quot;navn&quot;]) 

Setter linjeavstanden på teksten i tekstramma «navn» til «størrelse». Størrelsen måles i punkt. Hvis «navn» ikke er angitt brukes det gjeldende objektet.</translation>
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
        <translation>progressReset()

Rydder opp i de tidligere innstillingene til Scribus&apos; framdriftsviser. Den kalles før framdriftsviseren brukes neste gang. Se «progressSet».
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Setter framdriftsviserens høyeste trinnverdi til det angitte tallet.
Se «progressSet».
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[IKKE STØTTET!] Kan føre til at noe bryter sammen, så unngå å bruke den foreløpig.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Skrus av/på skjule-ikonet i verktøylinja til Scribus og menyoppføringa «Lagre» i filmenyen.
Det er nyttig å kalle denne prosedyren når du endrer dokumentet, fordi Scribus
ikke oppdager det automatisk når du har endret et dokument ved at bruke et skript.
</translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;markering&gt;)

Setter skaler til ramme på den markerede eller angitte bilde-ramma til «scaletoframe».
Hvis du har valgt «proporsjonalt», så blir størrelsesforholdet til «proporsjonalt».
Både «skalertilramme» og «proporsjonalt» er boolske verdier.

Kan gi feilmeldinga «WrongFrameTypeError».
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
        <translation>getPropertyCType(objekt, egenskap, inkludérsuper=True)

Gir navnet på «egenskapen»s C-type i det gitte «objektet». Se «getProperty()» for flere detaljer.

Hvis «inkludérsuper» er «true», let også i nedarvede egenskaper.
</translation>
    </message>
    <message>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation>getPropertyNames(objekt, inkludérsuper=True)

Gir en liste over egenskapsnavn som er støttet av «objekt».
Hvis «inkludérsuper» er «true» gis også navnene på de egenskapene som er 
støttet av foreldreklassen.
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
        <translation>getProperty(objekt, egenskap)

Gir verdien til egenskapen «egenskap» i det gitte «objektet».

Argumentet «object» kan være en streng,  og det vil da bli søkt etter det
navngitte Sideelement. Det kan også være et PyObjekt, som kan peke på
et hvilket som helst C++ QObjekt tilfelle.

Argumentet «egenskap» skal være en streng, og er navnet på den egenskapen
som det søkes etter i `objekt&apos;.

Verdien som gis vil variere avhengig av egenskapen til objektet.
</translation>
    </message>
    <message>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation>setProperty(objekt, egenskap, verdi)

Angir «egenskapen» til et «objekt» til «verdi». Hvis «verdi» ikke kan gjøres om til en type
som er forenelig med «egenskapen», vil det gi et unntak. Et unntak kan også opprettes hvis det ikke går å tillegge objektet denne egenskapen.

Se «getProperty()» for mer informasjon.
</translation>
    </message>
    <message>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation>getChildren(objekt, av_klasse=None, av_navn=None, regexpmatch=False, rekursiv=True)

Gir en liste med barn av «objekt», kanskje begrenset til barn
av klasse med navnet «av_klasse» eller barn med navnet «av_navn». Hvis «rekursiv»
er på («True»), så vil funksjonen rekursivt lete gjennom barn, barnebarn osv.

Se i QT-dokumentasjonen til «QObject::children()» for mer informasjon.
</translation>
    </message>
    <message>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation>getChild(objekt, barnenavn, av_klasse=None, rekursiv=True)

Gir det første barnet av «objekt» med navnet «barnenavn», søkingen
kan være begrenset til barn av klasser med navnet «af_klasse». Hvis «rekursiv»
er på (True), vil funksjonen søke rekursivt gjennom barn, barnebarn, etc.
</translation>
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
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Gir sidemargene som en (top, venstre, høyre, bunn) tuppel i dokumentets
aktuelle måle-enhet. Se  UNIT_&lt;type&gt; konstanter og getPageSize().
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
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation>newStyleDialog() -&gt; streng

Viser vinduet «Lag en ny avsnittsstil». Funksjonen gir det riktige
stilnavnet – eller ingenting når brukeren trykker «Avbryt» i vinduet.
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
        <translation>newDoc(størrelse, marger, retning, førsteSideNummer,
                   måle-enhet, modståendeSider, førsteSideVenstre) -&gt; bool

ADVARSEL: Foreldet prosedyre! bruk newDocument isteden.

Laver et Nyt dokument og gir \&quot;true\&quot; hvis det lykkedes. Parametrene har
følgende betydning:

    størrelse = A tuppel (bredde, høyde) beskriver størrelsen av dokumentet. du kan
    bruke forhåndsvalgte konstanter med navnet PAPER_&lt;paper_type&gt; f.eks. PAPER_A4 osv.

    marger = En tuppel (venstre, høyre, øverst, nederst) beskriver margene i dokumentets

    retning = sidens retning – konstanter PORTRAIT, LANDSCAPE

    førsteSideNummer = er nummeret på den første siden i dokumentet brukt for
    side-nummerering. Selv om verdien vanligvis er 1, så er det noen ganger bruk for
    et høyere nummer når et dokument består av flere dele.

    enhet: denne verdi velger hvilken måleenhet som skal brukes i dokumentet. Bruk en
    forhåndsvalgt konstant til dette, en av: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    modståendeSider = FACINGPAGES, NOFACINGPAGES

    førsteSideVenstre = FIRSTPAGELEFT, FIRSTPAGERIGHT

Verdiene for bredde, høyde og marger er oppgis i den gitte måleenheten
for dokumentet. PAPER_* konstanter er oppgitt i punkter. Hvis ditt dokument
ikke er i punkter, så sørg for at ta høyde for dette.

eksempel: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)</translation>
    </message>
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation>closeMasterPage()

Lukker den gjeldende malsiden, hvis det er noen, og går tilbake til vanlig sideredigering. Du kan starte å redigere en malside med kommandoen «editMasterPage()».
</translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation>masterPageNames()

Gir en liste over navnene på alle malsidene i dokumentet.</translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation>editMasterPage(sideNavn)

Åpner malsiden «sideNavn» så du kan endre den. Avslutt redigeringen med «closeMasterPage()».
</translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation>createMasterPage(sideNavn)

Lager en malside som heter «sideNavn» og åpner den så den kan redigeres.
</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(sideNavn)

Slett den navngittee malsiden.
</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(dobbelt)

Skaler opp visningen av dokumentet i hovedvinduet. Funksjonen bruker heltallsverdier
som 20.0, 100.0, osv. Hvis siden skal fylle vinduet, bruker -100.
</translation>
    </message>
    <message>
        <source>duplicateObject([&quot;name&quot;]) -&gt; string

creates a Duplicate of the selected Object (or Selection Group).
</source>
        <translation>duplicateObject([&quot;navn&quot;]) -&gt; streng

Lager en kopi av det valgte objektet (eller den valgte gruppa).</translation>
    </message>
    <message>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>setInfo(&quot;forfatter&quot;, &quot;informasjon&quot;, &quot;beskrivelse&quot;) -&gt; bool

Angir informasjon om dokumentet: «forfatter», «informasjon» og «beskrivelse» er strenger.</translation>
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
        <translation>&amp;Oversettelser</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Nett</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Utviklingslag:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Offisiell dokumentasjon:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Annen dokumentasjon:</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Offisielle oversettelser og oversettere:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Tidligere oversettere:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hjemmeside</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Nettreferanse</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Feil og ønsker</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>E-postliste</translation>
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
        <translation>Bruker Ghostscript-versjon %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ingen Ghostscript-versjon er tilgjengelig</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribusversjon %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Bygg ID:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Denne ruta viser versjon, byggdato og hvilke biblioteker som er tatt med i Scribus. Koden C-C-T-F viser til følgende: C=littlecms, C=CUPS, T=Støtte  for TIFF-formatet og F=Skriftoppsett. Den siste bokstaven viser til C=cairo eller A=libart. Manglende biblioteker er merket med en stjerne (*). Denne ruta viser også hvilken versjon av Ghostscript Scribus har tatt i bruk på ditt system.</translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Mac OS&amp;#174; X Aqua-versjon:</translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation>Versjon for Windows&amp;#174;:</translation>
    </message>
    <message>
        <source>Tango Project Icons:</source>
        <translation>Prosjektikoner for «Tango»:</translation>
    </message>
    <message>
        <source>OS/2&amp;#174;/eComStation&amp;#8482; Port:</source>
        <translation>OS/2&amp;#174;/eComStation&amp;#8482; Port:</translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation>Filnavn:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Versjon:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Skrudd på:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Utgivelsesdato:</translation>
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
        <translation>Opphavsrett:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Lisens:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: om tilleggsprogram</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>L&amp;ukk</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
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
        <translation>&amp;Åpne ...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>L&amp;ukk</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Gå &amp;tilbake til lagret versjon</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Samle for &amp;utskrift ...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Sett inn tekst ...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Legg til &amp;tekst ...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Sett inn bilde ...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Lagre &amp;tekst ...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Lagre som &amp;EPS ...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Lagre som &amp;pdf ...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Dokumentinnstillinger ...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Skriv ut ...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>A&amp;vslutt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør om</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Elementhandling</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Kli&amp;pp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Velg &amp;alt</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Fravelg alt</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søk/erstatt ...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Rediger bilde ...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Far&amp;ger ...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Avsnittsstiler ...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Linjestiler ...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Malsider ...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Innstillinger ...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Andre ...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Venstre</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Midtstilt</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Høyre</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blokkjustert</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Strengt blokkjustert</translation>
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
        <translation>&amp;Understreket</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Understrek &amp;ord</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Strek over</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>S&amp;tore bokstaver</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapiteler</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;Hevet skrift</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>&amp;Senket skrift</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Skygge</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Bildeeffekter</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorer ...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Lag en &amp;kopi</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Lag &amp;flere kopier</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Gruppér</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Oppløs gruppen</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>&amp;Låst</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Låst &amp;størr&amp;else</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Legg &amp;underst</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Legg &amp;øverst</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Senk</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Hev</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Send til &amp;kladdeboka</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Egenskaper ...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>&amp;Vis bildet</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Oppdater bildet</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Juster ramma til bildet</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Utvidede bildeegenskaper</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Lav oppløsning</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>&amp;Normal oppløsning</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Full oppløsning</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Er et pdf-&amp;bokmerke</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Er en pdf-&amp;kommentar</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Kommentar-&amp;egenskaper</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Feltegenskaper</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Rediger formen ...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Legg inn tekststien</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Fjern tekststien</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Sett sammen mangekanter</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>&amp;Del opp mangekanter</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Bé&amp;zierkurve</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;Bilderamme</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Mangekant</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Tekstramme</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Glyff ...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Eksempeltekst</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>Sett &amp;inn ...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>Im&amp;porter ...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Slett ...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopier ...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Flytt ...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>&amp;Bruk malside ...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Endre &amp;hjelpelinjene ...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>&amp;Tilpass til vinduet</translation>
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
        <translation>&amp;Miniatyrbilder</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Vis &amp;margene</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Vis &amp;rammene</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Vis &amp;bildene</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Vis &amp;rutenettet</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Vis &amp;hjelpelinjene</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Vis &amp;vannrette linjer</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Vis &amp;tekstlenkene</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Vis kontrolltegnene</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Fest til gitteret</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Fest til hjelpelinjene</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Egenskaper</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Kladdebok</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Lag</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Flytt om på sidene</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bokmerker</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Målinger</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>Skru på &amp;handlingshistorien</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Forhåndssjekk</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Juster og fordel</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Verktøy</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>&amp;Pdf-Verktøy</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Velg objekt</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>&amp;Tabell</translation>
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
        <translation>&amp;Frihåndslinje</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Roter objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Skaler</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Vis større</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Vis mindre</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Rediger innholdet i denne ramma</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Rediger tekst ...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Lenk sammen tekstrammer</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Fjern lenker mellom tekstrammer</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Fargeplukker</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopier elementegenskapene</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Rediger teksten med den innebygde tekstbehandleren</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Sett inn en tekstramme</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Sett inn en bilderamme</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Sett inn en tabell</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Sett inn en en form</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Sett inn en mangekant</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Sett inn en linje</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Sett inn en Bézierkurve</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Sett inn en frihåndslinje</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Bildeoppsett</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Orddel tekst</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Fjern orddelingene</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Lag Innholdsregistre</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>Om &amp;Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Om &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Hjelpetekster</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Hånd&amp;bok for Scribus ...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Myk bindestrek</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Hard tankestrek</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Hardt mellomrom</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>&amp;Sidetall</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Linjeskift</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Rammeskift</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Kolonneskift</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Opphavsrett</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registrert varemerke</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Varemerke</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Lang tankestrek</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Kort tankestrek</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation type="unfinished">Bilde-tankestrek</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation type="unfinished">Sitat-tankestrek</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apostrof</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Dobbel apostrof</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Engelsk hermetegn, venstre, enkelt</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Engelsk hermetegn, høyre, enkelt</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Engelsk hermetegn, venstre</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Engelsk hermetegn, høyre</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Enkelt engelsk hermetegn, snudd</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation type="unfinished">Dobbelt engelsk hermetegn, snudd</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Enkelthake, venstre</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Enkelthake, høyre</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Norsk hermetegn, venstre</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Norsk hermetegn, høyre</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Komma, nede</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Dobbeltkomma, nede</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>CJK, enkelt, venstre</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>CJK, enkelt, høyre</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>CJK, dobbel, venstre</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>CJK, dobbel, venstre</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation type="unfinished">Skru av/på vinduene</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Skru av/på hjelpelinjene</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Juster egenskapene for siden ...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Linjalmål i forhold til siden</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>&amp;Forhåndsvisning av utskrift</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;Javascript ...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Gjør om til malside ...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>Legg &amp;over hverandre</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>Legg ved &amp;siden av hverandre</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>Om &amp;tilleggsprogrammene</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Mer informasjon ...</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Vis på utskrifta</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>Snu &amp;vannrett</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>Snu &amp;loddrett</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Vis linjalene</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation type="unfinished">Skråstrek</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Midtstilt punkt</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>Kort mellomrom (n-bredde)</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Langt mellomrom (m-bredde)</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Smalt mellomrom</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Bredt mellomrom</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Midtstilt mellomrom</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Det smaleste mellomrommet</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Sett inn myk bindestrek</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Sett inn hard tankestrek</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Sett inn hardt mellomrom</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Sett inn sidetall</translation>
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
        <translation>&amp;Stiler ...</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Lim inn (&amp;absolutt)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Tøm</translation>
    </message>
    <message>
        <source>Show Text Frame Columns</source>
        <translation>Vis tekstramme-kolonner</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>
type effect</comment>
        <translation type="obsolete">&amp;Omriss</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>
Convert to oulines</comment>
        <translation type="obsolete">&amp;Omriss</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>
Document Outline Palette</comment>
        <translation type="obsolete">&amp;Omriss</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation type="unfinished">&amp;Omriss</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation type="unfinished">&amp;Omriss</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation type="unfinished">&amp;Omriss</translation>
    </message>
    <message>
        <source>&amp;400%</source>
        <translation type="unfinished">&amp;400%</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Juster og fordel</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Juster</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;I forhold til:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Første valgte</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Siste valgte</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Siden</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Margene</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>En hjelpelinje</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Utvalg</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Juster den høyre siden av objektene til den venstre siden av ankeret</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Juster den venstre siden av objektene til den høyre siden av ankeret</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Juster ut fra bunnlinjene</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Juster ut fra høyresidene</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Juster toppen av objektene til bunnen av ankeret</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Midt på den loddrette aksen</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Juster ut fra venstresidene</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Midt på den vannrette aksen</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Juster bunnen av objektene til toppen av ankeret</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Juster ut fra overkantene</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Valgt hjelpelinje:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Fordel</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Gjør de vannrette avstandene mellom objektene like store</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Gjør de vannrette avstandene mellom objektene lik den angitte verdien</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Fordel høyresidene med lik avstand</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Fordel underkantene med lik avstand</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Fordel midtpunktene med lik vannrett avstand</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Gjør den loddrette avstanden mellom objektene like store</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Gjør den loddrette avstanden mellom objektene lik den angitte verdien</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Fordel venstresidene med lik avstand</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Fordel midtpunktene med lik loddrett avstand</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Fordel overkantene med lik avstand</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Avstand:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Fordel objektene med angitt avstand</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Ingen valgte</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Noen av objektene er låst</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Lås opp alle</translation>
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
        <translation>Venstrestill teksten</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Høyrestill teksten</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Midtstill teksten</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Blokkjuster teksten</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Strengt blokkjuster teksten</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Feltegenskaper</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knapp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstfelt</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Avkryssingsboks</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinasjonsboks</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listeboks</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Verktøytips:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Ramme</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Tynn</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Bred</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Heltrukket</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Prikkete</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understreket</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>skråstilt</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation type="unfinished">Innlagt</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Annet</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Skrivebeskyttet</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Nødvendig</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Ikke eksporter verdi</translation>
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
        <translation>Skjult</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Ikke skriv ut</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Ikke vis</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Utseende</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Tekst for knapp ned</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Tekst for rull rundt</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikoner</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Bruk ikoner</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Fjern</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Trykket ned</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Rull rundt</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Ikonplassering ...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Marker</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="unfinished">Inverter</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Med omriss</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Dytt</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Flerlinje</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Passord</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Maksimalt</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ikke rull</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ingen stavekontroll</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Sjekk stil:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Sjekk</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kryss</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Sirkel</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Stjerne</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Firkant</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Standarden er valgt</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Redigerbar</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Send inn skjema</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Nullstill skjema</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importer data</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Hendelse:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mus opp</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mus ned</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mus Enter</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation type="unfinished">Pekeren forlater feltet</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Ved fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Ved tilsløring</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Skript:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Rediger ...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Send til URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Send data som HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importer data fra:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Bestemmelsessted</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Til fil:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Endre ...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Side:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Posisjon:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Posisjon:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Feltet er formatert som:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Enkelt</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Nummer</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Prosentdel</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Tid</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Tallformat</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Desimaler:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Bruk valutasymbol</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Sett valutasymbol foran</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatering</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Prosentformat</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datoformat</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Tidsformat</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Egne skript</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Inntasting:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Verdien er ikke kontrollert</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Verdien skal være lik med eller større enn:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>og lik med eller mindre enn:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Selvvalgt skript som tester gyldigheten:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validér</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Verdien er ikke beregnet</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Verdien er</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>sum</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation>gjennomsnitt</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>minst</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>høyst</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>av de følgende feltene:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Velg ...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Selvvalgt utregningsskript:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Regn ut</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Skriv inn en kommaseparert liste med felter her</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Du behøver i det minste Normal-Ikonet for å bruke ikoner på knapper</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Eksempel:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Valgendring</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Skrifttyper som skal brukes med pdf 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Dette valget blir ikke brukt i pdf 1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Pdf-filer (*.pdf);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>Javascript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Bilder (*.tif *.png *.jpg *.xpm);;postscript (*.eps);;Alle filer (*)</translation>
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
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Kommentaregenskaper</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lenke</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Ekstern lenke</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Ekstern internettlenke</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Bestemmelsessted</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Pdf-dokumenter (*.pdf);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Side:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Posisjon</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Posisjon:</translation>
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
        <translation>Bruk malside</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Malside:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Bruk på</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Denne &amp;siden</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt + P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Partallsider</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt + E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;Oddetallsider</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Alle sider</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;Innenfor området</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt + W</translation>
    </message>
    <message>
        <source>to</source>
        <translation>til</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt + O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Bruk den valgte malsiden på partallsider, oddetallsider, alle sidene eller alle sidene innen et visst område</translation>
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
        <translation>&amp;Lag strekkode</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Scribus&apos; grensesnitt mot et program for strekkoder laget i ren Postscript</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Klarte ikke å åpne fila %1</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 eller 13 siffer</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 siffer</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 eller 12 siffer</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 eller 8 siffer</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 siffer</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 siffer</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation type="obsolete">9 eller 10 tall delt med tankestreker på de rette stedene</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Et variabelt antall tegn, tall og symbolene -. *$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Et variabelt antall ASCII-tegn og symboler for spesielle funksjoner, starter med et passende starttegn for det første tegnsettet. UCC/EAN-128 må ha et FNC 1-symbol rett etter starttegnet.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Et variabelt antall tegn og symbolene -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Et variabelt antall siffer. En ITF-14 er på 14 tegn og har ikke et kontrollsiffer</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Et variabelt antall siffer</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Et variabelt antall store bokstaver og tall</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Et variabelt antall heksadesimale tegn</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Strekkoden er ufullstendig</translation>
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
        <translation>Strekkodeprogram</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Strekkode</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Type:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Velg en av de tilgjengelige strekkodetegnsettene</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Den numeriske gjengivelsen av selve koden. Se hjelpeteksten nedenfor</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Nullstill strekkodeprøven</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Ta med tekst i strekkoden</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt + I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Hvis dette er valgt vil tallene komme med på strekkoden</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Beskyttede mellomrom</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt + G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Tegn piler for å forsikre om at det er plass ved siden av koden</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farger</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Bakgrunn</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt + B</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Bakgrunnsfarge – under strekkoden</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Linjer</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt + L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Strekfarge på strekkoden</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt + T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Farge på tekst og tall</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Tips og hjelp vises her</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Forhåndsvisning av resultatet i 72 ppt.</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>&amp;Kode:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>&amp;Ta med kontrollsifferet i strekkoden</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt + N</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Lag strekkoden og ta med kontrollsifferet i strekkoden</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>&amp;Ta med kontrollsifferet i teksten</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt + U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Ta med kontrollsifferet i teksten under strekkoden</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Kladdebok</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slett</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nytt innslag</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Endre navn</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Hent ...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Forhåndsvisning</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er i bruk fra før.
Velg et annet.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Velg en mappe for kladdeboka</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en mappe</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>Kladdebok (*scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Velg den kladdebokfila du vil importere</translation>
    </message>
    <message>
        <source>&amp;Import Scrapbook File...</source>
        <translation>&amp;Importer kladdebokfil ...</translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="unfinished">Hovedbok</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Bokmerker</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Flytt bokmerke</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Sett inn bokmerke</translation>
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
        <translation>Bokmerker</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Ikonplassering </translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Utseende:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Bare tekst</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Bare ikoner</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Tekst under ikonene</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Tekst over ikonene</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Tekst til høyre for ikonene</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Tekst til venstre for ikonene</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Tekst oppå ikonene</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skalering:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Alltid</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Når ikonet er for lite</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Når ikonet er for stort</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Aldri</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Hvordan skalering skjer:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporsjonalt</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Ikke-proporsjonalt</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Tilbakestill</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>System Profiles</source>
        <translation>Systemprofiler</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Fargetilpasninger:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Sansningsmessig (perseptuell)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ fargeanalyse (kolorimetri)</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Metning</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutt fargeanalyse (kolorimetri)</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Standard fargeprofil for ensfargede farger på siden</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>En fargeprofil du har laget eller mottatt fra produsenten.
Denne profilen burde være spesielt laget for din skjerm og ikke en generell profil (f.eks. sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>En fargeprofil for din skrivermodel fra produsenten.
Denne profilen burde være spesielt laget for din skriver og ikke en generell profil (f.eks. sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard fargetilpasning for din skjerm. Hvis ikke du vet hva du skal bruke, bør du velge «Relativ fargeanalyse» eller «Sansningsmessig».</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard fargetilpasning for din skriver. Hvis ikke du vet hva du skal bruke, bør du velge «Relativ fargeanalyse» eller «Sansningsmessig». </translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Skru på skjermen hvordan dine dokumentfarger vil
skrives ut basert på den valgte skriverprofilen.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>En metode for å vise farger på skjermen som kanskje ikke skrives ut korrekt.
Dette krever veldig nøyaktige profiler, og er bare en advarsel.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Kompensering for svartpunkt er en metode for å øke bildekontrasten.
Hvis du har fotografier i ditt dokumentet er det lurt å bruke dette.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Bruk fargehåndtering</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Ensfarget:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Skjerm:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>S&amp;kriver:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>&amp;Skjerm:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>S&amp;kriver:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;uler skriver på skjermen</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Marker farger utenfor &amp;fargeområdet</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Kompenser for &amp;svartpunkt</translation>
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
        <translation>Standard fargeprofil for importerte CMYK-bilder</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Standard fargeprofil for importerte RGB-bilder</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Rediger farger</translation>
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
        <translation>Nettsikkert RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Ny</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Gammel</translation>
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
        <source>Dynamic Color Bars</source>
        <translation>Dynamiske fargesøyler</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statiske fargesøyler</translation>
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
        <source>HSV-Colormap</source>
        <translation>HSV-Fargekart</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Farge&amp;modell</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Er en spotfarge</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Er registerfarge</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Du kan ikke lage en farge som heter «%1», dette navnet er reservert for gjennomsiktige farger.</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>Fargenavnet er i bruk allerede</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation>Hvis du velger dette vil dette bli trykket på alle platene. Registerfarger bruker for passmerker, som for eksempel skjæremerker, registermerker og lignende. Disse brukes vanligvis ikke i selve sideoppsettet.</translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
        <translation>Ved at velge dette vil gjøre om denne fargen til en spotfarge, og dermed lage enda et punkt når det blir laget plater eller separasjoner. Det er ofte brukt, når en logo eller anden farve skal være helt nøyaktig eller ikke kan lages ved hjelp av trykkfargene i CMYK. Metalliske og fluorescerende farver er gode eksempler på farger, som det ikke er særlig let å lage med trykkfarger i CMYK.</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Du kan se et miniatyrbilde hvis du trykker og holder nede den høyre museknappen. Ved hjelp av «Sett inn» setter du inn en glyff i avsnittet nedenfor, og Delete-tasten fjernet den du satte inn sist.</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Velg Tegn:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Skrifttype:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Tegnklasse:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Sett &amp;inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Tøm</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Sett inn tegnene etter markøren i teksten</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Fjern det du har valgt.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Fullt tegnsett</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation type="unfinished">Basic Latin</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation type="unfinished">Latin-1 Supplement</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Utvidet Latin A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Utvidet Latin B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Generell tegnsetting</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Hevet og senket tekst</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Valutategn</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Bokstavlignende symboler</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Tallformer</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Piler</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Matematiske operatorer</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Tegn bokser</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blokkelementer</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometriske former</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Forskjellige symboler</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbats</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Småform-varianter</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligaturer</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Spesielle tegn</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Gresk</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Utvidet gresk</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Kyrillisk</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Tillegg for kyrillisk</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabisk</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Utvidet Arabisk A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Utvidet Arabisk B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebraisk</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation>&amp;Sett inn kode</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>Lukk dette dialogvinduet og gå tilbake til å redigere teksten</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Skriv inn en firesifret unicode-verdi direkte her</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Det mangler glyffer</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Teksten er for stor for ramma</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objektet er ikke på en side</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Manglende bilde</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Bildet har en oppløsning på under %1 ppt</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objektet er gjennomsiktig</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objektet er en pdf-kommentar eller et pdf-felt</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objektet er en pdf-fil som er satt inn</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Alt i orden</translation>
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
        <translation>Problemer funnet</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Forhåndssjekk</translation>
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
        <translation>Gjeldende profil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Overse feil</translation>
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
        <translation>Velg stil</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Tilgjengelige stiler</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en mappe</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Henter ...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Finner ikke alle filene som skal samles for fila:
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation type="unfinished">Klarte ikke å samle fila: 
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Farger</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Lag en kopi</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Fjern ubrukte</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Fargesett</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Nåværende fargesett:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Lagre fargevalget</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Velg et fargesett du vil laste inn</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Lagre nåværende fargesett</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Slett ubrukte farger fra dette dokumentets fargesett</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importer farger til dette settet fra et annet dokument</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Lag en ny farge i dette settet</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Rediger den valgte fargen</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Lag en kopi av den valgte fargen</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Slett den valgte fargen</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Sett det valgte fargesett som standard fargesett</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Velg et navn</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *.scd.gz);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter  (*.sla *.scd);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Ny farge</translation>
    </message>
    <message>
        <source>If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected. What this means is the color many not be able to be printed exactly as displayed on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management.</source>
        <translation>Hvis fargehåndtering er på, så vil en advarselstrekant gi advarsel hvis fargen kan være utenfor fargeområdet til den skriverprofilen du har valgt. Det betyr at fargen kanskje ikke kan skrives ut helt slik den vises på skjermen. Spotfarger antydes med en rød sirkel. Registerfarger vil ha et registreringsmerke ved siden av seg. Mer hjelp om advarsler angående fargeområdet (gamut) finner du på hjelpesidene på nettet under «Color Management» (http://wiki.scribus.net).</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Ensfarget</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Nabofarger (analogt)</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Komplementærfarger</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Nabofargene og deres komplementærfarger</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Treklang</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Firklang (dobbelt komplementært)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Grunnfarger</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Ensfarget, lys</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Ensfarget, mørk</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>Første nabofarge</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>Andre nabofarge</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>Første splittfarge</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>Andre splittfarge</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>Tredje splittfarge</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>Fjerde splittfarge</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>Første treklangfarge</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>Andre treklangfarge</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>Første firklangfarge (komplementærfargen)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>Andre firklangfarge (andre fargepar)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>Tredje firklangfarge (andre fargepar, farge to)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Color Wheel</source>
        <translation>Fargehjul</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farge</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
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
        <translation>Velg metode:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Vinkel (0–90 grader):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Hent inn farger</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Erstatt farger</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Hent de fargene som er opprettet inn i dokumentfargene</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Erstatt de opprettede fargene i dokumentfargene</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Ikke endre fargene</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Slår sammen fargene</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Feil:</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Åpner nå fargebehandleren.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Hente inn farger</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation>Normalt syn</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Full fargeblindhet</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Synsskade:</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>&amp;Lag farge ...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>&amp;Importer en eksisterende farge ...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>&amp;Hent inn farger</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>&amp;Erstatt farger</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Avslutt</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farge</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Forskjellen mellom de valgte verdiene og de som er talt. Se dokumentasjonen for mer informasjon.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Trykk på fargehjulet for å få grunnfargen. Fargetonen er i fargemodellen HSV.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Eksempel på et fargeskjema</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Velg en av metodene for å lage et fargeskjema</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Farger i ditt fargeskjema</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Her kan du simulere vanlige synsfeil. Velg fra lista.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Ny farge</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Finner ikke den valgte fargen. Sannsynligvis har du valgt svart, grått eller hvitt. Disse fargene kan ikke prosesseres.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>&amp;Fargekomponenter ...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Protanopia (Rødblindhet)</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Deuteranopia (Grønnblindhet)</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Tritanopia (Blåblindhet)</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Fargen %1 finnes allerede</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Fargen %1 er lagt til.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Fargehjul ...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Hjelpeprogram for fargeoppsettet</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Fargevelger med fargeteori inkludert</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Bruk</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Enkeltsidig</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Dobbeltsidig</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>Tredelt</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>Firedelt</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Venstre side</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>I midten</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>I midten mot venstre</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>I midten mot høyre</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Høyre side</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Normal Left</source>
        <translation>Normal, venstre</translation>
    </message>
    <message>
        <source>Normal Middle</source>
        <translation>Normal, midten</translation>
    </message>
    <message>
        <source>Normal Right</source>
        <translation>Normal, høyre</translation>
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
        <translation>Mars</translation>
    </message>
    <message>
        <source>April</source>
        <translation>April</translation>
    </message>
    <message>
        <source>May</source>
        <translation>Mai</translation>
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
        <translation>Desember</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Metningsgrad:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Gjennomsiktighet:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Vannrett skravering</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Loddrett skravering</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonal skravering</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Kryssdiagonal gradering</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Radial skravering</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Rediger egenskapene til strekfargen</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Rediger egenskapene til fyllfargen</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Fargemetning</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation type="unfinished">Normal eller skravering fyllmetode</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Angi gjennomsiktigheten til den valgte fargen</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Fri lineær skravering</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Fri radial skravering</translation>
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
        <source>Move Vector</source>
        <translation>Flytt vektor</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Flytt begynnelsen av gradientvektoren med venstre museknapp nede og flytt slutten av gradientvektoren med høyre museknapp nede.</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Valg for CVS-import</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Feltskille:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Verdiskille:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Første rad er overskrift</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
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
        <source>Printer Options</source>
        <translation>Skriverinnstillinger</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Sideoppsett</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle sidene</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Bare partallsider</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Bare oddetallsider</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Speil</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Retning</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation type="unfinished">N-Up-utskrift</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Side per ark</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Sider per ark</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Innstilling</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Verdi</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Denne ruta viser ulike valg for utskrift ved hjelp av Cups. Hvilke valg som er tilgjengelig vil avhenge av hvilken skriverdriver du har. Du kan bekrefte at du har støtte for Cups ved å velge «Hjelp – Om Scribus». Se etter C-C-T, som står for henholdsvis: C=CUPS C=littlecms T=TIFF-støtte. Hvis et bibliotek ikke er støttet er det markert med *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Koding:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Flytter til din standardmappe for dokumenter. Denne kan du endre i oppsettet.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Komprimer fila</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Bygg inn skrifttypene</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>Velg fargekomponent</translation>
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
        <translation>Bruk &amp;RGB</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>Bruk C&amp;MYK</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>Bruk &amp;HSV</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Avbrutt av brukeren</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Fjern farge</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Slett farge:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Erstatt med:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Slett sider</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>til:</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Slett fra:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Slett stilen</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Slett stilen:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Erstatt med:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Klarte ikke å importere</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Klarte ikke å importere Word-dokumentet 
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformasjon</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Tittel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Forfatter:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Nøkkelord:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Beskrivelse:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Utgiver:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Bidragsytere:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Dato:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Type:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Format:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Referanse:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Kilde:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Språk:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation type="unfinished">&amp;Forhold:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Dek&amp;ning:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Rettigheter:</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Mer &amp;informasjon</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>En person eller organisasjon som er ansvarlig for å gjøre dokumentet tilgjengelig</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>En person eller organisasjon som er ansvarlig for å bidra til dokumentets innhold</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>En dato knyttet til en hendelse i dokumentets livssyklus, i formatet YYYY-MM-DD, som angitt i ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Formen eller genren for dokumentets innhold, f.eks kategorier eller funksjoner.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>En entydig referanse til dokumentet innenfor en gitt kontekst, som ISBN eller URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>En referanse til et dokument som det nåværende dokument stammer fra, f.eks ISBN eller URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>En referanse til et lignende dokument, kanskje ved bruk av en formell henvisning som ISBN eller URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Avgrensning for dokumentets innhold, ta gjerne med sted, tid og juridisk virkningsområde</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informasjoner omkring rettigheter for og til dokumentet, f.eks opphavsrett, patenter og varemerker</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;t</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Den personen eller organisasjonen som hovedsaklig er ansvarlig for å ha laget innholdet. Dette feltet kan settes inn i Scribus-dokumentet som en referanse og også legges inn som metadata i en pdf-fil.</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Et navn på dokumentet. Navnet kan tas med i Scribus-dokumentet som en referanse og også legges inn som metadata i en pdf-fil.</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>En kort eller generell beskrivelse av innholdet i dokumentet. Dette vil bli lagt inn i pdf-fila som Scribus lager.</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Et emne eller tema for dette dokumentet i stikkordsform. Kan legges inn i pdf-fila og hjelpe brukeren, da informasjonen er søkbar.</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Fysisk eller digital informasjon om dokumentet. For eksempel mediatype og størrelse.  RFC2045,RFC2046 for MIME-typer kan også være nyttig.</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Hvilket språk dokumentet er skrevet på. Ofte er dette en språkkode i ISO-639-formatet. Den kan også ha en ekstra bindestrek med en ISO-3166-landskode etter seg, for eksempel en-GB, fr-CH.</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Legg til en bolk for sidetall. Den nye bolken vil bli lagt til etter den valgte bolken.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Slett den valgte bolken.</translation>
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
        <translation>&lt;b&gt;Navn:&lt;/b&gt; Valgfritt navn på bolken, for eksempel oversikt.&lt;br/&gt;
&lt;b&gt;Vist:&lt;/b&gt; Velg dette for å vise sidetallene i denne bolken hvis det er en eller flere tekstrammer som er satt opp til å gjøre dette.&lt;br/&gt;&lt;b&gt;Fra:&lt;/b&gt; Sideoversikten som denne bolken skal starte ved.&lt;br/&gt;&lt;b&gt;Til:&lt;/b&gt; Den sideoversikten denne bolken skal stoppe ved.&lt;br/&gt;&lt;b&gt;Stil:&lt;/b&gt; Velg hvilken sidetallstil du vil bruke&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; Sideoversikten med sideområdet stilen skal starte ved. Hvis for eksempel Start er «2» og Stilen er «a,b,c, ...» så vil sidenummereringa starte med b. For første bolken i dokumentet vil dette erstatte det tidligere «Første sidetall» i det nye filvinduet.</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Sidetallet er utenfor det gyldige intervallet</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Det sidetallet du skrev inn er ikke innenfor de sidetallene dokumentet har (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Dokumentbolker</translation>
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
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
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
        <translation>Knyttet til</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Er overordnet til</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Er underordnet til</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Tekstrammer</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Bilderammer</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Boolsk</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Heltall</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Streng</translation>
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
    <message>
        <source>Real Number</source>
        <translation>Virkelig tall</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Egenskaper for dokumentelementene</translation>
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
        <translation>Verdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Forhold</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Forhold til</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Legg automatisk til</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt + L</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Skriverinnstillinger</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
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
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Skriv inn en kommaseparert liste med tegn der
tegnet * står for alle sidene, og 1-5
for en rekke sider eller en enkeltside.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Valg av skriver</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Valg ...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fil:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Annen utskriftskommando</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Kommando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Område</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Skriv ut &amp;alt</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Skriv ut &amp;gjeldende side</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>&amp;Utskriftsområde</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Antall kopier:</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Skriv ut</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Skriv ut vanlig</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Del opp utskrifta</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Skriv ut i farger hvis det er tilgjengelig</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Skriv ut i svart-hvitt</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>postscript nivå 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>postscript nivå 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>postscript nivå 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Speilvend siden(e) vannrett</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Speilvend siden(e) loddrett</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Velg mediastørrelse</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farge</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Legg til under fargefjerning</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Gjør om spotfarger til prosessfarger</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Bruk ICC-profiler</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Flere valg</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Forhåndsvis ...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Velg postscript-nivå. Hvis du bruker nivå 1 eller 2 kan det gi veldig store filer</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Postscript-filer (*.ps);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Bruk en annen utskriftbehandler, for eksempel «Kprinter» eller «gtklp», for å få flere skrivervalg</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En metode for å skru av noen av gråsjatteringene som består av cyan, gul og magenta og bruke svart isteden. UCR berører for det meste de delene av bildet som er nøytrale og/eller har mørke toner som ligger nært opp mot grått. Hvis du bruker dette, kan det gi bedre utskrift for noen bilder, men det må prøves ut for hvert enkelt tilfelle. UCR gjør det mindre sannsynlig at det skal oppstå overmetning av blekk i fargene cyan, magenta og gul (CMY).</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Gjør om spotfarger til sammensatte farger. Med mindre du skal skrive ut spotfarger på en profesjonell skriver er det best å skru dette på.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Lar deg bruke ICC-profiler ved utskrift hvis fargehåndtering er skrudd på.</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Lar deg velge størrelsen på Postscript-fila. Anbefales ikke med mindre skriveren krever det.</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Juster til sidemargene</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Klarte ikke å finne skriverinnstillingene</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Ikke vis objekter utenfor margene på den utskrevne siden</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Klarte ikke å importere fila %1.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Alvorlig feil</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Feil</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation>Importerer postscript</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>Analyserer postscript</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Oppretter elementer</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>Klarte ikke å gjøre om %1 av bildene.</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Rediger stil</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linjeavstand</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatorer og innrykk</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Navnet på din avsnittsstil</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Skrifttypen til den markerte teksten eller objektet</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Farge på tekstfyll</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Farge på tekststrek</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Bestemmer høyden, i antal linjer, på en innfelt forbokstav</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Avstand over avsnittet</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Avstand under avsnittet</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linjer:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Avstander</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast linjeavstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatisk linjeavstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Juster til de vannrette linjene</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Innfelt forbokstav</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Avstand til teksten:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Forhåndsvisning av avsnittsstilen</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Angi hvor stor avstand det skal være mellom den innfelte forbokstaven og selve teksten</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Skru av eller på eksempelteksten som viser denne avsnittsstilen</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Det finnes allerede en stil med dette navnet</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Dette kan gjøre det lettere å lese stiler i lyse farger</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Justert tegnavstanden (tracking)</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Avstand til tegnenes grunnlinje</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Trykk og hold museknappen nede for å angi linjeavstanden.</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automatisk</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Tekstbehandler</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åpne ...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Lagre og avslutt</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Avslutt uten å lagre</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Klipp &amp;ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Tøm</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Hent feltnavn</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>Javascript (*.js);;Alle filer (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Bildeeffekter</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Innstillinger:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Metningsgrad:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Lyshetsgrad:</translation>
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
        <translation>Verdi:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Bruk plakateffekt:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Tilgjengelige effekter</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Slør til</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Lyshetsgrad</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Fargelegg</translation>
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
        <translation type="unfinished">Inverter</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Plakateffekt</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Gjør skarpere</translation>
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
        <translation>Effekter som er i bruk</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Velg en mappe for eksport</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Alle sidene</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Bytt utskriftsmappe</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Utskriftsmappa, stedet der du lagrer bildene dine.
Navnet på den eksporterte fila vil være «dokumentnavn-sidenummer.filtype»</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Bare eksporter den gjeldende siden</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Tilgjengelige eksportformater</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Eksporter til mappe:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Bildeformat:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kvalitet:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Eksporter som bilde(r)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Oppløsning:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppt</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Område</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Gjeldende side</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Område</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Eksporter en rekke sider</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Skriv inn en kommaseparert liste med tegn der
tegnet * står for alle sidene, og 1-5
for en rekke sider eller en enkeltside.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Eksporter alle sidene</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Bildeoppløsning
Bruk 72 ppt på bilder som bare skal vises på skjerm</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Bildekvalitet. 100% er best og 1% er lavest kvalitet</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Størrelse på bildene. 100% gir ingen endring, 200% gir dobbelt så store bilder osv.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Utvidede bildeegenskaper</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Gjør mørkere</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Gjør lysere</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Fargetone</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Metning</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farge</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Fargeglød</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Mangfoldiggjør</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Skjerm</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Oppløse</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Legg over</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hardt lys</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Bløtt lys</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Forskjell</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Utelukkelse</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Utelukk farger</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Brente farger</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Utelukkelse</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Blending</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Grad av gjennomsiktighet:</translation>
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
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Ikke bruk noen stier</translation>
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
        <source>Title:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Uten navn</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Forfatter:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukjent</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus-dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Oppløsning:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>Ppt</translation>
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
        <translation>Fargeområde:</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Noen skrifttyper i dette dokumentet har blitt erstattet:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>  ble erstatet av: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Tilgjengelige skrifttyper</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Erstatning av skrifttyper</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Andre stier</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Navn på skrifttypen</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Tilgjengelige skrifttyper</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Erstatning</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Erstatning av skrifttyper</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Andre &amp;stier</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en mappe</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Legg til ...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Fjern</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Navn på skrifttypen</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Bruk skrifttype</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Inkluder i:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Sti til skrifttypefil</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Font search paths can only be set when there are no documents open. Close any open documents, then use File -&gt;Preferences &gt; Fonts to change the font search path.</source>
        <translation>Du kan bare velge en søkesti for skrifttyper når ingen dokumenter er åpne. Lukk alle åpne dokumenter, bruk så «Fil &amp;ndash; egenskaper» for å endre søkestien til skrifttypene.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Lukk forhåndsvisninga</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Start søk</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Størrelse på den valgte skrifttypen</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Vår sære Zulu fra badeøya spilte jo whist og quickstep i min taxi</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Bruker</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>System</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>En prøve vil bli vist når du slipper knappen</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>I dette feltet kan du foreta et hurtigsøk etter en skrifttype. Søket skiller ikke mellom store og små bokstaver. Du kan også bruke vanlige jokertegn (*, ?, [...]). For ekssempel vil «t*» vise alle skrifttypene som begynner med t eller T. *bold* vil vise alle skrifttyper med ordet bol, bolder e.l. i navnet.</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>
font preview</comment>
        <translation type="obsolete">Legg den valgte skrifttypen inn under stilene, menyen for skrifttyper</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished">Legg den valgte skrifttypen inn under stilene, menyen for skrifttyper</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Forhåndsvisning av skrifttyper</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Hurtigsøk</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søk</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt + S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Navn på skrifttypen</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Doc</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Tilgang</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Skriftstørrelse:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Eksempeltekst som skal vises</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>&amp;Angi</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt + T</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Tilbakestill teksten</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Forhåndsvisning av skrifttyper ...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Dialogvindu for forhåndsvisning av skrifttyper</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Sortere, lete i og se på de tilgjengelige skrifttypene</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Skrifterstatning</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Opprinnelig skrifttype</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Erstatningsskrift</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Gjør disse erstatningene permanente</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Dette dokumentet inneholder noen skrifttyper som ikke er installert på ditt system. Du må velge en erstatning for dem. Hvis du velger «Avbryt» stopper du dokumentet i å lastes inn.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Avbryt erstatninga av skrifttypene og ikke last inn dokumentet.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Hvis du skrur på dette, vil Scribus også senere erstatte skriftyper på samme måte som du angir her. Du kan endre dette under «Fil – Innstillinger – Skrifttyper»</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Hvis du velger «Greit» og så lagrer vil disse erstatningene bli brukt også senere og på andre dokumenter.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Posisjon:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation type="unfinished">Her kan du legge til, endre eller fjerne fargeskift</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Oppsett av hjelpelinjer</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Vannrette hjelpelinjer</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Loddrette hjelpelinjer</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Posisjon:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Posisjon:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>Lås hje&amp;lpelinjene</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Kolonner og rader – Automatiske hjelpelinjer</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;Rader:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>Rad&amp;avstand</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Kolonne&amp;avstand</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>I forhold til:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Si&amp;den</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;Margene</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Utvalget</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>&amp;Oppdater</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Angi hjelpelinjene i dokumentet. Redigeringsvinduet for hjelpelinjer lukkes ikke, men endringene lagres.</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>&amp;Bruk på alle sidene</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hjelpelinje</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>Enhet</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Forhåndsvis</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>Det finnes allerede en tom hjelpelinje (0.0)</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Beklager, ingen håndbok er tilgjengelig. Ta en titt på http://docs.scribus.net for oppdaterte håndbøker, og www.scribus.net for nye versjoner av Scribus.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Innhold</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lenke</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus-hjelp på Internett</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Innhold</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søk</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Sø&amp;k</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nytt</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>Slett a&amp;lle</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>Bok&amp;merker</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Skriv &amp;ut ...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Avslutt</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Søket skiller ikke mellom store og små bokstaver</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Finn</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Søk etter:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Nytt bokmerke</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Navn på bokmerke:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Finn ...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Finn &amp;neste</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Finn f&amp;orrige</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Legg til </translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>S&amp;lett alle</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bokmerker</translation>
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
        <translation>Mulig orddeling</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Godta</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Hopp over</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Lengden av det minste ord som deles.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Høyeste antall etterfølgende orddelinger.
0 betyr et ubegrenset antall.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Språk:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Minste ord:</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;Orddelingsforslag</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Bruk orddeling automatisk mens du skriver</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Antall påfølgende orddelinger som er tillatt:</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>En dialogboks vil vise alle mulige orddelinger for hvert ord hvis du bruker funksjonen «Ekstra – Orddel tekst».</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Skrur på automatisk orddeling mens du skriver.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Bildeinformasjon</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Generell informasjon</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Dato/Tid:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Har en innebygd profil</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Profilnavn:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Har innebygde stier:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Inneholder lag:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>Exif-informasjon</translation>
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
        <translation>Brukerkommentar</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kameramodell:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Kameraforhandler:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beskrivelse:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Opphavsrett:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Skannermodell:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Forhandler av skanner:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importer &amp;EPS/PS ...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Importer EPS-filer</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Kan importere de fleste Eps-filer ved å gjøre om vektordataene til Scribus-objekter.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>PDF</source>
        <translation>Pdf</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Sett inn side</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>før side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>etter side</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>på slutten</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>side(r)</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Sett &amp;inn</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Malside:</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Malsider</translation>
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
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Flytt objektene sammen med siden</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Sett inn tabell</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Antall rader:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Antall kolonner:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Rediger Javascript</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nytt skript</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Rediger ...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Legg til ...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nytt skript:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;nei</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Er du sikker på at du vil slette dette skriptet?
</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation type="unfinished">Legger til et nytt skript og angir en funksjon med samme navn som det. Hvis du vil bruke skriptet som «Open Action»-skript, så pass på at du ikke endrer navn på funksjonen.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Nåværende hurtigtast</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Velg en hurtigtast for denne handlinga</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>Alt + Shift + T</translation>
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
        <translation>Shift +</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation>Alt +</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation>Ctrl +</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen hurtigtast</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Selvvalgt hurtigtast</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Angi &amp;hurtigtast</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Innlastbart oppsett for hurtigtaster</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Bruk</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importer ...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Eksporter ...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Tilbakestill</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Oppsettet for hurtigtaster kan nå lastes inn</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Last inn det valgte oppsettet for hurtigtaster</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importer et oppsett for hurtigtaster til dette oppsettet</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Lagre hurtigtast-oppsettet i en importerbar fil.</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Gå tilbake til Scribus&apos; standard hurtigtast-oppsett</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Oppsettsfiler for hurtigtaster i XML (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>Denne tastekombinasjonen er allerede i bruk</translation>
    </message>
    <message>
        <source>Meta</source>
        <translation>Metatast</translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation>Metatast +</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Slett lag</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Vil du også slette alle objektene på dette laget?</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Legg til et nytt lag</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Slett lag</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Hev lag</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Senk lag</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Rediger linjestiler</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Lag en &amp;kopi</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Er du sikker på at du vil slette denne stilen?</translation>
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
        <translation>Strekbredde:</translation>
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
        <translation>Flate</translation>
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
        <translation>Spisse</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Skrå</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Heltrukken linje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Linje av streker</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Prikket linje</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Linje av bindestrek og punktum</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Linje av bindestrek punktum punktum</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Velg Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Forfatter:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Hent mer:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML-fil:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Avsnitt:</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt + O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
    </message>
</context>
<context>
    <name>MarginDialog</name>
    <message>
        <source>Margin Guides</source>
        <translation>Hjelpelinjer for sidemargene</translation>
    </message>
    <message>
        <source>Manage Page Properties</source>
        <translation>Endre sideoppsettet</translation>
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
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Flytt objektene sammen med sidene de er på</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nedre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Øvre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Avstanden mellom øvre sidemarg og kanten av papiret</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Avstanden mellom nedre sidemarg og kanten av papiret</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Indre</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Ytre</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Forhåndsvalgte sideoppsett</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Bruk disse sidemargene på alle sidene</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Bruk endringene av sidemargene på alle de eksisterende sidene i dokumentet</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Avstand mellom venstre sidemarg og kanten av papiret. Hvis du har valgt motstående sider, så kan du bruke denne avstanden til å få riktig innbindingsmarg.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Avstand mellom høyre sidemarg og kanten av papiret. Hvis du har valgt motstående sider, så kan du bruke denne avstanden til å få riktig innbindingsmarg.</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Sidemarg for skrivere ...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Importer sidemargene for de valgte sidene fra de tilgjengelige skriverne.</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Endre malsidene</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Er du sikker på at du vil slette denne malsiden?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Ny malside</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Ny malside</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 av </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Lag en kopi av den valgte malsiden</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Slett den valgte malsiden</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Legg til en ny malside</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Importer malsider fra et annet dokument</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Ny malside %1</translation>
    </message>
    <message>
        <source>Unable to Rename Master Page</source>
        <translation>Klarer ikke å gi nytt navn til malsiden</translation>
    </message>
    <message>
        <source>The Normal page is not allowed to be renamed.</source>
        <translation>Det er ikke mulig å gi gi nytt navn til malen «normal».</translation>
    </message>
    <message>
        <source>Rename Master Page</source>
        <translation>Gi nytt navn til malside</translation>
    </message>
    <message>
        <source>New Name:</source>
        <translation>Nytt navn</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Lag flere kopier</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Antall kopier:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Vannrett forskyvning:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Loddrett forskyvning:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Avstander</translation>
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
        <translation>Lengde:</translation>
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
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importer side(r)</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> fra 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Lag side(r)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> fra %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Importer malsider</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Fra dokument:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>E&amp;ndre ...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importer side(r):</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Importer malsider</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Skriv inn en kommaseparert liste med tegn der
tegnet * står for alle sidene, og 1-5
for en rekke sider eller en enkeltside.</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Etter side</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>På slutten</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Manglende skrifttype</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Skrifttypen %1 er ikke installert.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>i stedet</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Flytt sider</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopier side</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Flytt side(r):</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Flytt side(r)</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Etter side</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>På slutten</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Til:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Antall kopier:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation type="unfinished">Geometri</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Målepunkt:</translation>
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
        <source>Distance of Text</source>
        <translation>Avstand til teksten</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Egenskaper for tekstkurven</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Vis kurve</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Start forskyvning:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Avstand fra kurven:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Inn-profil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Fargetilpasning:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Sansningsmessig (perseptuell)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ fargeanalyse (kolorimetri)</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Metning</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutt fargeanalyse (kolorimetri)</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Venstre punkt</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Endepunkter</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spiss</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Skrå</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flat</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkantet</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Cellelinjer</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Linje øverst</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linje til venstre</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Linje til høyre </translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Linje nederst</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Navn på det valgte objektet</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Vannrett posisjon på det nåværende målepunktet</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Loddrett posisjon på det nåværende målepunktet</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Bredde</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Høyde</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotasjon av objekt ved det nåværende målepunktet</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Det punktet målingene eller rotasjonsvinklene regnes utfra</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Velg øverst til venstre som målepunkt</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Velg øverst til høyre som målepunkt</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Bruk nederst til venstre som målepunkt</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Bruk nederst til høyre som målepunkt</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Bruk midten som målepunkt</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Speil vannrett</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Speil loddrett</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Flytt et nivå opp</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Flytt et nivå ned</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Plasser fremst</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Plasser bakerst</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Indikerer nivået objektet er på, 0 betyr at objektet er på bunnen</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Lås objektet eller fjern låsing</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Lås eller fjern låsing av objektets størrelse</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation type="obsolete">Skru av eller på utskrift av objektet</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Skrifttype på den valgte teksten eller objektet</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skaleringsbredde på tegnene</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Fargemetning på tekststreken</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Fargemetning på tekstfyllet</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Linjeavstand</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilen som brukes på det gjeldende avsnittet</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Endre innstillingene for venstrepunkter eller sluttpunkter</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Linjemønster</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Linjetykkelse</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation type="unfinished">Form for linjesammenføyninger</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation type="unfinished">Form for linjeendelser</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Linjestil på det gjeldende objektet</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Velg rammeform ...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Endre rammeform ...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Angi radiusen på hjørneavrundinga</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Antall antall kolonner i tekstramma</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Bytt mellom avstand og kolonnebredde</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Avstand mellom kolonnene</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Avstand mellom teksten og overkanten av ramma</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Avstand mellom teksten og bunnen av ramma</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Avstand mellom teksten og venstre rammekant</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Avstand mellom teksten og høyre rammekant</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Endre tabulatorinnstillingene til tekstramma ...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Tillat bildet å ha en annen størrelse enn ramma</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Vannrett forskyving av bildet innenfor ramma</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Loddrett forskyving av bildet innenfor ramma</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Skaler bildet vannrett</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Skaler bilder loddrett</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Behold X- og Y-skaleringa</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Behold størrelsesforholdet</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Tilpass bildet til ramma</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Bruk bildets størrelse istedenfor rammas</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Kildeprofil til bildet</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Fargetilpasning for bildet</translation>
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
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Bilde</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linje</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Farger</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Posisjon:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Posisjon:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Rediger form ...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>&amp;Runde hjørner:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Avstand:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Øverst:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nederst:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulatorer ...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>La teksten &amp;flyte rundt rammene</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Bruk &amp;grenseboksen</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Bruk &amp;konturlinjene</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;il:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>Sp&amp;råk:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Fri skalering</translation>
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
        <translation>Skaler &amp;til rammestørrelsen</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roporsjonal</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Målepunkt:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Li&amp;njetype:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Strek&amp;bredde:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Ka&amp;nter:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation type="unfinished">&amp;Endelser:</translation>
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
        <translation>Orddelingsspråk for ramma</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Skriv fra høyre mot venstre</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fast linjeavstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatisk linjeavstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Juster til de vannrette linjene</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Faktisk X-ppt:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Faktisk Y-ppt:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Startpil:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Sluttpil:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Avstand til tegnets grunnlinje</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Skaler tegnhøyden</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuell bokstavavstand (tracking)</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Navnet «%1» er allerede i bruk. &lt;br/&gt;Velg et annet.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Fyll-regel</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation type="unfinished">Like-ulike</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Ikke null</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Farge på tekststreken og/eller tekstskyggen, avhengig av hva som er valgt. Hvis begge er valgt så vil de ha samme farge.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Farge på den valgte teksten. Hvis du har valgt omriss også, vil denne fargen bli fyllfargen. Hvis du har valgt«Tekstskygge», så vil dette bli den øverste fargen.</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object. The options below define how this is enabled.</source>
        <translation>Få tekst i lavere rammer til å flyte rundt objektet. Funksjonen nedenfor angir hvordan dette skal skje.</translation>
    </message>
    <message>
        <source>Use the bounding box, which is always rectangular, instead of the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Bruk grenseboksen, som alltid er rektangulær, istedenfor formen på ramma til å avgrense tekstflyten under objektet. </translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow of text frames below the object. </source>
        <translation>Bruk en annen linje, opprinnelig basert på rammens form for tekstflyt av tekstrammer bak objektet.</translation>
    </message>
    <message>
        <source>Click and hold down to select the line spacing mode.</source>
        <translation>Trykk og hold museknappen nede for å angi linjeavstanden.</translation>
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
        <translation>Rediger stil</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flate</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Firkantet</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Runde</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spisse</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Skrå</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund sammenføyning</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Strekbredde:</translation>
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
        <translation>Greit</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Heltrukken linje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Linje av streker</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Prikket linje</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Linje av bindestrek punktum</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Linje av bindestrek punktum punktum</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Navnet «%1» er allerede i bruk.&lt;br/&gt;Velg et annet.</translation>
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
        <translation>Framgang</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Framgang totalt:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Mitt &amp;programtillegg</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus – Mitt programtillegg</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Programtillegget virket</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Nytt dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Hjelpelinjer for sidemargene</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Sidestørrelsen på dokumentet, enten en standardstørrelse eller en selvvalgt størrelse</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Sideretninga på dokumentet</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Bredden på sidene i dokumentet, kan bare endres hvis du har valgt en selvvalgt sidestørrelse</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Høyden på sidene i dokumentet. Kan bare endres hvis du har valgt en selvvalgt sidestørrelse</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard måleenhet for dokumentet</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Lag tekstrammer automatisk når nye sider legges til</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Antall kolonner i tekstrammene som lages automatisk</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Avstand mellom de automatisk opprettede kolonnene</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Sideretning:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Standard &amp;måleenhet:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Automatiske t&amp;ekstrammer</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Avstand:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Ikke vis denne dialogboksen igjen</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Antall sider i dette dokumentet</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>Antall s&amp;ider:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Nytt dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Åpne et &amp;eksisterende dokument</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Nylig åpnede &amp;dokumenter</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Ny &amp;fra mal ...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Last inn dokumenter med forhåndsvalgt oppsett</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Opprett et dokument fra en mal som er laget av andre eller av deg selv. Slik kan du opprettholde samme stiler i flere dokumenter.</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Noder</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Flytt noder</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Flytt kontrollpunkter</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Legg til noder</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Slett noder</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Nullstill kontrollpunktene</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Nullstill dette kontrollpunktet</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolutte koordinater</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Posisjon:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Posisjon:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Rediger &amp;konturlinjene</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Nullstill kontu&amp;rlinjene</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Avslutt r&amp;edigeringa</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Beveg kontrollpunktene individuelt</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Beveg kontrollpunktene symmetrisk</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Åpen en Mangekant eller skjærer en bezierkurve</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Lukk denne bezierkurven</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Speil stien vannrett</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Speil stien loddrett</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Beskjær stien vannrett til venstre</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Beskjær stien loddrett opp</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Beskjær stien loddrett ned</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Roter stien mot klokka</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Roter stien med klokka</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Forstørr stiens størrelse med viste %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Rotasjonsvinkel</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Rediger konturlinjene</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Tilbakestill konturlinja til rammas opprinnelige form</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Beskjær stien vannrett til høyre</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Kryss av her for å måle koordinatene i forhold til siden. Hvis ikke vil koordinatene stå i forhold til objektet.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Forminsk størrelsen på stien med den viste prosentverdien</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Forminsk størrelsen på stien med den viste verdien</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Forstørr størrelsen på stien med den viste verdien</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>Prosentvis forstørrelse eller forminskning</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Forstørrelses- eller forminskelsesgrad</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Dette dokumentet ser ikke ut til å være et OpenOffice.org Draw-dokument.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importer en &amp;OpenOffice.org Draw-fil ...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importerer OpenOffice.org Draw-filer</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importerer de fleste OpenOffice.org Draw-filene til det gjeldende dokumentet ved å gjøre om vektordataene til Scribus-objekter.</translation>
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
        <translation>Fila inneholder egenskaper som ikke er støttet</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Valg for importering av OpenDocument-filer</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Erstatt avsnittsstilene</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Hvis dette er på, vil de stilene som allerede finnes i dokumentet bli erstattet.</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Slå sammen avsnittsstilene</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Slå sammen avsnittsstiler ut fra hvilke egenskaper de har. Dette vil gi færre stiler og likevel beholde stilegenskapene, selv om stilene i det opprinnelige dokumentet har andre navn.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Bruk filnavnet som første del av stilnavnet</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Bruk dokumentnavnet som en del av stilnavnet i Scribus.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ikke spør igjen</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Gjør disse innstillingene til standardinnstillinga, og ikke spør igjen når du laster inn andre filer i samme format (OASIS OpenDocument).</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
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
        <translation>Scribus-dokument</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x-dokument</translation>
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
        <translation>Linjebredde</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Lagre som pdf</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>&amp;Send til fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>E&amp;ndre ...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Lag en fil for &amp;hver side</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Pdf-filer (*.pdf);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Dette lar deg opprette en egen pdf-fil for hver av sidene i dokumentet. Sidetallene legges til som en del av filnavnet. Dette er nyttig når man skal sende pdf-filer til et trykkeri.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Lagre-knappen vil bli utilgjengelig hvis du prøver å eksportere som PDF/X-3 og informasjonsteksten vil mangle på fanebladet for PDF/X-3.</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>Lagrer som pdf</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>Eksporterer malsidene:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Eksporterer sidene:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Eksporterer elementene på den gjeldende siden:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Forhåndsvisning av utskrift</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Viser gjennomsiktighet og gjennomsiktige objekter i dokumentet ditt. Trenger Ghostscript 7.07 eller senere</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Gir en forhåndsvisning av utskrifta ved å simulere standard CMYK-blekk istedenfor p bruke RGB-farger</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Skru av/på blekkplata for cyan (C)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Skru av/på blekkplata for magenta (M)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Skru av/på blekkplata for gult (Y)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Skru av/på blekkplata for svart (K)</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Vis gjennoms&amp;iktighet</translation>
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
        <translation>&amp;Erstatt underliggende farge (UCR)</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Separasjonsnavn</translation>
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
        <translation>Skalering:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Skriv ut ...</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En metode for å skru av noen av gråsjatteringene som består av cyan, gul og magenta og bruke svart isteden. UCR berører for det meste de delene av bildet som er nøytrale og/eller har mørke toner som ligger nært opp mot grått. Hvis du bruker dette, kan det gi bedre utskrift for noen bilder, men det må prøves ut for hvert enkelt tilfelle. UCR gjør det mindre sannsynlig at det skal oppstå overmetning av blekk i fargene cyan, magenta og gul (CMY).</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Endre skaleringa av siden</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation>Bruk &amp;antialiasing</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of Type 1 Fonts, TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Antialiasing har en bedre forhåndsvisning av skriftyper av type 1, True Type, Open Type, EPD og også pdf-bilder og vektorbilder. Men det vil også gjøre forhåndsvisninga litt tregere.</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>Behandler malsidene:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Eksporterer sidene:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Bilde</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linje</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mangekant</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinje</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Tekststi</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopi av</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Forbundet med</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Er en forelder av</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Er et barn av</translation>
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
        <translation>Egenskaper for elementene på siden</translation>
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
        <translation>Verdi</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Forhold</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Forbundet med</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt + L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
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
        <translation>Dokumentoppsett</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Dobbeltsidig</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>I midten mot høyre</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Dra sider eller malsider til papirkurven for å slette dem</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Dette er malsidene. For å lage en ny, dra en malside til sidevisninga nedenfor</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Flytt om på sidene</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Tilgjengelige malsider</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokumentsider:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
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
        <translation>Kvartformat</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation>Folioformat</translation>
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
        <translation>Resultat</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Søk i resultatene etter:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Forhåndsvis</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Velg</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Sti</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Skriv ut</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Tilstand</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Mangler</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Søk</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Avbryt søket</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Håndter bilder</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus – bildesøk</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Søket mislyktes: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Fant ingen bilder som heter «%1».</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Velg hvilken mappe du vil søke fra</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Lagre som b&amp;ilde ...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksporter som bilde</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Eksporter de valgte sidene som BMP-bilder</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>Finner ikke programtillegget</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>ukjent feil</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Finner ikke symbolet (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Programtillegg: laster %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>Klarte ikke å starte</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>Ukjent programtillegg-type</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Programtillegg: %1 er lastet inn</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Programtillegg: %1 klarte ikke å starte: %2</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Håndtering av programtillegg</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>programtillegg</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Hvordan den skal kjøres</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Starte den?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Programtillegg-ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Du må starte programet på nytt for at endringene skal tre i kraft.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Egenskaper for mangekant</translation>
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
        <translation>&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Bruk &amp;faktor</translation>
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
        <translation>Antall hjørner på mangekantene</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Rotasjonsgrad på mangekantene</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Legg til en konkav/konveks-effekt for å endre formen på mangekantene</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Eksempel-mangekant</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>En negativ verdi vil gjøre mangekanten konkav (eller stjerneformet) mens en positiv verdi vil gjøre den konveks (bøyd ut).</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hjelpelinjer</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Verktøy</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Eksterne verktøy</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Grensesnitt (GUI)</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Stier</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Hjelpelinjer for sidemargene</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autolagre</translation>
    </message>
    <message>
        <source>min</source>
        <translation>minimum</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Bildebehandlingsverktøy</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Standard skriftstørrelse i menyene og vinduene</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard måleenhet for dokumentene</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Antall linjer Scribus vil rulle opp eller ned for hver bevegelse med musehjulet</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Antall nylig brukte filer som vises i filmenyen</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standard dokument-mappe</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standardmappe for skript</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standard sidestørrelse, enten en standardstørrelse eller en selvvalgt størrelse</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standardretning for dokumentsidene</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Bredden på dokumentsidene, kan bare endres ved å bruke en selvvalgt sidestørrelse</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Høyden på sidene i dokumentet, kan bare endres ved å bruke en selvvalgt sidestørrelse.</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Hvor ofte automatisk lagring skal skje</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Papirfarge</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Gi området utenfor margene samme farge som marglinjene</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Velg standard skaleringnivå</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Bruk antialias på skrifta ved gjengivelse av pdf og eps på skjermen</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Bruk antialias på bilder ved gjengivelse av pdf og eps på skjermen</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Velg en mappe</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Hjulhopp:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nylig åpnede dokumenter:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenter:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-profiler:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Endre ...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skript:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>&amp;Endre ...</translation>
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
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nedre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Øvre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vis &amp;ikke-utskrivbare områder i samme farge som marglinjene</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Juster visningsstørrelsen</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Navn på kjørbar fil:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Bruk antialias på &amp;tekst</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Bruk antialias på &amp;bilder</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>&amp;Navn på kjørbar fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>E&amp;ndre ...</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Språk:</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>Dokument&amp;maler:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Enheter:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Angre/Gjør om</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Lengde på handlingshistorien</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Orddeler</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Skrifttyper</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Forhåndssjekk</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Fargehåndtering</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Pdf-eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Egenskaper for dokumentelementene</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Innholdsregistre og stikkordslister</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Hurtigtaster</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Sidevisning</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt + U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Vis bildene</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Vis tekstlenkene</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Vis kontrolltegnene</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Vis rammene</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Arbeidsbord</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>For å justere visninga, dra glidebryteren nedover i rullefeltet.</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>ppt</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Oppløsning:</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Alltid spør hvis skriftyper må erstattes når et dokument blir lastet inn</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Forhåndsvis avsnittsstilen mens du redigerer den</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Vis oppstartsbildet</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Bruk alltid standard Lorem Ipsum</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Antall avsnitt:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Diverse</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Programtillegg</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Vis ikke-utskrivbare tegn, for eksempel linjeskift, i tekstrammene</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Velg om rammene skal vises</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Velg om bildene skal vises</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Ekstramappe for dokumentmaler</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Sett en linjal foran skjermen og dra glidebryteren for å justere skaleringa slik at Scribus viser sidene og objektene i rett størrelse.</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Angi hvor stor ledig plass du vil ha til venstre for dokumentene. Dette området kan du bruke som lagringsplass. Disse elementene kan du dra inn på og ut fra sidene som du vil.</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Angi hvor stor ledig plass du vil ha til høyre for dokumentene. Dette området kan du
bruke som lagringsplass. Disse elementene kan du dra inn på og ut fra sidene som du vil.</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Angi hvor stor ledig plass du vil ha over dokumentene. Dette området kan du
bruke som lagringsplass. Disse elementene kan du dra inn på og ut fra sidene som du vil.</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Angi hvor stor ledig plass du vil ha under dokumentene. Dette området kan du
bruke som lagringsplass. Disse elementene kan du dra inn på og ut fra sidene som du vil.</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Vis linjalmålene i forhold til siden</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Avstand mellom sidene</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vannrett:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Loddrett:</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Finn Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Finn ditt bildebehandlingsprogram</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Program som skal tolke PostScript</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Velg om du vil vise lenkene mellom rammene</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Velg hvilket språk du vil at Scribus skal bruke. La dette stå tomt hvis du vil la systeminnstillingene (vha. miljøvariablene) bestemme dette. Du kan alltids starte Scribus på et annet språk ved å angi språkkoden på kommandolinja når du starter det.</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>&amp;Skriftstørrelse (i menyene):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Skriftstørrelse (i &amp;vinduene):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Velg standard vindusdekorasjon og utseende. Scribus kan overta alle tilgjengelige KDE- og QT-temaer hvis QT er satt opp til å lete etter programtillegg for KDE.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Standard skriftstørrelse i verktøyvinduene</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Standardmappe for ICC-profiler. Du kan ikke endre dette samtidig som et dokument er åpent. Standardinnstillinga er at Scribus ser i systemmappene på Mac OSX og Windows. På Linux vil Scribus lete i $home/.color/icc,/usr/share/color/icc og /usr/local/share/color/icc.</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Velg dette hvis du vil at Scribus skal lagre en sikkerhetskopi av fila hver gang den valgte tidsperioden er over. Fila vil ha filendelsen «.bak»</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Velg hvor mange handlinger som skal lagres i handlingshistorien. Hvis du velger «0» er antallet ubegrenset.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Hvor på filsystemet bildeprogrammet er plassert. Hvis du bruker Gimp og den allerede er installert på din Linuxversjon anbefaler vi «gimp-remote», da det lar deg åpne bildene i Gimp selv når programmet kjører.</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation>Legg inn stien til Ghostcript-programmet. På Windows er det viktig at du installerer programmet «gswin32c.exe» og IKKE «gswin32.exe», ellers kan det føre til at programmet henger når du starter Scribus.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Standard</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>Lagre ...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Lagre innstillingene</translation>
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
        <translation type="obsolete">Pdf 1.3</translation>
    </message>
    <message>
        <source>PDF 1.4</source>
        <translation type="obsolete">Pdf 1.4</translation>
    </message>
    <message>
        <source>PDF/X-3</source>
        <translation type="obsolete">Pdf/X-3</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Vil du bruke innstillingene fra tidligere Scribus-versjoner?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus fant oppsettsfiler for versjon 1.2. Vil du bruke disse innstillingene i den nye versjonen?</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Klarte ikke å åpne og skrive til oppsettsfila «%1»: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Klarte ikke å skrive til oppsettsfila «%1»: QIODevice statuskode %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Klarte ikke å åpnel oppsettsfila «%1»: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Klarte ikke å åpne XML-innstillingene fra «%1»: %2 på linje %3, kolonne %4</translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation>Det oppsto en feil da innstillingene skulle lagres</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus klarte ikke å lagre innstillingene:&lt;br&gt;%1&lt;br&gt;Sjekk fil- og mapperettighetene og om det er nok diskplass igjen.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Det oppsto en feil da innstillingene skulle lastes inn</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus klarte ikke å lagre innstillingene:&lt;br&gt;%1&lt;br&gt;Scribus tar i bruk standardinnstillingene isteden.</translation>
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
        <translation>Det gyldne snitt</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Nidelt</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Du kan velge et forhåndslaget sideoppsett her. Hvis du velger «Ingen» så forblir sidemargene som de er, «Gutenberg» er et klassisk sideoppsett og «Tidsskrift» gir alle margene samme verdi. </translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åpne ...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>&amp;Avslutt</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>&amp;Kjør</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Lagre utdata ...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Skript-skall</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Skriv inn en kommando her. Dette vil så bli tolket som et skript.</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Utdata fra skriptet</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Python-skript (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Lagre Python-kommandoene i en fil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Tekstfiler (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Lagre de nåværende utdataene</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Lagre &amp;som ...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Kjør i et &amp;skall</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Python-skall for Scribus</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Dette skallet er hentet fra et standard Python-skall og har derfor noen begrensninger, spesielt når det gjelder mellomrom. Se håndboka for Scribus for mer informasjon.</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Åpne en Pythonskript-fil</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Farge&amp;tone:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Metning:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Verdi:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rød:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Grønn:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bl&amp;å:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lfa-kanal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Grunnfarge</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Selvvalgte farger</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Lag dine egne farger &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Legg til egne farger</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Velg farge</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopier eller flytt en fil</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Les: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Skriv: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Fil&amp;navn:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Fil&amp;type:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>En mappe opp</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Størrelse</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Egenskaper</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Søk &amp;i:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Tilbake</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Lag ny mappe</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Listevisning</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Detaljert visning</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Forhåndsvis filinformasjon</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Forhåndsvis filinnhold</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lese og skrive</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Skrivebeskyttet</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Bare skrive</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Ikke tilgjengelig</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symbolsk lenke til en fil</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symbolsk lenke til en mappe</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Spesiell symbolsk lenke</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Mappe</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Spesiell</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Åpne</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Endre navn</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>S&amp;lett</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Oppdater</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Sorter etter &amp;navn</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Sorter etter &amp;størrelse</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Sorter etter &amp;dato</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Ikke sorter</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sorter</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Vis s&amp;kjulte filer</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>fila</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>mappa</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>den symbolske lenka</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Slett %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Er du sikker på at du vil slette %1 «%2»?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Nei</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Ny mappe 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Ny mappe</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Ny mappe %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Finn mappa</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Mapper</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Lagre</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Feil</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Fant ikke fila
Sjekk stien og filnavnet.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alle filtyper (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Velg en mappe</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Mappe:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Skrifttype</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Skriftst&amp;il</translation>
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
        <translation>&amp;Overstrøket</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Understreket</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farge</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Prøve</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>&amp;Skript</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Velg skrifttype</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Tøm</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Marker alt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Still opp</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Selvvalgt ...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Om Qt&lt;/h3&gt;&lt;p&gt;Dette programmet bruker Qt versjon %1.&lt;/p&gt;&lt;p&gt;Qt er et utviklingsverktøy skrevet i C++ for grensesnitt på flere plattformer&lt;/p&gt;&lt;p&gt;Qt gjør samme kildekodepakke portabel mellom MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, og alle større kommersielle Unix-varianter.&lt;br&gt;Qt er også tilgjengelig for «embedded devices».&lt;/p&gt;&lt;p&gt;Qt er et produkt fra Trolltech. Se &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for mer informasjon.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Starter opp ...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Er du sikker på at du vil overskrive fila: %1 ?</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Nyhetsbrev</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brosjyrer</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Mapper</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Løpesedler</translation>
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
        <translation>Brevhoder</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Konvolutter</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visittkort</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendere</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Annonser</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Merkelapper</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menyer</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programmer</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Pdf-skjemaer</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Pdf-presentasjoner</translation>
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
        <translation>Annonseringer</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Tekstdokumenter</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Foldere</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Egne maler</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Lagre som bilde</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Klarte ikke å lage utdata-filen(e)</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Eksporteringa ble vellykket.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Fila finnes allerede. Vil du erstatte den?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>finnes allerede. Vil du erstatte den?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Ja til alle</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alle støttede formater (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus-skript</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Kjør skript ...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Nylig brukte skript</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Vis skallet</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-bilder (*.svg ·svgz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-bilder (*.svg);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importerer tekst</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle støttede formater</translation>
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
        <translation>Tekstfiler</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Skrifttypen %1 er ødelagt og vil ikke bli brukt</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished">Mediebokser</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Kommaseparert fil</translation>
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
        <source>
External Links
</source>
        <translation>
Eksterne lenker</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Tekstfiltre</translation>
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
        <translation>Katalansk</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Kinesisk</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tsjekkisk</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dansk</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Nederlandsk</translation>
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
        <translation>Galisisk</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Gresk</translation>
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
        <translation>Litauisk</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norsk (Bokmål)</translation>
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
        <source>&amp;About Script...</source>
        <translation>&amp;Om skript ...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Om skript</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Verdien er utenfor det gyldige området. Bruk en av konstantene i scribus.UNIT_*.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Målet er ikke en bilderamme</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Hjørneradiusen må være et positivt tall.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare finne skriftstørrelser i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare finne skrifttyper i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare finne tekststørrelser i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare telle antall kolonner i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare finne linjeavstander i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare finne kolonnemellomrom i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare hente tekst fra en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare sette inn tekst i en tekstramme</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Justeringa er utenfor det gyldige området. Du har brukt en konstant som ikke finnes i scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objektet er ikke en lenket tekstramme, derfor kan ikke lenka fjernes</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objektet er det siste i en serie, kan ikke fjerne lenka. Fjern lenka til den forrige ramma istedenfor.</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente inn en farge som ikke har navn.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke endre en farge som ikke har navn.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke opprette en farge uten å gi den et navn-</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke slette en farge som ikke har navn.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke erstatte en farge som ikke har navn.</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Fant ikke fargen – Pythonfeil</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Tilleggsoppsett (valgfritt):</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standardoppsett: </translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Behandler småord. Vent ...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Småordene er ferdigbehandlet.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer-dokumenter</translation>
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
        <source>Welsh</source>
        <translation>Walisisk</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Filnavnet må være en streng</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>Egenskapen «allTypes» er skrivebeskyttet</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Klarte ikke å eksportere bildet</translation>
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
        <translation>Scribus krasjet</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus krasjer på grunn av signalnummeret %1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Malside</translation>
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
        <translation>Klarte ikke å åpne utfila %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation type="unfinished">Ut-strømmen er skrivebeskyttet</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Klarte ikke å kontrollere oppsettet: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Klarte ikke å åpne innfila %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Klarte ikke å lese XML-oppsettsfila:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (linje %2 kolonne %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Klarte ikke å lese XML-oppsettsfila: %1</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; ugyldig</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>fant %1 &lt;%2&gt; noder, trenger 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>uventet nullnode &lt;%2&gt;</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>noden &lt;%1&gt; er ikke et element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elementet &lt;%1&gt; mangler angivelsen «value»</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>Verdien på elementet &lt;%1&gt; må enten være «true» eller «false»</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elementet &lt;lpiSettingsEntry&gt; mangler angivelsen «name»</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Biblioteket Freetype2 er ikke tilgjengelig</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>Skrifttypen %1 er ødelagt og blir ikke tatt med</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation type="unfinished">Skrifttypen %1 er ødelagt (lesestrømmen) og blir ikke tatt med</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Skrifttypen %1 er ødelagt (FreeType2) og blir ikke tatt i bruk</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Skrifttypen %1 er ødelagt (mangler skrifttypenavn) og blir ikke tatt i bruk</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Glyffen %2 i skrifttypen %1 er ødelagt (tegnkode %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Skrifttypen %1 er ødelagt og blir ikke tatt i bruk</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Skrifttypen %1 kan ikke leses og blir ikke tatt med</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Klarte ikke å laste inn skrifttypen %1 – ukjent skrifttype</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Skrifttypen %1 ble lastet inn fra %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Skrifttypen %1(%2) er en kopi av %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Laster inn skrifttypen % (den ble funnet ved å bruke «fontconfig»)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Klarte ikke å laste inn skrifttypen «freetype2», klarte ikke å finne fila</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Skrifttypen %1 er ødelagt (FreeType) og blir ikke tatt i bruk</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>Glyffen %2 i skrifttypen %1 (tegnkode %3) er ugyldig og blir ikke tatt i bruk</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation type="unfinished">Leser navnet på skrifttypen fra %1 fra skrifttype %2 (forskyvning=%3, nTabeller=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation type="unfinished">memcpy-header: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>tabell «%1»</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation type="unfinished">memcpy-tabell: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation type="unfinished">memcpy-forskyvning: %1 %2 %3</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Utviklingsversjonen av Scribus</translation>
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
        <translation>cm</translation>
    </message>
    <message>
        <source> c</source>
        <translation>c</translation>
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
        <translation>Fila finnes allerede</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Erstatt</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Dokumentmal</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Fant ikke fargen</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Fant ikke denne fargen i dokumentet</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Fant ikke denne fargen blant standardfargene</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Klarte ikke å åpne dokumentet.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Klarte ikke å lagre dokumentet.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation type="unfinished">Argument skal være navn på et sideelement, eller et PyCObjekt-tilfelle</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Fant ikke egenskapen</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation type="unfinished">Fant ikke barneprosessen</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Klarte ikke å gjøre om resultattypen «%1»</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Egenskapen «%1» støttes ikke</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Klarte ikke å gjøre om «%1» til egenskapen</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Typene stemmer overens, men klarte ikke å angi egenskapen.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Det angitte elementet er ikke en bilderamme</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Må ha minst to objekter for å kunne gruppere</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Må ha minst to objekter for å kunne gruppere</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Trenger et utvalg eller en argumentliste med de elementene som skal grupperes</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Kan ikke endre størrelsen med 0 %</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Fant ikke skrifttypen.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Laget må ha et navn.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Fant ikke laget.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Kan ikke fjerne det siste laget.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Kan ikke opprette et lag uten navn.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Det finnes allerede et objekt med dette navnet.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>En punktliste må inneholde minst to punkter (fire verdier).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>En punktliste må inneholde et likt antall verdier.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>En punktliste må indeholde minst tre punkter (seks verdier).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>En punktliste må inneholde minst fire punkter (åtte verdier).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>En punktliste må ha et antall som går opp i seks.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Fant ikke objektet.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Fant ikke stilen.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare endre stil på tekstrammer.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Klarte ikke å lagre som EPS.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Sidetallet er utenfor det tillate området.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>Argument er ikke en liste: det må være en liste med kommatall</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>Argumentet inneholder ikke-numeriske verdier: det må være en liste med kommatall.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>Argumentet inneholder ikke-numeriske verdier: det må være en liste med kommatall.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Strekbredden er feil, den må være mellom 0 og 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Verdien for metningsgraden på linja er feil, den må være mellom 0 og 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Verdien for metningsgraden på fyllfargen er feil, den må være mellom 0 og 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Fant ikke linjestilen.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Teksten kan bare justeres i en tekstramme.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Skriftstørrelsen er feil, den må være mellom 1 og 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare angi skriftstørrelsen i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare angi skrifttypen i en tekstramme.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Linjeavstanden er feil, den må være minst 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Linjeavstanden kan bare angis i en tekstramme.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Kolonneavstanden er feil, den må være et positivt tall.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kolonnebredden kan bare angis i en trekstramme</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Antallet kolonner er feil, du må ha minst en.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kolonneantallet kan bare angis i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Du kan bare markere tekst i en tekstramme</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Du kan bare slette tekst i en tekstramme.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Du kan bare angi tekstfyll i en tekstramme</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Du kan bare angi metningsgrad på en tekst hvis den er i en tekstramme</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Du kan bare lenke sammen tekstrammer.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Målramma må være tom.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Målramma er allerede lenket til en annen ramme.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Målramma er allerede blitt lenket til fra en annen ramme.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Kilden og målet er det samme objektet.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Kan bare bryte lenker mellom tekstrammer.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Kan bare gjøre om tekstrammer til omriss.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Kan bare sjekke tekstrammer om de er overfylte.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Kan bare angi bokmerker i tekstrammer</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Kan bare hente informasjon fra tekstrammer</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Filnavnet kan ikke være en tom streng.</translation>
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
        <translation>Scribus-modul med et Python-grensesnitt

Denne modulen er et Python-grensesnitt for Scribus. Den inneholder funksjoner
for å kontrollere Scribus og for å behandle objekter på arbeidsbordet. Alle
funksjonene er beskrevet nedenfor.

Noen få ting er felles for det meste i grensesnittet.

De fleste funksjonenene utføres på en ramme. Rammene indentifiseres ved navn
(en streng), og er ikke ekte Python-objekter. Mange funksjoner har et
valgfritt parameter (uten passord), et rammenavn.
Mange unntak er også felles for de fleste funksjonene. Disse er
ennå ikke dokumentert i de tilhørende dokumentasjonsstrengen for hver av dem.
- Mange funksjoner vil gi feilen «NoDocOpenError» hvis du prøver å bruke dem
 uten at de har et dokument å utføres på.
- Hvis du ikke gir et rammenavn til en funksjon som trenger det, så
vil funksjonen bruke den gjeldende ramma, hvis det er en, eller gi feilmeldinga
«NoValidObjectError» hvis den ikke finner en ramme å arbeide på.
- Mange funksjoner vil gi feilen «WrongFrameTypeError» hvis du prøver å bruke dem
på en rammetype der det ikke gir mening å bruke dem, for eksempel å velge
tekstfarge på en bilderamme.
- Feil som kommer fra kall til det underliggende Python API vil bli
sendt videre uendret. Denne lista over unntak som oppstår ved
en funksjon og de tilhørende dokumentasjonsstrengene er ikke komplett.

Detaljer om hvilke unntak hver funksjon kan gi finner du i dokumentasjonen
til funksjonen, men, som det meste som gjelder Pythonkode, så er heller ikke denne liista helt komplett da det finnes unntak i funsjonenen som blir kalt opp.
</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Tekstdokumenter i OpenDocument-formatet</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>Side</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 av </translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Svart</translation>
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
        <source>Color Wheel</source>
        <translation>Fargehjul</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Forhåndsvisning av skrifttyper</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Mine programtillegg</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Ny fra mal</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Eksporter som bilde</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>Importfilter for Ps/Eps</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Lagre som mal</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skript</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Småord</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVG-eksport</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>Importering av SVG-filer</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>Importering av OpenOffice.org Draw-filer</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus krasjet pga. følgende unntak: %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Oppretter et mellomlager for skrifttyper</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Fant en ny skrifttype, sjekker ...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Fant en endret skrifttype, sjekker ...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Leser mellomlageret for skrifttyper</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Skriver til det oppdaterte mellomlageret for skrifttyper</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Leter etter skrifttyper</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Du bruker en utviklingsversjon av Scribus 1.3.x. Det dokument du arbeider med er oprinnelig laget i Scribusversjonen 1.2.3 eller lavere. Når du lagrer dokumentet, vil det være ubrukelig i Scribus 1.2.3 med mindre du bruker «Fil – Lagre som». Er du sikker på at du vil fortsette?</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Endringerne i ditt dokument er ikke lagret og du har valgt å forkaste dem. Vil du fortsette?</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>En fil med navnet «%1» finnes allerede.&lt;br/&gt;Vil du erstatte den med den fila du er i ferd med å lagre?</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Støtte for det tidligere formastet «.sla»</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Tysk (Gammel rettskrivning)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Eksporterer Postscript-filer</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Skriver ut fil</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Du prøver å hente inn flere sider enn det som er tilgjengelig i dette dokumentet, målt fra gjeldende side.&lt;/p&gt;Velg en av disse mulighetene:
&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Opprett&lt;/b&gt; de sidene som mangler&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importer&lt;/b&gt; så mange sider som passer på de sidene du har&lt;/li&gt;&lt;li&gt;&lt;b&gt;Avbryt&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Opprett</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Thai</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Lag strekkode</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word-dokumenter</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB-dokumenter</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>Importering av PDB-filer</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Klarte ikke å åpne file %1</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Luxemburgsk</translation>
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
        <source>Japanese</source>
        <translation>Japansk</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Den oppgitte malsiden stemmer ikke overens med noen av de eksisterende.</translation>
    </message>
    <message>
        <source>Icelandic</source>
        <translation>Islandsk</translation>
    </message>
    <message>
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation>%1 er kanskje skadet: mangler informasjon om oppløsninga</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>Denne fila blir ikke gjenkjent som et PDB-dokument. Meld dette inn som en feil, hvis du er sikker på det er det.</translation>
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
        <translation>%1 kan være ødelagt: Informasjonen om oppløsningen mangler eller er feil</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>
Load PDF settings</comment>
        <translation type="obsolete">nullrot-node</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>
python error</comment>
        <translation type="obsolete">«firstPageOrder» er større enn tillatt.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>
python error</comment>
        <translation type="obsolete">Kan ikke gjengi et tomt innhold.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>
scripter error</comment>
        <translation type="obsolete">Klarer ikke å lagre pixmap</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>
python error</comment>
        <translation type="obsolete">Kan ikke plassere tekst i annet enn tekstrammer.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>
python error</comment>
        <translation type="obsolete">Sett inn index – utenfor de tillatte verdiene</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>
python error</comment>
        <translation type="obsolete">Den valgte innholdfortegnelsen er utenfor det gyldige området</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>
python error</comment>
        <translation type="obsolete">Du kan bare angi bredden på bokstavstreken i en tekstramme</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>
python error</comment>
        <translation type="obsolete">Kan ikke slette innstillingene for bildetypen.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>
python error</comment>
        <translation type="obsolete">Bildetypen må være en streng</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>
PDB Importer</comment>
        <translation type="obsolete">PDB_data</translation>
    </message>
    <message>
        <source>German (Swiss)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished">nullrot-node</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished">«firstPageOrder» er større enn tillatt.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation type="unfinished">Kan ikke gjengi et tomt innhold.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation type="unfinished">Klarer ikke å lagre pixmap</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Kan ikke plassere tekst i annet enn tekstrammer.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished">Sett inn index – utenfor de tillatte verdiene</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished">Den valgte innholdfortegnelsen er utenfor det gyldige området</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished">Du kan bare angi bredden på bokstavstreken i en tekstramme</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation type="unfinished">Kan ikke slette innstillingene for bildetypen.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished">Bildetypen må være en streng</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished">PDB_data</translation>
    </message>
    <message>
        <source>Chinese (Trad.)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Tøm</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Marker alt</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør om</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>K&amp;opier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
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
        <translation>Rull opp</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Rull ned</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normaliser</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimer</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maksimer</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Gjenopprett</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Flytt</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimer</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Mak&amp;simer</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Forbli øverst</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimer</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Gjenopprett ned</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Lukk</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Rull opp</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 – [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Rull ned</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Dokumentinnstillinger</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Hjelpelinjer for sidemargene</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oppe:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nede:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Høyre:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Stående</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Liggende</translation>
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
        <source>&amp;Width:</source>
        <translation>&amp;Bredde:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Høyde:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Enhet:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autolagre</translation>
    </message>
    <message>
        <source>min</source>
        <translation>minimum</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformasjon</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hjelpelinjer</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Sidevisning</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vis &amp;ikke-utskrivbare områder i samme farge som marglinjene</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt + U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Vis bilder</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Vis tekstlenker</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Vis kontrolltegn for tekst</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Vis rammene</translation>
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
        <translation>Verktøy</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Orddeler</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Skrifttyper</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Forhåndssjekk</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Pdf-eksport</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Egenskaper for dokumentelementene</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Innholdsregistre og stikkordslister</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Fargehåndtering</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Vis ikke-utskrivbare tegn, som for eksempel linjeskift i tekstrammer</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Velg om du vil vise rammene</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Velg om du vil vise bildene</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Papirfarge</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Bruk samme farge på området utenfor margene som på marglinjene.</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="unfinished">Vis linjalmålene i forhold til siden</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation>Minstestørrelsen på arbeidsbordet</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Avstand mellom sidene</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Vannrett:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Loddrett:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Tilpasser fargene</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Vis eller skjul lenkene mellom tekstrammene.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>Bruk denne sidestørrelsen på alle sidene</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Bolker</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Bruk den endrede sidestørrelsen på alle sidene i dokumentet</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python-skript (*.py);; Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Kjør som et utvidelsesskript</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Stilbehandler</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation>Kolonne 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation>&amp;Kolon</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt + L</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt + K</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation>&amp;Bruk</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt + P</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt + N</translation>
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
        <translation>Linjer</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilen som brukes på det gjeldende avsnittet</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Stilinnstillinger</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Color of text fill</source>
        <translation>Farge på tekstfyll</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Metningsgrad på tekstens fyllfarge</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Fyllfarge-innstillinger</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Color of text stroke</source>
        <translation>Tekstens strekfarge</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Metningsgrad på tekststrekens farge</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Fyllfarge-innstillinger</translation>
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
        <translation>Skrifttype på den markerte teksten</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Skalering av tegnbredden</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Skrifttype-innstillinger</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Skalerer høyden på skrifttegnene</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Tegninnstillinger</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuel bokstavavstand (tracking)</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Lagre som &amp;SVG ...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Eksporter som SVG-fil</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Eksporterer den gjeldende siden som en SVG-fil</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importer en &amp;SVG-fil ...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importerer SVG-filer</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importerer de fleste SVG-filer til det gjeldende dokumentet ved å gjøre om vektordata til Scribus-objekter.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalerbare vektorbilder</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG-fila inneholder noen egenskaper som ikke er støttet</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Småord</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Bruk harde mellomrom på:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>De &amp;valgte rammene</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Gjeldende &amp;side</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Alle objektene</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Bare de valgte rammene er behandlet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Bare den gjeldende siden er behandlet</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Alle objekter i dokumentet er behandlet.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Brukerinnstillinger</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Systemets standardoppsett</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Lagre</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Nullstil</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Lagre brukerinnstillingene</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Last inn igjen standardoppsettet og slett brukerinnstillingene</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Redigér brukerinnstillingene. Hvis du lagrer dem vil de bli brukt isteden for det innebygde oppsettet</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Småord</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Brukerinnstillingene finnes allerede. Er du sikker på at du vil overskrive dem?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Kan ikke skrive fila %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Brukerinnstillingene er lagret</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>De innebygde innstillingene er gjenopprettet</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Kan ikke åpne fila %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Lagre som &amp;mal ...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Lagre et dokument som en mal</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Lagre et dokument som en mal. Dette er lurt for å forenkle det innledende arbeidet når du trenger flere dokumenter med samme oppsett.</translation>
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
        <translation>Last inn/Lagre/Importer/Eksporter</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukjent</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Topp</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Høyre</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Bunn</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>La den festes til ...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vannrett</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Loddrett</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation type="unfinished">Flytende ...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Skriver ut ...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 av </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation>Dokumentfarger</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Nytt lag</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Bakgrunn</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Er du sikker på at du vil slette hele teksten?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Kan ikke slette elementer som er i bruk</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Elementet %1 redigeres i den innebygde tekstbehandleren. Slettinga avbrytes.</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation>Det oppsto en feil under innlesningen av ICC-profilene, fargehåndtering er ikke skrudd på.</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>Initializing Plugins</source>
        <translation>Starter tilleggsprogrammene</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Laster inn hurtigtastene</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Leser innstillingene</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Starter den innebygde tekstbehandleren</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>Leser ICC-Profilene</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Starter orddelingsprogrammet</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Setter opp kladdeboka</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Setter opp hurtigtastene</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Rediger</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Leter etter skrifttypene</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Det ble ikke funnet noen skrifttyper på ditt system.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Avslutter nå.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Alvorlig feil</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Starter skrifttypesystemet</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Åpne &amp;nylig brukte</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Eksporter</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Stil</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farge</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Metningsgrad</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Skrifttype</translation>
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
        <translation>Innstillinger for forhåndsvisninga</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivå</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Send til &amp;lag</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Pdf-innstillinger</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;Gjør om til</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Sett inn</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Hermetegn</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellomrom</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Side</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Vis</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Ekstra</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Vinduer</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hjelp</translation>
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
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importerer sider ...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importerer side(r)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Ferdig med å importere</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Fant ingenting å importere</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Fila %1 er ikke i et akseptabelt format</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Leser inn ...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Noen av de ICC-profilene som er brukt i dette dokument er ikke installert:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> ble erstattet med: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(Konvertert)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle de støttede formatene</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filtypene (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumentene (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentene (*.sla *.scd);; Alle filene (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Lagrer ...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus har funnet noen feil. Kanskje det hjelper å bruke forhåndsjekkinga for å rette dem</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorer</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Skriver ut ...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Klarte ikke å skrive ut</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Kan ikke fjerne elementer som er i bruk</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Elementet %1 redigeres i den innebygde tekstbehandleren. Utklippinga blir avbrutt.</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Om Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus&apos; håndbok</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfilene (*.txt);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Gjør om siden til en malside</translation>
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
        <translation>&amp;Metningsgrad:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Metningsgrad</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Følgende program mangler:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript : Du kan ikke bruke EPS-bilder eller forhåndsvise utskrifter</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus har funnet noen feil.
Kanskje det hjelper å bruke forhåndsjekkinga for å rette dem</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS-filer (*.eps);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus har funnet noen feil.
Kanskje det hjelper å bruke forhåndsjekkinga for å rette dem</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Side%1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Noen objekter er låst.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Lås alle</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Lås opp alle</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Informasjon</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Programmet %1 kjører allerede</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Programmet %1 finnes ikke</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Den valgte fargen finnes ikke i dokumentets fargesett. Gi den nye fargen et navn.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Fant ikke fargen</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Navnet du har valgt er allerede i bruk. Gi fargen et annet navn.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Nivå</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Send til lag</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Innstillinger for forhånds&amp;visninga</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>Verk&amp;tøy</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Posisjon:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Posisjon:</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mellomrom og skift</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaturer</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Ny malside %1</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Antall kopier: %1
Vannrett forskyvning: %2
Loddrett forskyvning: %3</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript : Du kan ikke bruke EPS-bilder eller Postscripts forhåndsvisning av utskrifter</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript mangler: Postscripts forhåndsvisning av utskrift er ikke tilgjengelig</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Er du sikker på at du vil erstatte bildet?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Innhold</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>&amp;Ligatur</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Kan ikke skrive til fila: 
%1</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Ugyldig argument: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Fila %1 finnes ikke, avslutter.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Bruk: scribus [tilvalg ... ] [fil]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Innstillinger:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Skriv hjelpeteksten (denne beskjeden) og avslutt</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Bruker xx som en hurtigtast for et språk, f.eks. «en» eller «de»</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>List opp de grensesnittspråkene som er installert</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Vis informasjon i skallet når skrifttypene leses inn</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Ikke vis oppstartsbildet</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Vis versjonsinformasjon og avslutt</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Plasser vindusknappene fra høyre til venstre (f.eks. Avbryt/Nei/Ja istedenfor Ja/Nei/Avbryt)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>filnavn</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Bruk filnavnet som sti for selvvalgte innstillinger</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>De grensesnittspråkene for Scribus som er installerte er:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>For å overstyre standard språkvalg:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx eller scribus --lang xx, hvor xx er det språket du vil bruke.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribusversjon</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, et datatrykkeprogram (DTP) som er åpen kildekode</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Hjemmeside</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Dokumentasjon</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Feil og ønsker</translation>
    </message>
    <message>
        <source>Display a console window</source>
        <translation>Åpne et skall</translation>
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
        <translation>Lag</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopier hit</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Flytt hit</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Bilde</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fil: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">Opprinnelig PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">Faktisk PPI: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Lenket tekst</translation>
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
        <translation>Avsnitt: </translation>
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
        <translation type="obsolete">Skriv ut: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>I bruk</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Ikke i bruk</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Informasjon</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Innstillinger for forhåndsvisninga</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;Pdf-innstillinger</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Send til &amp;lag</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;vå</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Gjør om til</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Lenkede tekstrammer</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Du prøver at lenke til en fylt ramme, eller å lenke en ramme til seg selv.</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Kan ikke gjøre om et element som er i bruk</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Elementet %1 redigeres i den innebygde tekstbehandleren. Omgjøringa til omriss blir hoppet over.</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Side %1 til %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Fargeområde: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ukjent</translation>
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
        <translation>Innhold</translation>
    </message>
    <message>
        <source>Export: </source>
        <translation type="unfinished"></translation>
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
        <translation>er endret siden sist gang det ble lagret.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Forkast</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Støtte for innebygde Python-skript</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Skript</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Skriptfeil</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Hvis du kjører et offisielt skript, meld dette inn på
&lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Denne meldinga er overført til utklippstavla, bruk «Ctrl + V» for å lime den inn i feilmeldingssøkeren.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Det oppsto en intern feil i forsøket på at utføre den kommandoen du oppga. Detaljer om feilen ble skrevet til standard feilkanal (stderr).</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Undersøk skript</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Klarte ikke å sette opp programtillegg for Python. Detaljer om feilen ble sendt til standard feilkanal (stderr).</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Python-skript (*.py);;Alle filene (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentasjon for:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Skript</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation type="unfinished"> inneholder ingen doc-streng</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Skriptinnstilinger</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Bruk utvidelsesskript</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Utvidelser</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Skall</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Oppstartsskript</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Feil:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Kommentarer:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Nøkkelord:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Tegn:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Tall:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>strenger:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Grunntekster:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Velg farge</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Endre ...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Finn oppstartsskript</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Forhåndsvis sidene</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Søk/Erstatt</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Søk etter:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Avsnittsstil</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Skrifttype</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Skriftstørrelse</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Skrifteffekter</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Fyllfarge</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Metningsgrad på fyllfargen</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Strekfarge</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Metningsgrad på streken</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Venstre</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Midten</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Høyre</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blokk</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Tvunget</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Erstatt med:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Søket er ferdig</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Hele ordet</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ikke skill mellom små og store bokstaver</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Søk</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Erstatt</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Erstatt &amp;alle</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Lukk</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>T&amp;øm</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Søket er ferdig, fant %1 treff</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Velg felt</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Tilgjengelige felt</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Valgte felt</translation>
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
        <translation>Andre ...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Metningsgrad</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Metningsgrad:</translation>
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
        <translation>X-forskyvning</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-forskyvning</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Småord ...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Småord</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words. Available in the following languages: </source>
        <translation>Spesielt programtillegg for å legge til harde mellomrom foran eller etter ulike småord. Lagt inn for følgende språk:</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Rediger stiler ...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Ingen stil</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Rediger stiler</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi av %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Ny stil</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Lag en &amp;kopi</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Innebygd tekstbehandler</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Gjeldende avsnitt:</translation>
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
        <source>Totals:</source>
        <translation>Totalt:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Avsnitt: </translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Vil du lagre endringene?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfiler (*.txt);;Alle filtyper (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Lagre som</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Hent teksten pånytt fra ramma</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Lagre til fil ...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Hent fil ...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Lagre &amp;dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Oppdater tekstramma og avslutt</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;avslutt uten å oppdatere tekstramma</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Klipp ut</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopier</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Lim inn</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Tøm</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Oppdater tekstramma</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Rediger</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Marker &amp;Alt</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>R&amp;ediger stiler ...</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søk/Erstatt ...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Forhåndsvisning av skrifttypene ...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Bakgrunn ...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Vis skrifttype ...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Innstillinger</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Smart tekstvalg</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Sett inn glyff ...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Slett all teksten</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Innebygd tekstbehandler – %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Er du sikker på at du vil slette alle endringene?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Er du sikker på at du vil slette all teksten?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Sett &amp;inn</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Hermetegn</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Mellomrum og skift</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaturer</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Mellomrom</translation>
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
        <translation>Forskyvning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linjebredde</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation>Mer enn et element er valgt</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Small Caps</source>
        <translation>Kapiteler</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hevet skrift</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Bare store bokstaver</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Understrek tekst. Hold knappen nede et øyeblikk for å angi bredden og forskyvninga på streken.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Bare understrek ord. Hold knappen nede et øyeblikk for å angi bredden og forskyvninga på streken.</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Overstrykning. Hold knappen nede et øyeblikk for å endre bredden og forskyvninga på streken.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation>Omriss. Hold knappen nede et øyeblikk for å endre strekbredden på omrisset.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Skyggetekst. Hold knappen nede et øyeblikk for å endre forskyvninga.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Bruk filnavnet som første del av navnet på avsnittsstilen</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ikke spør igjen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Valg for importering av filer fra OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Overskriv avsnittsstiler</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Hvis du tar i bruk dette vil de eksisterende stilene i dette Scribus-dokumentet bli overskrevet.</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Slå sammen avsnittsstilene</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Slå sammen avsnittsstiler ut fra hvilke egenskaper de har. Dette vil gi færre stiler og likevel beholde stilegenskapene, selv om stilene i det opprinnelige dokumentet har andre navn.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Bruk dokumentnavnet som en del av stilnavnet i Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Gjør disse innstilingene til standardinnstillingene og ikke spør igjen når et annet OpenOffice.org 1.x-dokument blir lastet inn.</translation>
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
        <translation>Ingen</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>På slutten</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>På begynnelsen</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Ikke vist</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Innholdsregistre og stikkordslister</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Innholdsregistre</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Ramma som innholdsregisteret blir plassert i</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Sidetall plassert:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Navn på objektegenskapen:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Objektegenskapen som blir brukt på rammene som er grunnlaget for å lage oppføringene</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Sett sidetallene som hører til oppføringene på begynnelsen eller slutten av linja, eller ikke sett inn noe sidetall</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Vis ikke-utskrivbare oppføringer</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Ta også med rammer som er satt til ikke å bli skrevet ut</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Avsnittsstilen som blir brukt på førstelinjene</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Avsnittsstil:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Målramme:</translation>
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
        <translation type="unfinished">Innholdsregistre og stikkordslister</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Innholdsregistre</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Legg til</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt + A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt + D</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Ramma som innholdsregistrene blir plassert i</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation type="unfinished">Sidetall plassert:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Navn på objektegenskapen:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Objektegenskapen som blir brukt på rammene som er grunnlaget for å lage oppføringene</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Sett sidetallene som hører til oppføringene på begynnelsen eller slutten av linja, eller ikke sett inn noe sidetall</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Vis ikke-utskrivbare oppføringer</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Ta også med rammer som er satt til ikke å bli skrevet ut</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Avsnittsstilen som blir brukt på førstelinjene</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Avsnittsstil:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Målramme:</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Overse alle feil</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Foreta en automatisk sjekk før utskrift eller eksportering</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Se etter manglende glyffer</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Se etter objekter som ikke er på en side</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Se etter overflyt i tekstrammene</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Sjekk om det er blitt brukt gjennomsiktighet</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Se etter bilder som mangler</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Sjekk bildeoppløsninga</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Lavest tillatte oppløsning</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppt</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Se etter innsatte pdf-filer</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Sjekk pdf-kommentarer og -felter</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Legg til profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Fjern profil</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Fellesinnstillinger</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Plassering i dokumentene</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>I bakgrunnen</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>I forgrunnen</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Tiltrekning</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Festeområde:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation type="unfinished">Gripeavstand:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Vis hjelpelinjene</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farge:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Vis marglinjene</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Vis rutenettet</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Grovmasket rutenett</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Avstand:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Finmasket rutenett</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Vis vannrette linjer</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Innstillinger for de vannrette linjene</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Vannrette &amp;linjer:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>&amp;Forskyvning i forhold til grunnlinja:</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Hjelpelinjene er ikke synlige gjennom objektene på siden</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Hjelpelinjer er synlige over alle objektene på siden</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Avstand mellom rutene i det finmaskede rutenettet</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Avstand mellom rutene i det grovmaskede rutenettet</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Festeområde for hjelpelinjene, objekter nærmere enn dette vil festes til dem</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radiusen på området der Scribus vil la deg gripe håndtakene til et objekt</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Farge på gitterlinjene</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Farge på de grovmaskede gitterlinjene</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Farge på hjelpelinjene som du selv setter inn</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farge på marglinjene</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Fargen på de vannrette linjene</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Skru av eller på rutenettet</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Avstand mellom de vannrette linjene</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Avstand mellom toppen av siden og den første vannrette linja</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Slå av eller på rutenettet</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Slå av eller på hjelpelinjene</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Slå av eller på marglinjene</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Håndter tabulatorer</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Eksporter område</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Alle sidene</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Velg sider</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotasjon:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Filinnstillinger</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Samspill:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Innbinding:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Venstremarg</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Høyremarg</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Lag &amp;miniatyrbilder</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Lagre &amp;lenkede tekstrammer som pdf-artikler</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Ta med bokmerkene</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppt</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>&amp;Oppløsning for EPS-bilder:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>&amp;Komprimer tekst og vektorbilder</translation>
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
        <translation>Maksimal</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Høy</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Middels</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Lav</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimal</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Generelt</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Sett inn</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Tilgjengelige skrifttyper:</translation>
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
        <translation>Skrifttyper som skal settes inn:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Skrifttyper</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Bruk &amp;presentasjonseffekter</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>&amp;Forhåndsvis sidene</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Visningsvarighet:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Varighet på &amp;effekten:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Effekt&amp;type:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Bevegelige linjer:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Fra:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sekund</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Ingen effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Persienner</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Boks</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Oppløs</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Glitter</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Oppdeling</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Visk ut</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Vannrett</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Loddrett</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Innside</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Ytterside</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Fra venstre mot høyre</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Ovenfra og ned</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Nedenfra og opp</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Fra høyre mot venstre</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Fra øverst til venstre til nederst til høyre</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Bruk effekten på alle sidene</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Ekstra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Bruk kryptering</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Passord</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Bruker:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Eier:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Innstillinger</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>La andre &amp;skrive ut dokumentet</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>La andre &amp;endre dokumentet</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>&amp;La andre kopiere tekst og bilder</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>La andre legge til &amp;nye kommentarer og felt</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Sikkerhet</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Utskrift &amp;beregnet for:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Skjerm/Internett</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Skriver</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gråskala</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Bruk selvvalgte innstillinger for framvisning</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Innstillinger for framvisning</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Frekvens:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Vinkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">&amp;Spot-funksjon:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Enkelt prikk</translation>
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
        <translation>Ellipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Ensfarget:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Bruk en ICC-profil</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Fargetilpassningsmetode:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Sansningsmessig (perseptuell)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ fargeanalyse (kolorimetri)</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Metning</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolutt fargeanalyse (kolorimetri)</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Ikke bruk innebygde ICC-profiler</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farge</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">Pdf/X-3 Output Intent</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Informasjonsstreng:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>&amp;Ut-profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Beskjæringsboks</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>Pdf/X-&amp;3</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Forhåndsvis alle de valgte sidene ovenfor.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Visningseffekt</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Retning på bevegende linjer for oppdelings- og persienne-effektene.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Startposisjonen av boks- og oppdelingseffektene.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Retningen på glitter eller utvisknings-effektene.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Bruk effekten på alle sidene.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Eksporter alle sidene til pdf</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Eksporter et utvalg av sidene til pdf</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Lag pdf-artikler, dette er nyttig for å navigere mellom lenkede artikler i en pdf-fil.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>Ppt (punkt per tomme) på bildene når de eksporteres</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Velg et passord så brukere kan lese din pdf-fil.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>La andre skrive ut pdf-fila. Hvis du fjerner krysset blir ikke det mulig.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>La andre endre på pdf-fila. Hvis du fjerner krysser blir ikke det mulig.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Legg inn en fargeprofil for ensfargede farger</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Fargeprofil for ensfargede farger</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Fargetilpasning for ensfargede farger</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Legg inn en fargeprofil for bilder</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Ikke bruk fargeprofiler som er lagt inn i kilde-bildene</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Fargeprofil for bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Fargetilpasning for bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Ut-profil for utskrift. Hvis det er mulig, se hva skriveren sier om profilvalg.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Utfallende (avstand) fra toppen av den fysiske siden for trykk til kant</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Avstand for utfallende (trykk til kant) fra bunnen av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Avstand for utfallende (trykk til kant) fra venstre side av den fysiske siden</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Avstand for utfallende (trykk til kant) fra høyre side av den fysiske siden</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Speilvend siden(e) vannrett</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Speilvend siden(e) loddrett</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Gjør om spotfarger til prosessfarger</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Komprimerings&amp;kvalitet:</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>La andre kopiere tekst eller bilder fra pdf-Fila. Hvis du fjerner krysser blir ikke det mulig.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>La andre legge inn kommentarer og felter i pdf-fila. Hvis du fjerner krysser blir ikke det mulig.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Skrur på omgjøring av spotfarger til sammensatte farger. Med mindre du planlegger å trykke med spotfarger på et trykkeri, er det sannsynligvis best å la dette være på.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Ta med la&amp;g</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>&amp;Komprimeringsmetode:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Endre &amp;oppløsninga på bildene til:</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Den tiden effekten varer. En kort tidsangivelse vil gjøre effekten raskere mens en lengre vil sinke den.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Skriv inn en kommaseparert liste med tegn der
tegnet * står for alle sidene, og 1-5
for en rekke sider eller en enkeltside.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Angi sideinnbindinga i pdf-fila. Med mindre du vet det skal endres er det best å beholde standardvalget – Venstre.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Lager miniatyrbilder av alle sidene i pdf-fila. Noen visningsprogrammer kan bruke disse til å bla i fila med.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Sett inn bokmerkene du lagde i dokumentet i pdf-fila. Disse er nyttige hvis du skal lese eller bla i lange pdf-dokumenter</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Oppløsning på teksten og vektorbildene. Dette endrer ikke oppløsninga på bitmap-bilder som f.eks. fotografier.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Bruk tapsfri komprimering av tekst og bilder. Med mindre du har en god grunn bør du krysse av for dette. Det vil redusere størrelsen på pdf-fila.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Bruk sikkerhetsfunksjonene i det eksporterte pdf-dokumentet. Hvis du valgte pdf 1.3, så vil pdf-fila være beskyttet av 40 bits kryptering. Hvis du valgte pdf 1.4, så vil pdf-fila være beskyttet av 128 bits kryptering. NB: pdf-kryptering er ikke like pålitelig som GPG- eller PGP-kryptering og har dessuten en del begrensninger.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Velg et hovedpassord som skrur av eller på alle sikkerhetsegenskapene i det eksporterte pdf-dokumentet</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Dette er en avansert innstilling, som ikke er på som standard. Denne bør bare skrus på hvis trykkeriet ber spesielt om det og oppgir nøyaktige detaljer om innstillingene. Ellers vil den eksporterte pdf-fila ikke kunne skrives ut på ordentlig vis og ikke være portabel på tvers av systemer.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Obligatorisk streng for pdf/X-3. Må fylles ut for å overholde reglene for pdf/X-3. Vi anbefaler at du bruker navnet på dokumentet.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Visningsinnstillinger</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Enkeltsider</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Fortløpende</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Dobbeltsidig, venstre</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Dobbeltsidig, høyre</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Visuelt utseende</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Bruk standardinnstillingene til pdf-framviseren</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Vis i fullskjerm</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Vis fanen for bokmerker</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Vis miniatyrbilder</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Vis fane for lag</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Skjul framviserens verktøylinje</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Skjul framviserens menylinje</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Skaler sidene til framviserens vindu</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Spesielle handlinger</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Ingen skript</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Framviser</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Skjær av ved sidemargene</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Jpg-komprimering med kvalitetstap</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Tapsfri Zip-komprimering</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Metode for komprimering av bilder</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript som skal utføres
når pdf-dokument åpnes:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Skru på presentasjons-effektene når Adobe&amp;#174; Reader&amp;#174; blir brukt eller andre pdf-lesere som støtter dette i fullskerm.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Bestemmer pdf-samspillet. Standarden er pdf 1.3, som gir det beste samspillet. Velg pdf 1.4, hvis fila har egenskaper som f.eks. gjennomsiktighet, eller hvis du vil ha 128 bits kryptering. Du trenger pdf 1.5 hvis du vil bevare objektene i egne lag i pdf-fila. Pdf/X-3 brukes for å eksportere til pdf når du trenger fargehåndtering for å sende fila til et trykkeri. Du kan bare velge dette hvis du har skrudd på fargehåndtering. Bare bruk dette hvis trykkeriet ber om det, eller – i noen tilfeller – når du skal skrive ut i firefargetrykk til en laserskriver.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Lag i dokumentet blir eksportert til pdf-fila. Dette er bare tilgjengelig hvis du har valgt pdf 1.5.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Endrer oppløsninga på bitmap-bildene til det du velger her. Hvis du skrur dette av, så brukes den oppløsninga bildene har fra før.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Fargemodel for utdata av pdf-dokumentet. Velg Skjerm/Internett for pdf-filer som skal vises på skjermen eller skrives ut på vanlige blekkskrivere. Velg «Skriver» hvis dokumentet skal skrives ut på en 4 fargers CMYK-skriver. Velg gråskala hvis du bare vil bruke gråtoner i pdf-fila.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Vis ikke objekter utenfor margene i den eksporterte fila</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Den tiden siden vises før presentasjonen starter på den valgte siden. Verdien 0 skrur av automatiske sideskift.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation>Komprimeringsmetode for bilder. Lar Scribus automatisk velge den beste metoden. Zip er tapsfri og passer godt for bilder med ensfargede farger. Jpg er bedre hvis du vil ha små pdf-filer og har mange bilder (men gir gjerne litt lavere bildekvalitet). La innstillinga stå på «Automatisk» med mindre du har bruk for en spesiell slags komprimering.</translation>
    </message>
    <message>
        <source>&amp;Embed All</source>
        <translation>&amp;Bygg inn alle</translation>
    </message>
    <message>
        <source>Fonts to outline:</source>
        <translation>Gjør skriftene om til omriss:</translation>
    </message>
    <message>
        <source>Outline &amp;All</source>
        <translation>Gjør alle skriftene om til omriss:</translation>
    </message>
    <message>
        <source>Document Layout</source>
        <translation>Dokumentoppsett</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6. </source>
        <translation>Ta med skriftene i PDF-fila. Hvis du tar med skriftene, så opprettholder du samme oppsett og samme utseende fordi den som leser kan bruke de samme skriftene. Noen skrifter kan bare hentes inn som avkortede tegnfiler, det vil si at bare de tegnene som er i bruk i dokumentet bygges inn og ikke alle tilgjengelige tegn. Open Type -skrifter kan bare bygges inn som avkortede tegnfiler i PDF-versjoner lavere enn 1.6.</translation>
    </message>
    <message>
        <source>Subset all fonts into the PDF. Subsetting fonts is when only the glyphs used in the PDF are embedded, not the whole font. Some fonts like Open Type can only be subset, as they are not able to be embedded into PDF versions before PDF 1.6.</source>
        <translation>Bygg inn alle skriftene i PDF-fila som avkortede tegnfiler. Avkortede tegnfiler inneholder bare de tegnene som er i bruk i dokumentet og ikke alle tilgjengelige tegn. Noen skrifter vil bare kunne bygges inn i avkortet form. Open Type -skrifter kan bare bygges inn som avkortede tegnfiler i PDF-versjoner lavere enn 1.6.</translation>
    </message>
    <message>
        <source>Quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level. Even with Maximum selected, there is always some quality loss with jpeg.</source>
        <translation>Kvalitetsnivå for komprimeringsmetoder med kvalitetstap: Laveste (25%), Lav (50%), Medium (75%), Høy (85%), Høyeste (95%). Kvalitetsnivået kan ikke direkte sammenlignes med bildestørrelsen &amp;ndash; både størrelsen og kvalitetstapet vil variere fra bilde til bilde på ulike kvalitetsnivåer. Selv på høyeste nivå vil jpg gi kvalitetsttap.</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Skrifttype:</translation>
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
        <translation>Fyllfarge:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Strekfarge:</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Fylltegn for tabulatorområder</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Tabuleringsbredde:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Avstand:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Vår sære Zulu fra badeøya spilte jo whist og quickstep i min taxi</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Strekfarge:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Metningsgrad:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>&amp;Metningsgrad:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Linjestil:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Strekbredde</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>&amp;Linjestil:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Piler:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Start:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Slutt:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Fri skalering</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Vannrett skalering:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Loddrett skalering:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Skaler bildet til rammestørrelsen</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Behold &amp;størrelsesforholdet</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Fyllfarge:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation type="unfinished">Bruk den innebygde klippestien</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Forhåndsvisning på skjermen</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Vis i full oppløsning</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Vis i normal oppløsning</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Vis i lav oppløsning</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>&amp;Minimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>M&amp;aksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Trinn:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Egenskaper for tekstfelt</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Egenskaper for bilderammer</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Egenskaper for formtegning</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Standard forstørrelsesgrad</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Egenskaper for tegning av linjer</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Tegneegenskaper for mangekanter</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Skrifttype for nye tekstrammer</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Størrelse på skrifta i nye tekstrammer</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Skriftfarge</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Antall kolonner i en tekstramme</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Avstand mellom kolonnene i tekstrammene</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Forhåndsvisning av skrifttypen</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>I en bilderammene kan bildene skaleres til enhver størrelse</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Vannrett skalering av bilder</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Loddrett skalering av bilder</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Samme skalering vannrett og loddrett</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Bilder i bilderammer skaleres opp til samme størrelse som ramma</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automatisk skalerte bilder beholder samme størrelsesforhold</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Fyllfarge på bilderammer</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Fyllfargens metningsgrad</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Strekfarge på formene</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Strekfargenes metning</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Fyllfarge på formene</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Formenes linjestil</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Formenes strekbredde</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Minste tillatte forstørrelse</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Største tillatte forstørrelse</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Skaleringsgrad for hver skaleringsoperasjon</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Strekfarge</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Fargemetning</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Linjestil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Strekbredde</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Selvvalgt:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Selvvalgt: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Tekstfarge:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Metningsgrad:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation type="unfinished">Tekststrøk:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Prikk</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Bindestrek</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Understrekning</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Senket skrift</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Forskyvning:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Skalering:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hevet skrift</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>&amp;Forskyvning:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>&amp;Skalering:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understrekning</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Forskyvning:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Strekbredde:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Overstrøket</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapiteler</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>&amp;Skalering:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Automatisk &amp;linjeavstand</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Linjeavstand:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Hvor langt over grunnlinja en skrifttype skal forskyves</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Størrelsesforholdet mellom hevet skrift og vanlig skrift</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Forskyvning under bunnlinja for en skrifttype på en linje</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Størrelsesforholdet mellom senket skrift og vanlig skrift</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Størrelsesforholdet mellom kapiteler og vanlig skrift</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Hvor mye større linjeavstanden skal være enn skriftstørrelsen, målt i prosent</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation type="unfinished">Hvor mye senket skrift skal senkes i forhold til skriftas grunnlinje, målt i prosent av skrifthøyden</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Bredden på streken, angitt som en prosentandel av skriftstørrelsen</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation type="unfinished">Hvor mye hevet skrift skal heves i forhold til skriftas grunnlinje, målt i prosent av skrifthøyden</translation>
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
        <translation>Høyre</translation>
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
        <translation>Midten</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Slett alle</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Innrykk for første linje av avsnittet</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Innrykk fra venstre for hele avsnittet</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Slett alle tabulatorene</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation type="unfinished">&amp;Posisjon:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ingen</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Fylltegn:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Selvvalgt:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Selvvalgt: </translation>
    </message>
    <message>
        <source>Dot</source>
        <translation type="unfinished">Prikk</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Bindestrek</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation type="unfinished">Understreket</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Selvvalgt</translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished">Ingen</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Oversikt</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Gruppe </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Frie objekter</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Side </translation>
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
        <translation>Forskyvning</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linjebredde</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Legg til loddrett hjelpelinje</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Legg til vannrett hjelpelinje</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Fjern loddrett hjelpelinje</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Fjern vannrett hjelpelinje</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Flytt loddrett hjelpelinje</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Flytt vannrett hjelpelinje</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Lås hjelpelinjene</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Lås opp hjelpelinjene</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Flytt</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Endre størrelse</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Roter</translation>
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
        <translation>Utvalg</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="unfinished">Grupper</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Markering/Gruppe</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Opprett</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Juster/Fordel</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Elementer involvert</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Avbryt</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Angi fyllfarge</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Farge1: %1, Farge2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Angi metningsgraden på fyllfargen</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Angi strekfargen</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Angi metningsgraden på strekfargen</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Speilvend vannrett</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Speilvend loddrett</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Lås</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Lås opp</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Lås størrelsen</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Lås opp størrelsen</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Løs opp gruppa</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slett</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Endre navn</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Fra %1
til %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Bruk malside</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Lim inn</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Klipp ut</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Angi gjennomsiktigheten på fyllfargen</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Angi gjennomsiktigheten på strekfargen</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Angi linjestil</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation type="unfinished">Angi stilen på linjeslutten</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation type="unfinished">Velg linjesammenslutning</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Velg strekbredde</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Ingen stil</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Velg en selvvalgt linjestil</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Ikke bruk selvvalgte linjestiler</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation type="unfinished">Velg startpil</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation type="unfinished">Velg sluttpil</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Lag tabell</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Rader: %1, Kolonner: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Velg skrifttype</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Velg skriftstørrelse</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation type="unfinished">Velg skriftbredde</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Velg skrifthøyde</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Velg fyllfarge på skrifta</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Velg strekfarge på skrifta</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Velg metningsgraden på fyllfargen på skrifta</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Velg strekfargens metningsgrad på skrifttypen</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Velg kerning</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Velg linjeavstand</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Velg avsnittsstil</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Velg språk</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation type="unfinished">Juster teksten</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Velg skrifteffekter</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Bilderamme</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Mangekant</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bezierkurve</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinje</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Gjør om til</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Importer SVG-bilde</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importer EPS-bilde</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Importer OpenOffice.org Draw-bilde</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Arbeidsbord</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>La teksten flyte rundt ramma</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>La teksten flyte rundt grenseboksen</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>La teksten flyte rundt konturlinjene</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Ingen tekstflyt</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Ingen grenseboks</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Ingen konturlinje</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Side %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Velg skaleringa av bildet</translation>
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
        <translation>Behold &amp;størrelsesforholdet</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Endre størrelsesforholdet</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Rediger konturlinja</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Rediger formen</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Nullstil konturlinja</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Legg til side</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Legg til sider</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Slett side</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Slett sider</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Legg til lag</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Slett lag</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Gi nytt navn til et lag</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Hev lag</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Senk lag</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Flytt til lag</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Skru på utskrift av lag</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Skru av utskrift av lag</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Gi laget et annet navn</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Sett inn et bilde</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Tekst på en sti</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Skru på utskrivning av elementer</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Skru av utskrivning av elementer</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Lag flere kopier</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Endre bilde-forskyvninga</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Endre bildeskaleringa</translation>
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
        <translation>Nullstill kontrollpunktet</translation>
    </message>
    <message>
        <source>Reset control points</source>
        <translation>Nullstill kontrollpunktene</translation>
    </message>
    <message>
        <source>Modify image effects</source>
        <translation>Endre bildeeffektene</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Starttilstanden</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Handlingshistorie</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Bare vis de valgte objektene</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Angre</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Gjør om</translation>
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
        <translation type="unfinished">Prøver å få tak i oppdateringsfila for denne versjonen av Scribus</translation>
    </message>
    <message>
        <source>(No data on your computer will be sent to an external location)</source>
        <translation>(Det vil ikke bli sendt data fra din datamaskin til et annet sted)</translation>
    </message>
    <message>
        <source>Timed out when attempting to get update file.</source>
        <translation>Tidsavbrudd ved forsøker på å laste ned oppdateringsfila</translation>
    </message>
    <message>
        <source>Error when attempting to get update file: %1</source>
        <translation>Det oppsto en feil da oppdateringsfila skulle hentes: %1</translation>
    </message>
    <message>
        <source>File not found on server</source>
        <translation>Fant ikke fila på tjeneren</translation>
    </message>
    <message>
        <source>Could not open version file: %1
Error:%2 at line: %3, row: %4</source>
        <translation>Klarte ikke å åpne versjonsfila: %1 
Feil: %2 på linje: %3, rad: %4</translation>
    </message>
    <message>
        <source>An error occurred while looking for updates for Scribus, please check your internet connection.</source>
        <translation>Det oppsto en feil når maskinen lette etter oppdateringer for Scribus. Sjekk om internettforbindelsen din er i orden.</translation>
    </message>
    <message>
        <source>No updates are available for your version of Scribus %1</source>
        <translation>Det finnes ingen oppdateringer for din versjon av Scribus %1</translation>
    </message>
    <message>
        <source>One or more updates for your version of Scribus (%1) are available:</source>
        <translation>Det finnes en eller flere oppdateringer for din versjon av Scribus (%1)</translation>
    </message>
    <message>
        <source>This list may contain development versions.</source>
        <translation>Denne lista kan inneholde utviklingsversjoner</translation>
    </message>
    <message>
        <source>Please visit www.scribus.net for details.</source>
        <translation>Se www.scribus.net for flere detaljer</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Minstemargin for sidestørrelsen %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Bruk skrivermargene</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Velg &amp;skriver:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marger</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Høyre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Øvre:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Nedre:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Greit</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt + O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Avbryt</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt + C</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Skriv inn en verdi</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Skriv inn en verdi, og trykk på «Greit».</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Skriv inn en verdi, og trykk på «Greit»</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt + O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Send din verdi til skriptet</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Verktøy</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Egenskaper ...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Button</source>
        <translation>Knapp</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstfelt</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Avkryssingsboks</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinasjonsboks</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listeboks</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lenke</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>Pdf-verktøy</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Sett inn et pdf-felt</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Sett inn pdf-kommentarer</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Velg hvilket filter som skal brukes ved import</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importer tekst uten å formatere den</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importer:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Koding:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Bare importer teksten</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åpne</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Velg hvordan du vil importere</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Husk filendelsen</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Lagre hvilket importfilter som skal brukes for filer med denne filendelsen, og ikke spør igjen for andre filer av samme type.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Ny fra mal</translation>
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
        <translation>Farger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Laget med</translation>
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
        <translation>&amp;Åpne</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Last ned maler</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Installer maler</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Pakk ut til den midlertidige mappa «~/.scribus/templates» som tilhører den gjeldende brukeren, eller bruk «PREFIX/share/scribus/templates» for alle brukerne på systemet.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Klargjør en mal</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Fjern en mal</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Oversetter template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Dokumentmaler finnes på http://www.scribus.net, se under «Downloads».</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Sørg for at bildene og skrifttypene du bruker er fritt tilgjengelige. Hvis skrifttypen ikke kan bli delt med andre, ikke saml dem når dokumentet lagres som en mal.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Maloppretteren sørger også for at avsnittet for installasjonsmalen overfor også passer til deres maler. Dette betyr at brukeren skulle kunne være i stand til å laste ned en samling med maler, pakke dem ut i malmappa og starte å bruke dem.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Det å fjerne en mal fra dialogvinduet «Ny mal» vil bare fjerne den fra template.xml, det vil ikke slette dokumentfilene. En oppsprettsmeny som spør om du vil fjerne den vises bare hvis du har skrivetilgang til fila «template.xml».</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopier en eksisterende «template.xml» til en fil med navnet «template.språk_LAND.xml» (bruk samme språkkode som er brukt i qm-fila for ditt språk), for eksempel «template.nb_NO.xml» for en versjon på norsk bokmål av «template.xml». Kopien må ligge i samme mappe som den opprinnelige «template.xml» for at Scribus skal kunne bruke den.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Dato</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Lagre som mal</translation>
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
        <translation>Farger</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Forfatter</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>E-post</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Flere detaljer</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Greit</translation>
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
        <translation>liggende</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>stående</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>selvvalgt</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Lag et filter</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Tøm</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Slett</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Velg et tidligere lagret filter</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Gi et navn til dette filteret så det kan lagres</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Oppgi et navn før du lagrer</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Fjern eller skru av denne filter-raden</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Fjern denne filter-raden</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Legg til et nytt filter</translation>
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
        <translation>fjern treffene</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ikke fjern treffene</translation>
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
        <translation>Erstatt</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Bruk</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Verdien til venstre er et regulært uttrykk</translation>
    </message>
    <message>
        <source>with</source>
        <translation>med</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>avsnittsstil</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>alle tilfeller av</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>alle avsnittene</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>avsnitt som begynner med</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>avsnitt med mindre enn</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>avsnitt med mer enn</translation>
    </message>
</context>
</TS>
