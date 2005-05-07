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
        <translation>newDoc(størrelse, marginer, retning, førsteSideNummer,
                   måle-enhed, modståendeSider, førsteSideVenstre) -&gt; bool

Laver et Nyt dokument og returnerer &quot;true&quot; hvis det lykkedes. Parametrene har 
følgende betydning:

    størrelse = A tuple (bredde, højde) beskriver størrelsen af dokumentet. du kan
    bruge predefinerede constanter med navnet PAPER_&lt;paper_type&gt; f.eks. PAPER_A4 etc.

    marginer = En tuple (venstre, højre, top, bund) beskriver documentets marginer

    retning = sidens retning - constanter PORTRAIT, LANDSCAPE

    førsteSideNummer = er nummeret på den første side i dokumentet brugt for
    side-nummerering. Selvom værdien oftest er 1, så er der nogen gange brug for 
    et højere nummer, når et dokument består af flere dele.

    enhed: denne værdi sætter måle-enheden, som bruges i dokumentet. Brug en
    predefineret constant for dette, en af: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    modståendeSider = FACINGPAGES, NOFACINGPAGES

    førsteSideVenstre = FIRSTPAGELEFT, FIRSTPAGERIGHT

Værdierne for bredde, højde og marginer er udtrykt i den givne måle-enhed
for dokumentet. PAPER_* constanter er udtrykt i punkter. Hvis dit dokument
ikke er i punkter, så sørg for at tage højde for dette.

eksempel: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
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
        <source>newPage(where [,&quot;template&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;template&quot; specifies the name of the
template page for the new page.

May raise IndexError if the page number is out of range
</source>
        <translation>newPage(hvor [,&quot;skabelon&quot;])

Laver en ny side. Hvis &quot;hvor&quot; er -1, så tilføjes den nye side i slutningen
af dokumentet, ellers er den ny side indsat før &quot;hvor&quot;. Side-numre er
talt fra 1 og opefter, lige meget hvad nummer den første side har fået.
Den valgfri parameter &quot;skabelon&quot; angiver navnet på den skabelon, der
skal bruges til den nye side.

Kan rejse IndexError hvis side-nummer er uden for tilladte værdi
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

Returnerer font-størrelsen i punkter for tekst-ramme &quot;navn&quot;. Hvis denne
tekst-ramme har noget tekst markeret, så returneres værdien for første
karakter i den markerede tekst.
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
        <translation>register_macro_callable(navn, kørbar_objekt, accel=&apos;&apos;)

Laver en makro med navnet &quot;navn&quot; med det eksisterende kørbare objekt &quot;kørbar_objekt&quot;.
Det givne kørbar_objekt må ikke kræve parametre når det kaldes (det må godt tage
valgfri parametre, men der bliver ikke givet nogen).
Hvis strengen &quot;accel&quot; er givet, vil den blive brugt til at sætte en tastatur genvej for makroen.
Hvis det givne kørbar_objekt er en klasse, vil den blive afvist. Funktioner og bundne
metoder er acceptable, og det gælder også  instanser af klasser der giver en __call__ metode 
uden argumenter. Der er ikke noget problem med at registrere et kørbar_objekt mere en 
gang, eller med at registrere flere bundne metoder af en enkelt instans.
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
        <translation>fileDialog(&quot;overskrift&quot;, [&quot;filter&quot;, &quot;standardnavn&quot; ,har-previsning, er-gem]) -&gt; streng med fil-navn

Viser en &quot;Fil-åbn&quot; dialog-boks med overskriften &quot;overskrift&quot;. Filer er filtreret
med filter-strengen &quot;filter&quot;. Et standard fil-navn eller fil-sti kan også
angives, lad denne streng være tom, når du ikke ønsker at bruge den.
&quot;True&quot; for har-previsning aktiverer et lille visningsvindue i fil-valg-boksen.  Når
er-gem værdien er sat til &quot;True&quot;, så fungerer dialogboksen som en &quot;Gem som&quot; dialog,
ellers fungerer den som en &quot;Fil åbn&quot; dialog. Standardværdien for begge de to valgfri
parametre er &quot;False.&quot;

Filteret, hvis specificeret, har formen &apos;kommentar (*.type *.type2 ...)&apos;.
For eksempel &apos;billeder&apos; (*.png *.xpm *.jpg)&apos;.

Se Qt-dokumentationen over QFileDialog for detaljer om filtre.

Eksempel: fileDialog(&apos;Åbn input&apos;, &apos;CSV filer (*.csv)&apos;)
Eksempel: fileDialog(&apos;Gem report&apos;, standardnavn=&apos;report.txt&apos;, er-gem=True)
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
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;forfatter&quot;, &quot;info&quot;, &quot;beskrivelse&quot;) -&gt; bool

Sætter dokumentets information. &quot;forfatter&quot;, &quot;Info&quot;, &quot;Beskrivelse&quot; 
er strenge.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Sætter dokumentets marginer, Venstre(lr), Højre(rr), Top(tr) og Bund(br)
marginer er angivet i dokumentets måle-enhed - se UNIT_&lt;type&gt; konstanter.
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

Læser afsnits-typografierne fra Scribus dokumentet &quot;filnavn&quot; 
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
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getCornerRadius([&quot;navn&quot;]) -&gt; integer

Returnerer radius på hjørnerne på objektet &quot;navn&quot;. Radius 
er udtrykt i punkter. Hvis &quot;navn&quot; ikke angives, så bruges 
det markerede element.
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

Læser billedet &quot;filnavn&quot; ind i billed-rammen &quot;navn&quot;.  Hvis &quot;navn&quot; 
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
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;lag&quot;, printbar)

Sætter laget &quot;lag&quot; til at kunne udskrives eller ej. Hvis &quot;printbar&quot; sættes til 
&quot;False&quot;, så bliver laget ikke udskrivet.

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
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;navn&quot; [, tilstand]

Aktiverer/deaktiverer &quot;Tekst flyder uden om ramme&quot; egenskab for objektet &quot;navn&quot;
Kaldes med navn og valgfri boolean  &quot;tilstand&quot;. Hvis &quot;tilstand&quot; ikke angives, så
bliver &quot;tekst flyder uden om&quot; tilstanden sat til det modsatte af det, det var.
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
har noget tekst markeret, så returneres værdien for første karakter i den 
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

Konverterer tekst-rammen &quot;navn&quot; til omrids. Hvis &quot;navn&quot; ikke angives, 
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
Både `skalertilramme&apos; og `proportional&apos; er boolean.

Kan rejse  WrongFrameTypeError.
</translation>
    </message>
    <message>
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared. If &quot;count&quot; is -1, all text in the frame will
be selected. If &quot;name&quot; is not given the currently selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation>selectText(start, antal, [&quot;navn&quot;])

Markerer &quot;antal&quot; karakterer af teksten i tekst-rammen &quot;navn&quot; startende fra
karakter &quot;start&quot;. Karakter tælling starter ved 0. Hvis &quot;antal&quot; er nul, vil enhver 
tekstmarkering blive nulstillet. Hvis &quot;antal&quot; er -1, vil al tekst i rammen blive 
markeret. Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse IndexError hvis hvis markeringen er sat udenfor teksten.
</translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
</source>
        <translation>register_macro_code(navn, kildetekst, accel=&apos;&apos;)

Lav en makro der hedder &quot;navn&quot; ved at vurdere kildekoden &quot;kildetekst&quot;.
&quot;kildetekst&quot; skal følge de samme regler som makroer lavet i GUI&apos;en.
Hvis angivet, så vil strengen &quot;accel&quot; blive brugt som tastatur-genvej for makroen.
</translation>
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
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Returnerer side marginer som en (top, venstre, højre, bund) tuple i dokumentets 
aktuelle måle-enhed. Se  UNIT_&lt;type&gt; konstanter og getPageSize().
</translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference). The first character has
an index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot;
is not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation>insertText(&quot;tekst&quot;, pos, [&quot;navn&quot;])

Indsætter teksten &quot;tekst&quot; ved position &quot;pos&quot; i tekst-rammen &quot;navn&quot;. 
Teksten skal være UTF kodet (se  setText() som reference). Første karakter
har index 0. Indsættes ved position -1, så placeres teksten i slutningen af  
rammen. Hvis &quot;navn&quot; ikke er givet, så bruges det markerede element.

Kan rejse IndexError for en indsætning uden for et gyldigt område.
</translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Finnish:</source>
        <translation>Finsk:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Brasiliansk:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galicisk:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Tjekkisk:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Walisisk:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Bidrag fra:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Polsk:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ukrainsk:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Slovakisk:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italiensk:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Dansk:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Litaurisk:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Windows port:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Tyrkisk:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Russisk:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Ungarsk:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Fransk:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Bulgarsk:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norsk:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Tysk:</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Scribus Version %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Build-ID:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Slovensk:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Baskisk:</translation>
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
        <source>Swedish:</source>
        <translation>Svensk:</translation>
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
        <source>English (British):</source>
        <translation>Engelsk (Britisk):</translation>
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
        <source>Catalan:</source>
        <translation>Catalansk:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>Koreansk:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>Spansk:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>Esperanto:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>Serbisk:</translation>
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
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Dette panel viser versionsnummer, dato for kompilering 
og den indbyggede biblioteksunderstøttelse i Scribus
C-C-T står for C=littlecms C=CUPS T=TIFF understøttelse.
Manglende biblioteksunderstøttelse er vist ved en *</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation>%1 %2 %3 </translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation>Afrikaans:</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation>Portugisisk (Brasiliansk):</translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Avancerede indstillinger</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Laver PostScript niveau 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Laver kun Postscript niveau 2, 
dette kan forårsage meget store filer</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Laver kun Postscript niveau 1, 
dette kan forårsage meget store filer</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Spejl side(r) &amp;horisontalt</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Spejl side(r) &amp;vertikalt</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Anvend &amp;ICC-Profiler</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript niveau &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript niveau &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript niveau &amp;3</translation>
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
        <source>Apply Under Color &amp;Removal</source>
        <translation>Anvend &amp;Reduktion af underliggende farve</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En måde at fjerne nogle af gråtonerne, som er sammensat
af cyan, gul og magenta, så der i stedet for disse farver bruges sort.
UCR påvirker mest de dele af billedet, som er neutrale og/eller mørke i farven, det, 
som er tættets på grå. Brug af denne metode kan forbedre udskrivningen af nogle 
billeder og nogen eksperimenteren og testning er nødvendigt fra billede til billede.
UCR formindsker muligheden for overmætning med CMY farverne.</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Angiv medie størrelse</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation>Dette gør dig i stand til direkte at 
angive medie størrelsen i postscript filen.
Anbefales ikke med mindre
det kræves af din printer.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Align</source>
        <translation>Opstil på linie</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontalt</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Øverste sider</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Midten</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Højre sider</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Venstre sider</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Nederste sider</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikalt</translation>
    </message>
    <message>
        <source>Distribute/Align</source>
        <translation>Fordel/Opstil på linie</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>A&amp;nvend</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Ud fra:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>Opstil p&amp;å linie</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>Fors&amp;kydning</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Fordel &amp;jævnt</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>Ud &amp;fra:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>Foretag &amp;ingen ændring</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Opstil på &amp;linie</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Forsk&amp;ydning</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Fordel j&amp;ævnt</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>Foreta&amp;g ingen ændring</translation>
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
        <translation>Bruger</translation>
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
        <source>Java Script</source>
        <translation>Javascript</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Hjælpe-tekst:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Felt mister fokus</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Eksporter ikke værdi</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Billeder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle filer (*)</translation>
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
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Anvend skabelon</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Skabelon:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Anvend på &amp;denne side</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Anvend fra &amp;side:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Til:</translation>
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
        <source>Apply to all &amp;even Pages</source>
        <translation>Anvend på alle &amp;lige sider</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Anvend på alle &amp;ulige sider</translation>
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
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Scrapbøger (*.scs);; Alle filer (*)</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.
Vælg venligst et andet.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Indlæs...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gem &amp;Som...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Lille</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Medium</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Stor</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fil</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Vis</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
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
        <translation>Absolut kolorimetrisk</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>System profiler</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Hensigt med gengivelse</translation>
    </message>
    <message>
        <source>Color Management Settings</source>
        <translation>Farvestyrings indstillinger</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Opfattelsesmæssigt</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolorimetrisk</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Standard Farveprofil for importerede billeder</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Standard Farveprofil for massive farver på siden</translation>
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
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard gengivelse beregnet for din skærm. Hvis du ikke ved hvorfor man ændrer det,
vælg da relativ kolorimetrisk eller opfattelsesmæssigt.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard gengivelse beregnet for din printer. Hvis du ikke ved hvorfor man ændrer det,
vælg da relativ kolorimetrisk eller opfattelsesmæssigt.</translation>
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
        <source>&amp;Pictures:</source>
        <translation>&amp;Billeder:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Massive farver:</translation>
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
        <source>M&amp;onitor:</source>
        <translation>Sk&amp;ærm:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Pr&amp;inter:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
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
        <source>OK</source>
        <translation>OK</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Navnet på denne farve er ikke entydig</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Du kan ikke lave en farve, der hedder &quot;%1&quot;.
Navnet er reserveret for gennemsigtig farve</translation>
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
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
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
        <source>Color of selected object</source>
        <translation>Det valgte objekts farve</translation>
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
        <source>Free radial Gradient</source>
        <translation>Fri radial gradient</translation>
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
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>No</source>
        <translation>Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
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
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Dette panel viser forskellige CUPS valgmuligheder for udskrivning.
De precise parametre, som er tilgængelige afhænger af printerdriveren.
Du kan se om CUPS er understøttet ved at vælge Hjælp &gt; Om Scribus.
Se efter: C-C-T Disse står for C=CUPS C=littlecms T=TIFF understøttelse.
Manglende biblioteksunderstøttelse er vist ved en *</translation>
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
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Slet farve</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Erstat den med:</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Slet farve:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
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
    <name>DelPages</name>
    <message>
        <source>to:</source>
        <translation>til:</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Slet fra:</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation>Slet sider</translation>
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
    <name>DmF</name>
    <message>
        <source>Use</source>
        <translation>Brug</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>i stedet</translation>
    </message>
    <message>
        <source>Missing Font</source>
        <translation>Manglende font</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Fonten %1 er ikke installeret.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
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
        <source>&amp;Document</source>
        <translation>&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Yderligere &amp;Information</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Personen eller organisationen der hovedsaligt er ansvarlig for dokumentets indhold.
Dette felt kan indlejres i Scribus dokumentet, såvel som i et PDF-dokuments metadata</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Et navn som dokumentet har fået.
Dette felt kan indlejres i Scribus dokumentet som reference, såvel som i et PDF-dokuments metadata</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>En beskrivelse af dokumentets indhold.
Dette felt er beregnet til en kort beskrivelse eller resume af dokumentet. Det indlejres i PDF-filen ved eksport</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Emnet for dokumentets indhold.
Dette felt er for dokument-nøgleord, som du ønsker at indlejre i en PDF-fil til hjælp for søgning og indeksering</translation>
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
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Dokumentets fysiske eller digitale udformning. Medietype og omfang kunne være værd at nævne.
RFC2045,RFC2046 for MIME typer er også brugbart her</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>En utvetydig reference til dokumentet inden for et givet sammenhæng som f.eks. ISBN eller URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Det sprog, som dokumentet er skrevet i, sædvanligvis en ISO-639 sprogkode
eventuelt efterfulgt af en bindestreg og en ISI-3166 landekode, f.eks en-GB, fr-CH</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>En reference til et dokument fra hvilket det aktuelle dokument er udledt af, f.eks ISBN eller URI</translation>
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
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Postscript-filer (*.ps);; Alle filer (*)</translation>
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
        <source>Print &amp;Normal</source>
        <translation>Udskriv &amp;normalt</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Udskriv &amp;separationer</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Udskriv i fa&amp;rve hvis det er tilgængeligt</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Udskriv i gr&amp;åskala</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Avan&amp;cerede indstillinger...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Udskriv</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Brug en alternativ printer håndtering, som f.eks. kprinter eller gtklp, 
for at kunne få flere printerindstillinger</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Importering af fil:
%1
fejlede!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatal fejl</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation>Redigering af makro: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Macro håndtering</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation>Filen &apos;%1&apos; eksisterer allerede.
Er du sikker på, at du ønsker at overskrive den?
</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation>Du har allerede redigeret i denne makro.
Er du sikker på, at du ønsker at kassere dine ændringer?
</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation>En fuldstændig tilbagesporing følger:

%1
</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation>Komplleringen af makroen mislykkedes, så den 
kan ikke gemmes i sin nuværende form. Fejlen var:
%1
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation>Scribus - Ny makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Dette er Redigér makro / Ny makro dialog vinduet. Her kan du ændre makroers kildekode. Redigere makroens kildekode i
tekst redigerings området under &quot;Kildekode&quot; mærkaten og klik OK for at gemme dine ændringer til makroen.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation>Kildekode:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation>Redigering af makro:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation>Navnet på makroen, der redigeres.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation>&lt;qt&gt;Dette element viser navnet på den makro, du lige nu redigerer.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Forkast alle ændringer og afslut.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Hvis du afslutter redigeringsvinduet, så mister du alle ændringer, du har lavet. Hvis du ønsker at afslutte uden at gemme makroen, men ikke ønsker at miste dine ændringer, så gem dine ændringer med &quot;Gem kilde som...&quot;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Gem ændringer og afslut.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Gem ændringer til makroen og afslut. Hvis der er et problem med makroen, så vil en besked blive vist, og redigeringsvinduet vil ikke blive lukket.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Dette tekstområde indeholder makroens kildekode. Hvis du laver en ny makro, så vil der ikke være noget i det, og hvis du redigerer en eksisterende makro, så vil kildekoden, der definerede makroen blive vist her.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation>&amp;Indlæs kildetekst ...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Erstat den nuværende kildekode med kode fra en fil.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Indlæs ny kildekode fra &quot;fil &quot;for redigering. Al kildekode i redigeringsområdet bliver overskrevet. Den indlæste kildekode skal være en Scribus makrofunktion. Hvis du indlæser en anden slags script, så skal den redigeres, så den fungerer som en scripter makro, før den kan gemmes.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation>&amp;Gem kildetekst som...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Gem kildekoden, som redigeres, til en fil.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation>Gem makroens kildekode - tekst - til en fil. Du kan redigere den gemte kilde og indlæse den igen med &quot;Indlæs kilde...&quot;. </translation>
    </message>
    <message>
        <source>Python source files (.py)</source>
        <translation>Python kildetekst filer (.py)</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation>Gem fil dialogvindue</translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation>Gem makro kildetekst</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation>Åbn fil dialogvindue</translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation>Vælg kildetekst filen, der skal indlæses</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Navnet på denne typografi er ikke entydig</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Redigér typografi</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Tegn</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Effekt:</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Vertikal afstand</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
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
        <source>OK</source>
        <translation>OK</translation>
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
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Laver det første bogstav i et afsnit større. Bruges som blikfang</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Bestemmer højden af unicialen i antal linier</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Juster tekst til basislinie-gitter</translation>
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
        <source>&amp;Font:</source>
        <translation>&amp;Font:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>St&amp;ørrelse:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Justering:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Uncialer</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linier:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>F&amp;yldfarve:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>St&amp;regfarve:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Juster til basislinie-&amp;gitter</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>Linie af&amp;stand:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>O&amp;ver:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Under:</translation>
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
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Editor</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);; Alle filer (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
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
        <translation>S&amp;let</translation>
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
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>A&amp;lle sider</translation>
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
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farver</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filer (*)</translation>
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
        <source>New Color</source>
        <translation>Ny farve</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi af %1</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Vælg et navn</translation>
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
        <source>Append colors to the current set from an existing document</source>
        <translation>Tilføj farver til det aktive sæt fra et eksisterende dokument</translation>
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
        <source>&amp;Append</source>
        <translation>&amp;Tilføj</translation>
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
        <source>&amp;Remove Unused</source>
        <translation>&amp;Fjern ubrugte</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Gem farvesæt</translation>
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
        <source>&amp;Name:</source>
        <translation>&amp;Navn:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Font navn</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Erstatnings fonte</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Substituere</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Erstatning</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Indlejr i:</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Brug font</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Global Font Settings</source>
        <translation>Globale fontindstillinger</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Sti til fontfil</translation>
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
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
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
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Vævet silkepyjamas byttet med blå kvarts</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Font visning</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation>Tilføj den markerede font til typografi, Font menu</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation>Afslut visning</translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation>Fontstørrelse</translation>
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
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Her kan du tilføje, ændre og fjerne farve-stop.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Horizontal Guides</source>
        <translation>Horisontale hjælpelinier</translation>
    </message>
    <message>
        <source>Manage Guides</source>
        <translation>Håndtér hjælpelinier</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Vertikale hjælpelinier</translation>
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
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>Sl&amp;et</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Pos:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Til&amp;føj</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>S&amp;let</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Lås hjælpelinier</translation>
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
        <source>S&amp;earch</source>
        <translation>S&amp;øg</translation>
    </message>
    <message>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>ukendt</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Find</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Søgeord:</translation>
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
        <source>Hyphenator Settings</source>
        <translation>Orddelings instillinger</translation>
    </message>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
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
        <source>Inserting</source>
        <translation>Indsætter</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>efter side</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Skabelon (højre side):</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Indsætter</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Side(r)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>S&amp;kabelon (venstre side):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Skabelon:</translation>
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
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Indsæt tabel</translation>
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
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
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
        <source>&amp;No</source>
        <translation>&amp;Nej</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Ønsker du virkelig at slette denne script?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Shift</source>
        <translation>Skift</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Handling</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Skift+</translation>
    </message>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Indstilling af tastatur genveje</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Vælg en taste for denne handling</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Nuværende taste</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
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
        <source>Alt+</source>
        <translation>Alt+</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation>Ctrl+</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ingen taste</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Brugerdefineret taste</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Aflæs &amp;taste</translation>
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
        <source>This Key Sequence is already in use</source>
        <translation>Denne tastesekvens er allerede i brug</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Slet lag</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Sænk lag</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Hæv lag</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nyt lag</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Tilføj et ny lag</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Ønsker du også at slette alle objekter i dette lag?</translation>
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
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filer (*)</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Tilføj</translation>
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
        <translation>D&amp;uplikër</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
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
        <source>Do you really want to delete this Style?</source>
        <translation>Ønsker du virkelig at slette denne typografi?</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation> pt</translation>
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
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation>Det givne objekt kan ikke kaldes</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation>Håndtér makroer</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation>Frembringer et grafisk vindue for at kunne lave, slette, redigere, gemme og indlæse makroer.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation>Lav, redigér og slet makroer</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation>&amp;Makro</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Macro håndtering</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation>Er ikke i stand til at åbne den efterspurgte fil: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation>Scribus - Redigér makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Det makro navn du vil bruge er allerede brugt til en anden makro&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Det mislykkedes at lave makroen. Makro håndteringen var ikke i stand til at opsætte makroen&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation>Makroen &apos;%1&apos; har reporteret en mindre fejl.
Fejlen er: %2
En fuldstændig tilbagesporing følger:

%3
</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation>Makroen &apos;%1&apos; kunne ikke udføres korrekt.
Fejlen er: %2
En fuldstændig tilbagesporing følger:

%3
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation>Scribus - Ny makro</translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation>Indtast navn på ny makro: </translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Macro håndtering</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation>Makroen kunne ikke omdøbes, da navnet allerede er i brug.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation>Scribus - Håndtér makroer</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Dette vindue er Scribus Makro håndtering. Her kan du lave makroer, redigér makroer, etc. Alle ændrnger laves ved at bruge knapperne på den højre side af vinduet.&lt;/p&gt;
&lt;p&gt;Alle ændringer foretaget i dette vindue har øjeblikkelig virkning - du kan ikke annullere handlinger du laver her.
Tabellen i midten af vinduet viser de makroer, der for øjeblikket er indlæst og nogen information om dem. Brug &quot;Hvad er det&quot; på tabellen for at få mere information.&lt;/p&gt;&lt;/qt&gt;
</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Lav en ny makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Lav en ny makro ved at angive makroens navn og så åbne redigér makro vinduet.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation>Luk dette vindue</translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation>Returnér til Scribus</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation>Makro</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Redigér</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation>Genvej</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beskrivelse</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Denne tabel viser de makroer, der allerede er defineret.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Navn:&lt;/b&gt; Makroens navn, som den er vist i menuen og andre steder i Scribus&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Redigering:&lt;/b&gt; Hvis makroen kan redigeres, står der &quot;Ja&quot; i denne kolonne. Hvis en makro ikke kan redigeres, er den som regel lavet med register_macro kommandoen i en script.&lt;/p&gt;
 &lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; Menu genvejstaster, der er knyttet til makroen. For eksempel CTRL-F8 betyder, at du kan taste Control-F8 i scribus for at køre makroen.&lt;/p&gt;
 &lt;p&gt;&lt;b&gt;Beskrivelse:&lt;/b&gt; Hvis makroen indeholder en &quot;docstring&quot;, en speciel streng i starten af dens defination, som beskriver den. så er den vist her. Hvis beskrivelsen er lang, så er kun begyndelsen vist - brug &quot;Hvad er det&quot; ud for makroen i Makro menuen for at se den fulde beskrivelse.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation>Om&amp;døb</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation>Alt+M</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation>Omdøb den valgte makro.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Omdøb den valgte makro. Du vil blive spurgt om det nye navn.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Redigér...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Redigér kildekoden for den markerede makro, hvis kilden er tilgængelig.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Redigér den markerede makro. &lt;/p&gt;
&lt;p&gt;Hvis knappen ikke er aktiv, så er der enten ikke markeret en makro eller også har makro håndteringen ikke adgang til kildekoden for den markerede makro (i det tilfælde er vist &lt;tt&gt;Nej&lt;/tt&gt; i makroens &lt;tt&gt;Redigering &lt;/tt&gt;kolonne).&lt;/p&gt;
&lt;p&gt;Hvis Scribus ikke har kildekoden, er makroen sansynligvis lavet med en script.&lt;/p&gt;</translation>
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
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Slet den markerede makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Slet den markerede makro. Dette er øjeblikkeligt, det er ikke muligt at genskabe makroen, når den en gang er slettet. Hvis makroen er lavet af en opstarts-makro, vil den være der, næste gang Scribus startes.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation>Sæt &amp;genvej</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Sæt tastatur genvejen for den markerede makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Sæt tastarur genvej (accelerator) taste for den valgte makro. Du vil blive spurgt om den nye genvej i en dialog boks.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation>E&amp;ksport</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation>Alt+X</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation>Eksportér makroer til en fil.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Eksportér makroer til en ekstern fil. Filen vil være en Python script, der indeholder scripter kommandoer, som genskaber makroerne. Den kan køres med &lt;tt&gt;Indlæs udvidelses script&lt;/tt&gt; fra &lt;tt&gt;script&lt;/tt&gt; menuen, eller med import knappen i Makro håndteringen.&lt;/p&gt;
&lt;p&gt;Hvis du ønsker en god, menneske læsbar version af dine makroer, så markér den makro du ønsker, tryk &lt;tt&gt;Rediger&lt;/tt&gt; knappen, og brug &lt;tt&gt;Gem kilde&lt;/tt&gt; knappen i &lt;tt&gt;Rediger makro&lt;/tt&gt; vinduet. Du vil ikke være i stand til at indlæse denne version med &lt;tt&gt;Indlæs udvidelses script&lt;/tt&gt; - lav i stedet for en ny makro med &lt;tt&gt; Ny&lt;/p&gt; knappen og brug &lt;tt&gt;Indlæs kilde&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation>Slet &amp;alle</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation>Slet alle makroer.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Slet alle registrerede makroer. Dette er øjeblikkeligt, og det er ikke muligt at genskabe de slettede makroer, Makroer lavet af din opstarts-makro, vil være der igen, næste gang Scribus startes.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importër</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation>Importér makroer fra en fil.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Indlæser makroer fra en ekstern fil.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation>Scribus makro filer (*.pymacro)</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation>Åbn fil dialogvindue</translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation>Vælg makro filen, der skal indlæses.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation>Gem fil dialogvindue</translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation>Gem alle makroer</translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation>Scribus - omdøb makro</translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation>Indtast nyt navn: </translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation>Scribus - sæt makro genvej</translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation>Indtast ny genvej: </translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nej</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplikér flere gange</translation>
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
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importér</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Vælg...</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation>Importér skabelon</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importér side(r)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Fra dokument:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Importér side(r):</translation>
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
        <source> from 0</source>
        <translation> fra 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Lav side(r)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>efter side</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>i slutningen</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> fra %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>to:</source>
        <translation>til:</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Flyt side(r):</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>i slutningen</translation>
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
        <source>before Page</source>
        <translation>Før side</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>Efter side</translation>
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
    <name>Mpalette</name>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
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
        <translation>Hensigt med gengivelse:</translation>
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
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.
Vælg venligst et andet.</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Venstre punkt</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Afstande</translation>
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
        <translation>Opfattelsesmæssigt</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
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
        <translation>Relativ kolorimetrisk</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolorimetrisk</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Farvemætning:</translation>
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
        <translation>Skalér skrifttegnenes bredde</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Tekstens stregfarve</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Tekstens fyldfarve</translation>
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
        <source>Style of current paragraph</source>
        <translation>Typografi i nuværende afsnit</translation>
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
        <source>Keep the aspect ratio</source>
        <translation>Bevar bredde/højde forholdet</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Billedets kildeprofil</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Hensigt med gengivelsen af billedet</translation>
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
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Får tekst i bagvedliggende objekter til at flyde rundt om dette objekts form</translation>
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
        <source>Text &amp;Flows Around Frame</source>
        <translation>Te&amp;kst flyder uden om ramme</translation>
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
        <source>&amp;Gap:</source>
        <translation>Afsta&amp;nd:</translation>
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
        <source>&amp;Font Size:</source>
        <translation>F&amp;ontstørrelse:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>L&amp;inieafstand:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>Af&amp;snits typografi:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>S&amp;prog:</translation>
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
        <source>Hyphenation language of frame</source>
        <translation>Sprog for Ordeling i rammen</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Brug den omkringliggende firkant i stedet for rammens form til at holde tekst fri af rammen</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Brug en anden linie, der baseret på rammens form, til at holde tekst fri af rammen</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Skrivning fra højre til venstre</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation>Sp&amp;atiering:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuel spatiering</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Flat Cap</source>
        <translation>Flade</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Streg prik prik linie</translation>
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
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.
Vælg venligst et andet.</translation>
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
        <source>Dashed Line</source>
        <translation>Streg linie</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Prik linie</translation>
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
        <source>Dash Dot Line</source>
        <translation>Streg prik linie</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Massiv linie</translation>
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
        <source> pt </source>
        <translation> pt </translation>
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
    <name>MusterSeiten</name>
    <message>
        <source>Name:</source>
        <translation>Navn:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopi af %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Ny skabelon</translation>
    </message>
    <message>
        <source>Edit Templates</source>
        <translation>Rediger skabeloner</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Tilføj</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplikër</translation>
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
        <source>&amp;No</source>
        <translation>&amp;Nej</translation>
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
        <source>Do you really want to delete this Template?</source>
        <translation>Ønsker du virkelig at slette denne skabelon?</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>Points (pts)</source>
        <translation>Punkter (pts)</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nyt dokument</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brugerdefineret</translation>
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
        <source>Inches (in)</source>
        <translation>Tommer (in)</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Kolonne hjælpelinier</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
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
        <source>Enable single or spread based layout</source>
        <translation>Vælg enkelt- eller tosidet layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Vælg venstre side af dokumentet som den første side</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem venstre margen-hjælpelinie og sidens kant.
Hvis der er valgt modstående sider, så kan denne margen bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem højre margen-hjælpelinie og kanten af siden.
Hvis modstående sider er valgt, så kan denne margen afstand bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Første sidenummer i dokumentet</translation>
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
        <source>&amp;Facing Pages</source>
        <translation>Modst&amp;ående sider</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Venstre side &amp;først</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>H&amp;øjre:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>B&amp;und:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Inderside:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Retning:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Første side&amp;nummer:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Standard &amp;enhed:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Yderside:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation>Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Ledger</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation>Venstre side</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Højre side</translation>
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
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Noder</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Sæt kryds for at få coordinater i forhold til siden,
ellers er coordinaterne vist i forhold til objektet</translation>
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
        <translation>&amp;Nulstil omrids linie</translation>
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
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Formindsk størrelsen på stien med den viste %</translation>
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
        <source>% to Enlarge or Reduce By</source>
        <translation>% til at forøge eller formindske med</translation>
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
        <translation>Sammenflet afsnits typografier efter egenskaber. Dette resulterer i færre ens afsnits typografier, vil bevare typografi egenskaber,  selv om det originale dokuments typografier er navngivet forskelligt.</translation>
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
        <source>Update paragraph Styles</source>
        <translation>Opdatër afsnits typografier</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Sammenflet afsnits typografier</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Script konsol</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Box</source>
        <translation>Boks</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Udvisk</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Billeder:</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Adskil</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Opløs</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horisontalt</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Eksport område</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>fil indstillinger</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Højre margen</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Fonte til indlejring:</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Venstre til højre</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Højre til venstre</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Massive farver:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Hensigt med gengivelse:</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Gardiner</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Ingen effekt</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Brug ikke indlejrede ICC-profiler</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Tilgængelige fonte:</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Inderside</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Venstre margen</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Skærm / Net</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Yderside</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>top-venstre til bund-højre</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Printer</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Indlejrer</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Beskær boks</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Opfattelsesmæssigt</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Adgangskode</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Hensigt med PDF/X-3 udskrift</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekter</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Mætningsgrad</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikalt</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Indstillinger</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Glitren</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Bund til top</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>top til bund</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sec</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ kolorimetrisk</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut kolorimetrisk</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Billed-indstillinger</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisk</translation>
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
        <translation>minimum</translation>
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
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Bestemmer PDF kompatibiliteten. Standarden er Acrobat 4.0, som giver den største kompatibilitet.
Vælg Acrobat 5.0, hvis filen har PDF 1.4 egenskaber, som f.eks. transparens eller hvis du ønsker 128 bit kryptering.
PDF/X-3 bruges til eksport til PDF for kommerciel udskrivning og kan vælges, når farvestyring er aktiveret.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Bestemmer sidernes indbinding i PDF. Behold standardværdien - venstre, 
medmindre du ved, at du bøhøver at ændre det.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Laver miniaturer af hver side i PDF-filen.
Nogle PDF-læsere kan bruge disse til navigering.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Lav PDF-indlæg, som er anvendelig for navigering i sammenkædede indlæg i en PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Indlejr bogmærkerne, som du lavede i dit dokument.
Disse er anvendelig for navigering i lange PDF dokumenter.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Opløsning af eksporteret tekst og vektorgrafik.
Dette påvirker ikke opløsningen af bitmap billeder som fotografier.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Komprimering af tekst og grafik.
Medmindre du har grund til andet, så lade dette være aktiveret, Det reducerer størrelsen af PDF-filen.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Komprimerings-metode for billeder.
Automatisk tillader Scribus at vælge den bedste metode.
ZIP er god til billeder med masive farver.
JPEG er bedre til at lave PDF-filerne mindre, når der er mange fotografier (evt. med et lille tab af billedkvalitet).
Vælg automatisk, medmindre du har brug for specielle komprimerings-indstillinger.</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Ændrer bitmap-billedet til valgte DPI.
Hvis dette ikke vælges, så bruges billedets oprindelige opløsning.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Punkter pr tomme) for eksport af billede.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Indlejr fonte i PDF-filen. 
Dette vil bevare opsætning og udseende af dokumentet.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Aktiverer præsentations-effekter, når &apos;Acrobat Reader&apos; bruges i fuldskærm tilstand.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Viser en miniature af hver side, som er vist her ovenover.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Den tid siden er vist, før præsentationen starter på den valgte side.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Den tid effekten varer.
En kortere tid vil øge farten af effekten, en længere tid vil nedsætte tempoet.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Skærm-effektens type.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Retning på bevægelige liner for adskil og gardin effekter.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Start position for boks og adskil effekter.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Retning på gitter eller udvisk effekter.
</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Anvend den valgte effekt på alle sider.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Aktivér sikkerheds-unktionerne i det eksporterede PDF-dokument.
Hvis du valgte Acrobat 4.0, så vil PDF være beskyttet af 40 bit kryptering.
Hvis du valgte Acrobat 5.0, så vil PDF være beskyttet af 128 bit kryptering.
OBS: PDF kryptering er ikke så pålidelig som GPG eller PGP kryptering og har desuden en del begrænsninger.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Vælg en ejer adgangskode, som aktiverer eller deaktiverer alle
sikkerheds-funktioner i det eksporterede PDF-dokument</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Farvemodel for uddata fra PDF-dokumentet.
Vælg Skærm/Web for PDF-filer, som bruges til visning på skærm og til udskrivning på almindeliger Blæk-printere.
Vælg Printer, når der udskrives til en rigtig 4 farvet CMYK-printer.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Indlejr en farve-profil for masive farver</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Farve-profil for masive farver</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Gengivelse-hensigt for masive farver</translation>
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
        <translation>Gengivelses-hensigt for billeder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Uddata-profil for udskrivning. Hvis det er muligt, så få hjælp fra din printer til valg af profil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Obligatorisk streng for PDF/X-3. Skal udfyldes for at overholde 
reglerne for PDF/X-3. Vi anbefaler at bruge titlen på dokumentet. </translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Afstand for beskæring fra toppen af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Afstand for beskæring fra bunden af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Afstand for beskæring fra venstre side af den fysiske side</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Afstand for beskæring fra højre side af den fysiske side</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>Ge&amp;nerelt</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Fonte</translation>
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
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Komprimerings-niveauer: Minimum (25%), Lav (50%), Medium (75%), Høj (85%), Maksimum (95%)</translation>
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
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Tillad kopiering af tekst eller grafik fra PDF-Filen.
Tekst og grafik kan ikke kopieres uden afkryds.</translation>
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
        <source>Rendering Settings</source>
        <translation>Indstillinger for gengivelse</translation>
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
        <source>O&amp;utput to File:</source>
        <translation>&amp;Skriv til fil:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Væ&amp;lg...</translation>
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
        <source>Compatibilit&amp;y:</source>
        <translation>F&amp;ormat:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Indbinding:</translation>
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
        <source>&amp;Resolution:</source>
        <translation>Opløs&amp;ning:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Metode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>Kvalite&amp;t:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>Formindsk billedopl&amp;øsning til:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Indlejre alle fonte</translation>
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
        <source>Show Page Pre&amp;views</source>
        <translation>Vis side&amp;miniaturer</translation>
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
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Anvend effekt p&amp;å alle sider</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Brug kr&amp;yptering</translation>
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
        <source>Output &amp;Intended For:</source>
        <translation>Udskrift &amp;beregnet for:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>Brug br&amp;uger indstillinger for gengivelse</translation>
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
        <source>Use ICC Profile</source>
        <translation>Brug ICC profil</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Far&amp;ve</translation>
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
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF filer (*.pdf);;Alle filer (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Dette er en anvanceret indstilling, som ikke er aktiv som standard. Denne bør kun aktiveres, når 
trykkeriet specifikt har anmodet om det og opgivet de nøjagtige detaljer om indstillingerne. Ellers vil 
den eksporterede PDF fil ikke kunne udskrives ordentligt og den er ikke portabel på tværs af systemer.</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Komprimer tekst &amp; vektorgrafik</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>Anvend pr&amp;æsentationseffekter</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Præsentation</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>S&amp;ubstituere alle fonte</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Fonte til substituering:</translation>
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
        <source>Save as PDF</source>
        <translation>Gem som PDF</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Farvestyring skal være aktiveret for at bruge PDF/X-3. Du kan aktivere farvestyring fra menuen &apos;Instillinger&apos;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF/X-3 er understøttet og aktiveret, men kan ikke bruges i den valgte PDF version. Hvis du ønsker at bruge PDF/X-3, så skal du vælge PDF/X-3 som PDF version (kompatibilitets niveau).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF/X-3 er ikke understøttet i denne Scribus kompilering (CMS understøttelse ikke tilstede).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation>Tillad at kommentarer og felter kan tilføjes PDF-Filen.
Redigering af kommentarer og felter er ikke muligt uden afkryds.</translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation>PDF sikkerheds indstillinger</translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation>PDF/X-3 indstillinger</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF sikkerhed kan ikke bruges sammen med PDF/X-3. Hvis du ønsker at aktivere sikkerhed, så vælg en anden PDF version (kompatibilitets niveau) end PDF/X-3.&lt;/qt&gt;</translation>
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
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Giver en mere behagelig forhåndsvisning af tekst-objekter, men der
går lidt længere tid med at vise siden. Det gælder kun type 1 fonte</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Viser transparens og gennemsigtige objekter i dit dokument. Ghostscript 7.07 eller nyere skal være installeret</translation>
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
        <source>Anti-alias &amp;Text</source>
        <translation>Anti-alias &amp;Tekst</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Anti-alias &amp;Grafik</translation>
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
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En måde at ændre nogle af gråtonerne, som er sammensat
af cyan, gul og magenta, så der i stedet for disse farver bruges sort.
UCR påvirker mest de dele af billedet, som er neutrale og/eller mørke i farven, det, 
som er tættets på gråt. Brug af denne metode kan forbedre udskrivningen af nogle 
billeder og nogen eksperimenteren og testning er nødvendigt fra billede til billede.
UCR formindsker muligheden for overmætning med CMY farverne.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Giver en mere behagelig forhåndsvisning af &apos;TrueType&apos; og &apos;OpenType&apos; fonte, 
EPS, PDF og vektorgrafik, på bekostning af lidt længere tid med at vise siden</translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Ikke aktiv</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fil: </translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Tekstramme</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Udskriv: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Tegn: </translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Tekst på en sti</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Sammenkædet tekst</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Aktiv</translation>
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
        <source>Edit Text...</source>
        <translation>Redigér tekst...</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Programmet</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>mangler!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
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
        <translation>Vis &amp;basislinie-gitter</translation>
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
        <source>Original PPI: </source>
        <translation>Original PPI: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Faktiske PPI: </translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;fo</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>&amp;Hent billede...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Billede er &amp;synlig</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>&amp;Opdatér billede</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Redigér billede</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>&amp;Justér ramme efter billede</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Hent tekst...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Tilføj tekst...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>&amp;Redigér tekst...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>&amp;Indsæt prøvetekst</translation>
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
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF Indstillinger</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Lås</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Lås &amp;op</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Lås objekt-&amp;størrelse</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Frigør objekt-&amp;størrelse</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Placér i s&amp;crapbog</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Placér i la&amp;g</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>Lav til &amp;gruppe</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Bryd &amp;gruppe</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;veau</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Placér &amp;bagest</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
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
        <source>&amp;Picture Frame</source>
        <translation>&amp;Billedramme</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Pol&amp;ygon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Figurer</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Tekstramme</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier-kurve</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Konve&amp;rtér til</translation>
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
        <source>&amp;Delete</source>
        <translation>&amp;Slet</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>S&amp;let indhold</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Vis ege&amp;nskaber...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Skjul ege&amp;nskaber...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Ønsker du virkelig at slette al din tekst?</translation>
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
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Side </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation> af %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Result</source>
        <translation>Resultat</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Vælg</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Søg resultater for: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vis</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Gå til</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Navn</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Sti</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Udskriv</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Mangler</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Søg</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Pictures</source>
        <translation>Billeder</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygon egenskaber</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Hjørn&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
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
        <source>Number of corners for polygons</source>
        <translation>Antal hjørner i polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Rotation i grader for polygoner</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Prøve polygon</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Anvend &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Anvens convex/concav faktor for at ændre polygonens form</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>En negativ værdi vil lave polygonen concav (eller stjerneformet),
en positiv værdi vil lave den convex</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>None</source>
        <translation>Ingen</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menuer</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Stier</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andet</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Lille</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Værktøjer</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Enheder</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Andre indstillinger</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Små kapitæler</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brugerdefineret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Landskab</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hjælpelinier</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Farve på gitter og hjælpelinier</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Gitter indstillinger</translation>
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
        <source>Portrait</source>
        <translation>Portræt</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Placering</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margen hjælpelinier</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Vævet silkepyjamas byttet med blå kvarts</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbog</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Indstil hvor meget 100% visning fylder på skærmen</translation>
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
        <source>Points (pt)</source>
        <translation>Punkter (pt)</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Vis</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Tommer (in)</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Gem automatisk</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Præferencer</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Generelt</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
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
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Medium</translation>
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
        <source>External Tools</source>
        <translation>Eksterne værktøjer</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Postscript-fortolker</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Program til Billedbehandling</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Diverse</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Udskrivning</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Vælg standard-vinduets udseende.
Scribus kan bruge alle tilgængelige KDE og Qt temaer</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Standard fontstørrelse for menuer og vinduer</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard målenhed for dokument-redigering</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Antal linier der rulles for hver bevægelse af muse-hjulet</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Størrelsen af det område, hvor du kan tage om et objekts håndtag</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Antal nyeste dokumenter, som vises i fil-menuen</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standard dokumentmappe</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standard Scripter scriptmappe</translation>
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
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Bredde af dokument-sider, redigérbar hvis du har valgt en brugerdefineret sidestørrelse</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Højde af dokument-sider, redigérbar hvis du har valgt en brugerdefineret sidestørrelse</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Vælg enkelt- eller tosidet layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Vælg den venstre side af dokumentet som den første side</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem venstre margen-hjælpelinie og sidens kant.
Hvis der er valgt modstående sider, så kan denne margen bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Når dette er valgt, så gemmer Scribus en backup-kopi af din fil med .bak tilføjet filnavnet
hver gang det valgte tids-interval udløber</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Tid-interval for den automatiske gemning</translation>
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
        <translation>Den afstand indenfor hvor et objekt vil placere sig op af en hjælpelinie</translation>
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
        <source>Place the grid behind your page objects</source>
        <translation>Placér gitteret bag ved objekterne på siden</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Placér gitteret foran objekterne på siden</translation>
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
        <source>Number of corners for polygons</source>
        <translation>Antal hjørner i polygoner</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Rotation i grader for polygoner</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Prøve polygon</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Vælg størrelse på ikoner af scrapbogen indhold</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Når modstående sider anvendes, vis da de to sider ved siden af hinanden</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Farve på papir</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farve på margenlinier</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Vis området uden for margener i margenfarve</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Muliggør transparent egenskaber i PDF 1.4 eksport</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Vælg standard zoom-niveau</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Antialias tekst for EPS og PDF skærmvisning</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Antialias grafik for EPS og PDF skærmvisning</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Vis ikke objekter udenfor margener på den udskrevne side eller i den eksporterede fil</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem højre margen-hjælpelinie og kanten af siden.
Hvis modstående sider er valgt, så kan denne margen afstand bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Gem scrapbogens indhold hver gang efter en ændring</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Basislinie-gitter</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Fontstørrelse:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Mus indstillinger</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Hjul &amp;hop:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>&amp;Gribe radius:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Nyeste dokumenter:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumenter:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Vælg...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-profiler:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>V&amp;ælg...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Væ&amp;lg...</translation>
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
        <translation>&amp;Højde:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Modstående sider</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Venstre &amp;side først</translation>
    </message>
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
        <source>&amp;Enabled</source>
        <translation>A&amp;ktiv</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>I&amp;nterval:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>Afstand mellem l&amp;inier:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Afstand mellem ho&amp;vedlinier:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>&apos;Klæb til&apos; &amp;afstand:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>&amp;Gitterlinier:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>&amp;Hoved-gitterlinier:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Hj&amp;ælpelinier:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>&amp;Basislinie-gitter:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>I &amp;baggrunden</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>I &amp;forgrunden</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>&amp;Til</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>&amp;Fra</translation>
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
        <source>D&amp;isplacement:</source>
        <translation>F&amp;orskydning:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>S&amp;kalering:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Sk&amp;alering:</translation>
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
        <source>Automatic &amp;Line Spacing:</source>
        <translation>A&amp;utomatisk linieafstand:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>Standard &amp;font:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Standard &amp;størrelse:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>&amp;Tekstfarve:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Kolonner:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Afstan&amp;d:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Liniefarve:</translation>
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
        <source>&amp;Type of Line:</source>
        <translation>&amp;Typen af linie:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linie&amp;bredde:</translation>
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
        <source>Corn&amp;ers:</source>
        <translation>Hjørn&amp;er:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Gem indhold &amp;ved ændringer</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Stor</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Vis sider ved &amp;siden af hinanden</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Sidefarver</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Baggrund:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Margener:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Vis område, der ikke kan &amp;udskrives i margenfarve</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Brug PDF-1.4 &amp;transparent egenskaber</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Indstil størrelse af skærmvisning</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Navn på program:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Antialias &amp;Tekst</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Antialias &amp;Grafik</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Navn på &amp;program:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Afskær ved &amp;margener</translation>
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
        <source>&amp;Inside:</source>
        <translation>&amp;Inderside:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Yderside:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Anvend Reduktion af &amp;underliggende farve</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>S&amp;kabeloner:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Væ&amp;lg...</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Ekstra mappe for dokument skabeloner</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Anvend &amp;faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Anvend convex/concav faktor for at ændre polygonens form</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>En negativ værdi vil lave polygonen concav (eller stjerneformet),
en positiv værdi vil lave den convex</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>En måde at fjerne nogle af gråtonerne, som er sammensat
af cyan, gul og magenta, så der i stedet for disse farver bruges sort.
UCR påvirker mest de dele af billedet, som er neutrale og/eller mørke i farven, det, 
som er tættets på gråt. Brug af denne metode kan forbedre udskrivningen af nogle 
billeder og nogen eksperimenteren og testning er nødvendigt fra tilfælde til tilfælde.
UCR formindsker muligheden for overmætning af CMY farverne.</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation>Executive</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Ledger</translation>
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
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation>Placering af grafikprogram i filsystemet. Hvis du bruger gimp og 
din distribution inkluderer &apos;gimp-remote&apos;, så anbefaler vi, at du bruger 
denne, da det tillader dig at redigere billedet i en allerede kørende gimp.</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation>Standard ICC-profilmappe. Denne kan ikke 
ændres, når der er åbnede dokumenter.</translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation>Viser basislinie-gitter</translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation>Skjuler basislinie-gitter</translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation>Placering af Ghostscript fortolkeren i filsystemet</translation>
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
        <translation>&amp;Definér brugerfarver &gt;&gt;</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Vælg farve</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Tilføj til brugerfarver</translation>
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
        <translation>&amp;Brugerfarver</translation>
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
        <source>&amp;No</source>
        <translation>&amp;Nej</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Mappe</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
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
        <translation>Attributter</translation>
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
        <translation>Slet</translation>
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
        <source>No</source>
        <translation>Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
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
        <source>Document</source>
        <translation>Dokument</translation>
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
        <source>Online Reference</source>
        <translation>Scripter manual</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-billeder (*.svg *.svgz);;Alle filer (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-billeder (*.svg);;Alle filer (*)</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Filen eksisterer. Overskriv?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>eksisterer allerede. Overskriv?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Ja alle</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Fejl ved udskrivning af uddata-fil(er).</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Gem som billede</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Fejl ved skrivning af uddata-fil(er).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Eksporteret uden problemer.</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>&amp;Fontvisning</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Indsæt specialtegn</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Ny &amp;fra skabelon...</translation>
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
        <translation>Skilte</translation>
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
        <translation>Tryksager</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Egne skabeloner</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Gem som &amp;billede...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>&amp;Vis udskrift</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importér &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alle understøttede formater (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filer (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Gem som skabel&amp;on...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>S&amp;cripter Manual...</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Gem side som &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importér &amp;SVG...</translation>
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
        <source>Print Preview</source>
        <translation>Vis udskrift</translation>
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
        <source>Template: </source>
        <translation>Skabelon: </translation>
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
        <source>Brazilian</source>
        <translation>Brasiliansk</translation>
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
        <source>Norwegian (Bokmaal)</source>
        <translation>Norsk (Bokmaal)</translation>
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
        <source>Tried to set progress &gt; maximum progress</source>
        <translation>Prøvede at sætte fremgang &gt; maksimal fremgang</translation>
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
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation>Åbn dokument fejlede</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation>Gem dokument fejlede</translation>
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
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation>Der findes allerede et objekt med samme navn</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation>Punkt liste skal indeholde mindst to punkter (fire værdier)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation>Punkt liste skal indeholde et lige antal værdier</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation>Punkt liste skal indeholde mindst tre punkter (seks værdier)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation>Punkt liste skal indeholde mindst fire punkter (otte værdier)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation>Punkt liste skal indeholde et multible af seks værdier</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation>Objekt ikke fundet</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation>Typografi ikke fundet</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation>Kan ikke sætte typografi på andet end tekst-ramme</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation>Gem EPS fejlede</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation>Sidenummer er uden for tilladt område</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation>Argument er ikke en liste: skal være en liste af komma-tal</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation>Argument indeholder andet end numeriske værdier: skal være en liste af komma-tal</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation>Linie-bredde uden for tilladt område, skal være 0 &lt;= linie_bredde &lt;= 12</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Linie-farvemætning uden for tilladt område, skal være 0 &lt;= farvemætning &lt;= 100</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Fyld-farvemætning uden for tilladt område, skal være 0 &lt;= farvemætning &lt;= 100</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Hjørne-radius skal være et positivt number.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation>Linie-stil ikke fundet</translation>
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
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importér &amp;OpenOffice.org Tegning...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Tegning (*.sxd);;All Files (*)</translation>
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
- Mange funktioner vil rejse en NoDocOpenError hvis du prøver at bruge dem uden at der er et dokument at operere på.
- Hvis du ikke giver et ramme-navn til en funktion, der behøver det, så vil funktionen bruge den aktive ramme, hvis der er nogen, eller
rejse en NoValidObjectError hvis den ikke kan finde noget at operere på.
- Mange funktioner vil rejse en WrongFrameTypeError hvis du prøver at bruge dem
på en rammetype, hvor det ikke giver mening at bruge dem. For eksempel, sætte
tekstfarve på en billedramme giver ikke mening, og vil resultere
i at denne indsigelse rejses.
- Fejl der kommer fra kald til de underliggende Python API vil blive
sendt videre uændret. Som sådan, er listen af indsigelser, der rejses af
en funktion som nævnt her og i dens docstreng, ikke komplet.

Detaljer om hvilke indsigelser hver funktion kan rejse er givet i 
funktionens dokumentation. </translation>
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
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Korte ord...</translation>
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
        <translation>&apos;allTypes&apos; egenskab er READ-ONL</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Eksport af billede mislykkedes</translation>
    </message>
    <message>
        <source>Cannot get a colour with an empty name.</source>
        <comment>python error</comment>
        <translation>Kan ikke hente en farve med et tomt navn.</translation>
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
        <source>Unable to save pixmap.</source>
        <comment>scripter error</comment>
        <translation>Ikke i stand til at gemme pixmap.</translation>
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
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Mål-rammen skal være tom.</translation>
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
        <source>Object is not a linked text frame, cannot unlink.</source>
        <comment>python error</comment>
        <translation>Objekt er ikke en sammkædet tekst-ramme, kan ikke bryde lænke.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, cannot unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objektet er den sidste i en serie. Bryd i stedet for lænken i den foregående ramme.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Kan ikke konvertere til omrids i andet end tekst-ramme.</translation>
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
        <source>Portuguese (Brazilian)</source>
        <translation>Portugisisk (Brasiliansk)</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation>Farve ikke fundet</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisk</translation>
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
        <translation>Slet</translation>
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
    <name>Query</name>
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
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Dokument indstillinger</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margen hjælpelinier</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Vælg enkelt- eller tosidet layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Vælg venstre side af dokumentet som den første side</translation>
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
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem venstre margen-hjælpelinie og sidens kant.
Hvis der er valgt modstående sider, så kan denne margen bruges til at opnå de rigtige margener for indbinding</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Afstand mellem højre margen-hjælpelinie og kanten af siden.
Hvis modstående sider er valgt, så kan denne margen afstand bruges til at opnå de rigtige margener for indbinding</translation>
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
        <source>&amp;Top:</source>
        <translation>&amp;Top:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Venstre:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Bund:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Højre:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Modstående sider</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Venstre &amp;side først</translation>
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
        <source>&amp;Inside:</source>
        <translation>&amp;Inderside:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>&amp;Yderside:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Sidestørrelse</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Størrelse:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Brugerdefineret</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation>Retning:</translation>
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
        <source>Width:</source>
        <translation>Bredde:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Højde:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>&amp;Første sidenummer:</translation>
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
        <source>None</source>
        <translation>Ingen</translation>
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
        <source>None</source>
        <translation>Ingen</translation>
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
        <translation>Skalér skrifttegnenes bredde</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Skrifttegn indstillinger</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation>Spatiering:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manuel spatiering</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopi #%1 af </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Baggrund</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Klip</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Ny</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopiér</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fil</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Font</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Lås</translation>
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
        <source>Quit</source>
        <translation>Afslut</translation>
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
        <source>Exiting now</source>
        <translation>Afslutter nu</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Fortryd</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Justér mod højre</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Gennemstreg</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Centrér</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Typografier...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Fordel/Opstil på linie...</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Slet</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Luk</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farve</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tjekkisk</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Hollandsk</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Græsk</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Lav til gruppe</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irsk</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Flyt bagud</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Indsæt</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Flyt forud</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Klar</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Farvemætning</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Værktøjer</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Placér bagerst</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Følgende programmer mangler:</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiensk</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Klæb til gitter</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Initialiserer Plugins</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Bryd gruppe</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Smart bindestreg</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS-filer (*.eps);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Slet...</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Små kapitæler</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplikér flere gange</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Vis Arrangér sider</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Farver...</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Miniaturer</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Leder efter fonte</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Dokument Info...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Flyt...</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Skjul rammer</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dansk</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Slet</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Ikke brydbar mellemrum</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Skjul billeder</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungarsk</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Fransk</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Tysk</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Lås/Lås op</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertér</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Vis lag</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polsk</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Indlæser:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovakisk</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Lås op</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Fonte...</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Åbn...</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Vis gitter</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Oversigt</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Egenskaber</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Duplikér</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Skjul margener</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Gem som...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Hent billede...</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Forbind tekst til sti</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Indsæt sidenummer</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Placér forrest</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Tilpasser farver</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Håndtér hjælpelinier</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukrainsk</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Scrapbog</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Læser scrapbog</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Dokument indstillinger...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Tilpas til vindue</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Udskriv...</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Om Qt</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Hent Tekst/billede...</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalansk</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gem som</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Anvend skabelon...</translation>
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
        <source>Insert...</source>
        <translation>Indsæt...</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisk</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Udskrivning fejlede!</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Orddel tekst</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spansk</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus nedbrud p.g.a Signal #%1</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Hent tekst...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Udskrivning...</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Understreg</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle filer (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Indlæser...</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Tekstfiler (*.txt);;Alle filer (*)</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Vælg en mappe</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Scribus manual...</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Markér alt</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugisisk</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Håndtér billeder</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Engelsk</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Indrammet</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus nedbrud</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Skabeloner...</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Gemmer...</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finsk</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Læser præferencer</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Laver genveje</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fatal fejl</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Justér mod venstre</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Vælg ny font</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Om Scribus</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Init Orddeler</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Kan ikke gemme filen: 
%1</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus manual</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Javascripts...</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>50%</source>
        <translation>50%</translation>
    </message>
    <message>
        <source>75%</source>
        <translation>75%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
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
        <source>All Supported Formats</source>
        <translation>Alle understøttede formater</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Lav et nyt dokument</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Åbn et dokument</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Gem det aktive dokument</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Luk det aktive dokument</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Udskriv det aktive dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Gem det aktive dokument som PDF</translation>
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
        <source>&amp;View</source>
        <translation>Vi&amp;s</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Værktøjer</translation>
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
        <source>Show Baseline Grid</source>
        <translation>Vis basislinie-gitter</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>Si&amp;de</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Skjul basislinie-gitter</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Nogle objekter er låst.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Annullér</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Lås alt</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Frigør alt</translation>
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
        <source>Lithuanian</source>
        <translation>Litaurisk</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Svensk</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovensk</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Indstillinger</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>F&amp;arvestyring...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Åbn...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Åbn n&amp;yeste</translation>
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
        <source>&amp;Get Text/Picture...</source>
        <translation>&amp;Hent tekst/billede...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Tilføj &amp;tekst...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importër</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Gem &amp;tekst...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Gem side som &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Gem som P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportër</translation>
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
        <translation>S&amp;let</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Markér &amp;alt</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Søg/erstat...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Far&amp;ver...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Afsnits typografier...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Liniestile...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>Ska&amp;beloner...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Javascripts...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplikér</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Duplikér flere &amp;gange</translation>
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
        <source>&amp;Lock</source>
        <translation>&amp;Lås</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Placér &amp;bagest</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
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
        <source>Distribute/&amp;Align...</source>
        <translation>Spred/&amp;Opstil på linie...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>For&amp;m</translation>
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
        <translation>&amp;Adskil polygoner</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>&amp;Konvertér til figurer</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Indsæt...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Slet...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Flyt...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Anvend skabelon...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>Tilpas til &amp;vindue</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>Miniat&amp;urer</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Vis &amp;gitter</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Kl&amp;æb til hjælpelinier</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Vis b&amp;asislinie-gitter</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Egenskaber</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Oversigt</translation>
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
        <source>P&amp;age Palette</source>
        <translation>&amp;Arrangër sider</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Bogmærker</translation>
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
        <source>Toolti&amp;ps</source>
        <translation>Hj&amp;ælpe-tekst</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF-værktøjer</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Hjælpe-tekst</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>&amp;Fonte...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Orddeling...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>&amp;Tastaturgenveje...</translation>
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
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus &amp;Manual...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>S&amp;til</translation>
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
        <source>&amp;Other...</source>
        <translation>&amp;Andet...</translation>
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
        <source>&amp;Color</source>
        <translation>Far&amp;ve</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Bytte om på</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Hent tekst...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Font</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Størrelse</translation>
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
        <translation>Farve&amp;mætning</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatorer...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Lås &amp;op</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Vis &amp;billeder</translation>
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
        <source>Show G&amp;uides</source>
        <translation>Vis &amp;hjælpelinier</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript : Du kan ikke bruge EPS-billeder</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Importér &amp;Side(r)...</translation>
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
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Du prøver at importere flere sider end der er tilgængelig i det aktuelle dokument talt fra fra den aktive side.&lt;/p&gt;Vælg et af følgende:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Lav&lt;/b&gt; manglende sider&lt;/li&gt;&lt;b&gt;Importér&lt;/b&gt; sider indtil den sidste side&lt;/li&gt;&lt;li&gt;&lt;b&gt;Annullér&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Lav</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importér</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importering er færdig</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Fandt ingen ting at importere</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>&amp;Klæb til gitter</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Præferencer...</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Henter ICC Profiler</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Håndtër &amp;hjælpelinier...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Størrelse:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Farvemætning:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>Do&amp;kument information...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>&amp;Fortryd sletning af objekt</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>&amp;Fortryd flytning af objekt</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>&amp;Fortryd ændring af objekt</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Redigér form</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>Filen %1 er ikke i Scribus format</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Font-system initialiseret</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugisisk (BR)</translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation>Der er ingen brugbare fonte installeret på dit system</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Farvestyring er understøttet, men kan ikke aktiveres. Sørg for, at du har installeret de nødvendige ICC farveprofiler, og at stien til ICC-profiler i præferencer peger til det sted, hvor de er installeret.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Farvestyring er ikke understøttet i denne udgave af Scribus (ikke  kompileret ind).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisk</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Lag</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
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
        <source>Copy of</source>
        <translation>Kopi af</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>&amp;Afslut alligevel</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>&amp;Luk alligevel</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Gem nu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus scripts</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Kør Script...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation>Kør en Python script fra en fil.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation>&amp;Indlæs udvidelses Script...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation>Indlæs en Python script som en udvidelse. Bruges til at indlæse makroer og avancerede Python scripts, som udvider Scribus brugerflade.</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Nyeste scripts</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Vis konsol</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation>Vis en interaktiv Python konsol, hvor du kan skrive og køre Python programmer, der bruger Scripter værktøjerne.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;Om script...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation>Scri&amp;pter indstillinger</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python scripts (*.py);; Alle filer (*)</translation>
    </message>
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
        <source>Scribus - Script Plugin</source>
        <translation>Scribus - Script Plugin</translation>
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
        <translation>Script plugin funktionen &apos;Indlæs script&apos; er ikke aktiveret.
Hvis du bare ønsker at køre en normal script, så skal du 
sansynligvis bruge &apos;Kør script...&apos; i stedet for.

Hvis du faktisk ønsker at indlæse en Python udvidelses script eller 
makro, så skal du gå ind i Scripter indstillinger i Script menuen og 
der aktivere scripter udvidelser.

Læs venligst først dokumentationen om udvidelses scripts.
</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>Skj&amp;ul konsol</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Om script</translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation>Scribus - Scripter Præferencer</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation>Aktivér Scripter udvidelser</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation>Aktiver udvidelses scripts og makroer</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Aktivering af scripter udvidelser muliggør yderligere scripter funktionalitet, indeholdende Python makroer og muligheden for en Python opstarts script. Aktivering af dette tilvalg, åbner for &lt;tt&gt;Indlæs udvidelses script&lt;/tt&gt; indgangen i Script menuen.&lt;/p&gt;
&lt;p&gt;
Kun scripts, som er skrevet til at blive kørt som udvidelses scripts, bør kaldes med &lt;tt&gt;Indlæs udvidelses script&lt;/tt&gt; eller bruges som opstarts script. Se scripter dokumentationen for flere detaljer.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation>Brug en opstarts script</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Hvis &lt;tt&gt;Brug en opstarts script&lt;/tt&gt; er valgt, vil Scribus indlæse script-filen, specificeret her, som en udvidelses script ved opstart. Det er vigtigt, at scripten er skrevet som en udvidelses script, den kan forårsage problemer, hvis den ikke er omhyggeligt skrevet.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Brug en opstarts script&lt;/tt&gt; vil være slået fra, hvis scripter udvidelser er slået fra, da udvidelses scripts ikke kan indlæses, hvis ikke scripter udvidelser er aktiveret.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Gennemse...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation>Gennemse for en ny script fil</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Gennemse for en ny script fil&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation>Hvilken script-fil skal indlæses ved opstart</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Filen med Python scripten, der skal køres som en udvidelses script ved opstart.&lt;/p&gt;
&lt;/p&gt;Vær opmærksom på, at når denne script køres, er Scribus ikke helt startet og arbejdsområdet eksisterer endnu ikke.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation>Script Fil:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation>Luk uden at gemme ændringer</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation>Gem ændringer og luk</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Avancerede indstillinger</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation>Importér alle navne ved opstart</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation>Udfør &apos;from scribus import *&apos; i script-konsolen ved opstart</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Importér alle navne verd opstart&lt;/tt&gt; er et avanceret tilvalg. Du bør sansynligvis lade det være valgt, medmindre du har læst dokumentationen og ved hvad du gør.&lt;/p&gt;
&lt;p&gt;Er det Valgt fra, forhindres scripter i at køre den sædvanlige &lt;tt&gt;from scribus import *&lt;/tt&gt; kommando, når den initialiserer hoved-fortolkeren (brugt af script-konsolen og udvidelses -scripts) ved opstart&lt;/p&gt;
&lt;p&gt;dette tilvalg træder ikke i funktion før Scribus er genstartet.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation>Aktiver forældet navne alias</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Aktivér brugen af GammelStil funktions-navne&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Aktivér forældet navne alias&lt;/tt&gt; er et avanceret tilvalg. Du bør sansynligvis lade det være, som det er.&lt;/p&gt;
&lt;p&gt;Dette tilvalg vil det forårsage, at scripter laver et stort antal funktion og konstant alias for 1.2.0 script kompatibilitet.
Tilvalgt er standard.&lt;/p&gt;
&lt;p&gt;dette tilvalg træder ikke i funktion før Scribus er genstartet.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation>Brug Erstatnings Stdin</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Erstat sys.stdin med en erstatnings-fil, for at forhindre Scribus i at hænge, når en script prøver at læse fra stdin.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Brug Erstatnings stdin&lt;/tt&gt; er et avanceret tilvalg.  Du bør sansynligvis lade det være, som det er.&lt;/p&gt;
&lt;p&gt;Normalt vil Scribus give Python et erstatnings-fil objekt for &lt;tt&gt;sys.stdin&lt;/tt&gt;, så læsning fra stdin altid returnerer en tom streng.
Hvis den virkelige &lt;tt&gt;sys.stdin&lt;/tt&gt; er på sin plads, så vil scripts, der prøver at læse fra den blokere - og derved blokere Scribus i at køre, så programmet ser ud til at hænge - indtil data ankommer til stdin. Det er usædvanligt, at GUI programmer forventer noget som helst fra stdin, så de fleste brugere vil tænke, at Scribus er brudt sammen.&lt;/p&gt; 
&lt;p&gt;Du kan deaktivere dette tilvalg, hvis du ønsker at acceptere indput fra stdin. Generelt bør du bruge &lt;tt&gt;os.popen&lt;/tt&gt; til at lave en kanal i stedet, eller brug en anden indput makanisme, men dette tilvalg er her for alle tilfældes skyld.&lt;/p&gt;&lt;/qt&gt;</translation>
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
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Vis skabelonnavne</translation>
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
        <source>None</source>
        <translation>Ingen</translation>
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
        <source>OK</source>
        <translation>OK</translation>
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
        <translation>S&amp;let</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Her er alle dine skabeloner. Træk en skabelon
til vinduet nedenunder, for at lave en ny side.</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Tilgængelige skabeloner:</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Venstre side først</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Træk sider eller skabeloner til skraldespanden for at slette dem.</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Modstående sider</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokument sider:</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Arrangér sider</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Vis alle sider i dokumentet.</translation>
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
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Ingen typografi</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>No</source>
        <translation>Nej</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Åbn</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.sla.gz *.scd *scd.gz);; Alle filer (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumenter (*.sla *.scd);; Alle filer (*)</translation>
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
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Tilføj</translation>
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
        <source>&amp;Save</source>
        <translation>&amp;Gem</translation>
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
        <source>Do you really want to delete this Style?</source>
        <translation>Ønsker du virkelig at slette denne typografi?</translation>
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
        <source>Do you really want to lose all your Changes?</source>
        <translation>Ønsker du virkelig at miste alle ændringer?</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Redigering af tekst</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Ønsker du virkelig at slette al din tekst?</translation>
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
        <source>Update Text Frame</source>
        <translation>Opdatér tekstramme</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
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
        <source>Update Text Frame and Exit</source>
        <translation>Opdatér tekstramme og afslut</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Afslut uden at opdatere tekstramme</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Ny</translation>
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
        <translation>S&amp;let</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>Indsæt spe&amp;cialtegn...</translation>
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
        <source>Load Text from File</source>
        <translation>Indlæs tekst fra fil</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Gem tekst i fil</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Genindlæs tekst fra ramme</translation>
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
        <translation>&amp;Indstillinger</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Søg/erstat</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Fontvisning...</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Slet al tekst</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Smart tekst markering</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Outline Text</source>
        <translation>Omrids</translation>
    </message>
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
        <source>Underline</source>
        <translation>Understreget</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Erstat standardværdier med bruger config</translation>
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
        <translation>Sammenflet afsnits typografier efter egenskaber. Dette resulterer i færre ens afsnits typografier, vil bevare typografi egenskaber,  selv om det originale dokuments typografier er navngivet forskelligt.</translation>
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
        <source>Update Paragraph Styles</source>
        <translation>Opdatër afsnits typografier</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Håndtér tabulatorer</translation>
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
        <source>&amp;Position:</source>
        <translation>&amp;Position:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>Første &amp;linie:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation>Venstre indr&amp;ykning:</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Line</source>
        <translation>Linie</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Side</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Font:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Billede</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Poly-linie</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Oversigt</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Sti-tekst</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
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
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Advarsel</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Navnet &quot;%1&quot; er ikke entydigt.
Vælg venligst et andet.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Lav til gruppe </translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
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
    <name>VlnaDialog</name>
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
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation>&amp;Annullér</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation>&amp;Info og
Sprog</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation>Erstat standardværdier med bruger opsætning</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation>Når bruger opsætnings-filen findes
(%1)
kan du vælge, om du ønsker at tilføje din opsætning
til den globale opsætning ved en ikke afkrydset knap.

Du kan også erstatte predefinerede værdier med 
dine med afkrydset knap.</translation>
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
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation>Korte ord for Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation>Tilgængelig i følgende sprog</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation>Om Korte Ord</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Værktøjer</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Indsæt linier</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Indsæt billede</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Markér objekter</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Redigér indhold af ramme</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Sammenkæd tekstrammer</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Indsæt bezier kurver</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotér objekt</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Bryd sammenkædning af tekstrammer</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Indsæt frihånds-linie</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Egenskaber...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Indsæt polygoner</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Indsæt tekstramme</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zoom ind eller ud</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Redigér teksten med den indbyggede tekstbehandler</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Tegn forskellige figurer</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Indsæt tabel</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Udfør opmålinger</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Link</source>
        <translation>Lænke</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Liste boks</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombo boks</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Knap</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Tekstfelt</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Afkrydsnings boks</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF Værktøjer</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Indsæt PDF Felter</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Indsæt PDF Kommentar</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Vælg tegn:</translation>
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
        <source>Close this dialog and return to text editing.</source>
        <translation>Luk dette vindue og returnér til tekst-redigering.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Indsæt</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>S&amp;let</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Luk</translation>
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
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Husker fil-endelsen - importfilter-tilknytning
og spørger ikke igen om at vælge et importfilter
for denne filtype.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Ny fra skabelon</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Annullér</translation>
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
        <translation>Når du fjerner en skabelon fra &apos;Ny Fra skabelon&apos; vinduet slettes kun linien fra template.xml. Dokumentfilerne bliver ikke slettet. Pop op menuen vil kun vise &quot;Fjern&quot; hvis du har skrive rettigheder til template.xml filen.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopiér en eksisterende template.xml til en fil med navnet template.lang_COUNTRY.xml (brug den samme kode, som bruges for qm filen for dit sprog), f.eks. template.fi.xml for den finske template.xml. Kopien skal være placeret i den samme mappe, som den originale template.xml så Scribus kan indlæse den.</translation>
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
        <translation>S&amp;let</translation>
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
