<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;izena&quot;])

&quot;izena&quot; objektua desplazatzen du dx eta dy balioekin uneko posiziotik. Dokumentuan 
uneko neurtzeko unitateetan adierazten dira distantziak (ikus UNIT konstanteak). 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
&quot;izena&quot; objektua talde batekoa bada, talde osoa desplazatuko da.
</translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="unfinished">getColorNames() -&gt; zerrenda

Dokumentuan definitutako kolore guztien izenen zerrenda itzultzen du. 
Dokumenturik ez balego irekita, dokumentu lehenetsiaren koloreen zerrenda 
itzuliko luke.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation type="unfinished">newDocDialog() -&gt; boolearra

&quot;Dokumentu berria&quot; elkarrizketa-koadroa bistaratzen du. Dokumentu berria sortuko du erabiltzaileak ezarpenak onartuz gero. Ez du dokumenturik sortuko 
erabiltzaileak bertan behera uzteko botoia sakatzen badu. EGIA itzultzen du 
dokumentu bat sortzean.</translation>
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
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished">getFillColor([&quot;izena&quot;]) -&gt; katea

&quot;izena&quot; objektuaren kolore-betegarriaren izena itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation type="unfinished">setRedraw(boolearra)

Boolearraren balioa FALSE (FALTSUA) denean orria birmarraztea desgaitzen du, bestela 
(EGIA bada) berriro marraztea gaitzen du. Aldaketa hau mantendu egingo da nahiz 
eta script-ak irten egin, beraz ziurtatu amaitzean setRedraw(TRUE) funtzioari deitzeaz: jarri zure script-aren maila gorenean.</translation>
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
        <translation type="unfinished">createRect(x, y, zabalera, altuera, [&quot;izena&quot;]) -&gt; katea

Laukizuzen berria sortzen du uneko orrian eta bere izena itzultzen du. Koordenatuak uneko dokumentua neurtzeko unitateetan ematen dira (ikus UNIT konstanteak). &quot;Izena&quot; objektuaren identifikatzaile bakarra izan daiteke, etorkizunean objektu honi erreferentzia egiteko erabiliko baita. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emago dio zure ordez.

NameExistsError abiaraziko da esplizitoki emandako izena lehendik ere 
egonez gero.</translation>
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
        <translation type="unfinished">newPage(non [,&quot;orriMaisua&quot;])

Orrialde berria sortzen du. &quot;non&quot; -1 balioa izanez gero, orrialde berria erantsiko zaio 
dokumentuari, bestela orri berria &quot;non&quot; aurretik txertatuko da. Orrialdeen zenbakiak 
1etik gorantz zenbatzen dira, berdin dio dokumentuko aurreneko orrialdearen 
zenbakia zein den. Aukerazko &quot;orrimaisua&quot; parametroak orri maisuaren izena zehazten du orrialde berriarentzako.

Orrialdearen zenbakia barrutitik kanpo egonez gero IndexError abiaraziko da.
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation type="unfinished">setGradientFill(mota, &quot;kolorea1&quot;, itzaldura1, &quot;kolorea2&quot;, itzaldura2, [&quot;izena&quot;])

&quot;izena&quot; objektuaren gradiente betegarria motarekin ezartzen du. Koloreen 
azalpenak setFillColor() eta setFillShade() funtzioetakoak bezalakoak dira. Ikusi mota erabilgarrien konstanteak (FILL_&lt;mota&gt;).</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished">getFontSize([&quot;izena&quot;]) -&gt; mugikorra

&quot;izena&quot; testu-markoaren letra-tamaina itzultzen du puntuetan. Testu-markoak testu zati bat hautatuta edukiz gero, hautapeneko aurreneko karaktereari esleitutako balioa itzuliko du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished">messagebarText(&quot;katea&quot;)

&quot;katea&quot; Scribus aplikazioko mezu-barran (egoera-barran) idazten du. 
Testua UTF8 kodeketako edo Unicode motako katea izan behar du (gomendatua).</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished">importSVG(&quot;katea&quot;)

SVG irudiaren baliozko fitxategi-izena eduki behar du &quot;katea&quot; parametroak. 
Testua UTF8 kodeketako edo Unicode motako katea izan behar du (gomendatua).</translation>
    </message>
</context>
<context>
    <name>#</name>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="obsolete">getFontSize([&quot;izena&quot;]) -&gt; mugikorra

&quot;izena&quot; testu-markoaren letra-tamaina itzultzen du puntuetan. Testu-markoak testu zati bat hautatuta edukiz gero, hautapeneko aurreneko karaktereari esleitutako balioa itzuliko du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="obsolete">getColorNames() -&gt; zerrenda

Dokumentuan definitutako kolore guztien izenen zerrenda itzultzen du. 
Dokumenturik ez balego irekita, dokumentu lehenetsiaren koloreen zerrenda 
itzuliko luke.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation type="obsolete">newDocDialog() -&gt; boolearra

&quot;Dokumentu berria&quot; elkarrizketa-koadroa bistaratzen du. Dokumentu berria sortuko du erabiltzaileak ezarpenak onartuz gero. Ez du dokumenturik sortuko 
erabiltzaileak bertan behera uzteko botoia sakatzen badu. EGIA itzultzen du 
dokumentu bat sortzean.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="obsolete">getFillColor([&quot;izena&quot;]) -&gt; katea

&quot;izena&quot; objektuaren kolore-betegarriaren izena itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation type="obsolete">setRedraw(boolearra)

Boolearraren balioa FALSE (FALTSUA) denean orria birmarraztea desgaitzen du, bestela 
(EGIA bada) berriro marraztea gaitzen du. Aldaketa hau mantendu egingo da nahiz 
eta script-ak irten egin, beraz ziurtatu amaitzean setRedraw(TRUE) funtzioari deitzeaz: jarri zure script-aren maila gorenean.</translation>
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
        <translation type="obsolete">createRect(x, y, zabalera, altuera, [&quot;izena&quot;]) -&gt; katea

Laukizuzen berria sortzen du uneko orrian eta bere izena itzultzen du. Koordenatuak uneko dokumentua neurtzeko unitateetan ematen dira (ikus UNIT konstanteak). &quot;Izena&quot; objektuaren identifikatzaile bakarra izan daiteke, etorkizunean objektu honi erreferentzia egiteko erabiliko baita. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emago dio zure ordez.

NameExistsError abiaraziko da esplizitoki emandako izena lehendik ere 
egonez gero.</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation type="obsolete">setGradientFill(mota, &quot;kolorea1&quot;, itzaldura1, &quot;kolorea2&quot;, itzaldura2, [&quot;izena&quot;])

&quot;izena&quot; objektuaren gradiente betegarria motarekin ezartzen du. Koloreen 
azalpenak setFillColor() eta setFillShade() funtzioetakoak bezalakoak dira. Ikusi mota erabilgarrien konstanteak (FILL_&lt;mota&gt;).</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="obsolete">messagebarText(&quot;katea&quot;)

&quot;katea&quot; Scribus aplikazioko mezu-barran (egoera-barran) idazten du. 
Testua UTF8 kodeketako edo Unicode motako katea izan behar du (gomendatua).</translation>
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
        <translation type="obsolete">newPage(non [,&quot;orriMaisua&quot;])

Orrialde berria sortzen du. &quot;non&quot; -1 balioa izanez gero, orrialde berria erantsiko zaio 
dokumentuari, bestela orri berria &quot;non&quot; aurretik txertatuko da. Orrialdeen zenbakiak 
1etik gorantz zenbatzen dira, berdin dio dokumentuko aurreneko orrialdearen 
zenbakia zein den. Aukerazko &quot;orrimaisua&quot; parametroak orri maisuaren izena zehazten du orrialde berriarentzako.

Orrialdearen zenbakia barrutitik kanpo egonez gero IndexError abiaraziko da.
</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="obsolete">importSVG(&quot;katea&quot;)

SVG irudiaren baliozko fitxategi-izena eduki behar du &quot;katea&quot; parametroak. 
Testua UTF8 kodeketako edo Unicode motako katea izan behar du (gomendatua).</translation>
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
        <translation type="obsolete">newDocument(tamaina, marjinak, orientazioa, lehenOrriZenbakia,
                        unitatea, orriMotak, lehenOrriOrdena) -&gt; boolearra

Dokumentu berria sortzen du, eta TRUE (EGIA) itzultzen du ongi sortzen badu. Parametroek honako esanahia dute:

tamaina = dokumentuaren tamaina azaltzen duen tupla (zabalera, altuera). 
Aurredefinitutako PAPER_&lt;paper_mota&gt; konstanteak erabil ditzakezu, adib. 
PAPER_A4, e.a.

marjinak = dokumentuaren marjinak zehatzen duen tupla (ezkerra, eskuina, ,goia, behea).

orientazioa = orriaren orientazioa - konstanteak: PORTRAIT (bertikala), 
LANDSCAPE (horizontala)

lehenOrriZenbakia = dokumentuko aurreneko orriaren zenbakia da, orriak 
zenbatzeko erabiliko dena. Gehienetan 1 balioa erabiltzea nahiko duzun arren, 
zenbaki altuagoak erabil ditzakezu hainbat zatiko dokumentua sortzean.

unitatea = dokumentua neurtzeko erabilitzen den unitatea ezartzen du. 
Aurrez definitutako konstanteak erabil ditzakezu: UNIT_INCHES (atzak), 
UNIT_MILLIMETERS (milimetroak), UNIT_PICAS (pikak) , UNIT_POINTS (puntuak).

orriMotak = aurrez definitutako PAGE_n konstanteetariko bat. 
Page_1 orri bakuna da, PAGE_2 bi aldetako dokumentuak dira, PAGE_3 3 orriko eta PAGE_4 4 orriko tolesturak dira.

lehenOrriOrdena = dokumentuko aurreneko orriaren posizioa. 
0tik indexatua (0 = lehena).

Zabalera, altuera eta marjinen balioak dokumentuko unitateetan ematen dira. 
PAPER_* konstanteak puntuetan adierazita daude. Dokumentua ez badago 
puntuetan neurtuta, ziurtatu hau kontutan duzula.

adibidea: newDocument(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 7, UNIT_POINTS,
PAGE_4, 3)

ScribusError abiaraziko du lehenOrriOrdena eremuko balioa orriMotak baimendutakoa baino 
handiagoa bada.
</translation>
    </message>
</context>
<context>
    <name>@default</name>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;izena&quot;]) -&gt; katea

Testu-markoaren letra-tipoaren izena itzultzen du. Testu-markoak testu zati bat 
hautatuta edukiz gero, hautapeneko aurreneko karaktereari esleitutako balioa 
itzuliko da. &quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;izena&quot;]) -&gt; osokoa

&quot;izena&quot; testu-markoan dagoen testuaren luzera itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;izena&quot;]) -&gt; katea

&quot;izena&quot; testu-markoan dagoen testua itzultzen du. Testu-markoan testu zati bat 
hautatuta egonez gero, hautatutako testua itzuliko du. Markoan dagoen testu 
guztia itzultzen du, ez ikusgai dagoen testua bakarrik. &quot;izena&quot; ez bada ematen, unean 
hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>getAllText([&quot;izena&quot;]) -&gt; katea

&quot;izena&quot; testu-markoan dagoen eta marko honi estekatuta dauden beste 
testu-marko guztien testua itzultzen du. Testu-markoan testu zati bat hautatuta 
egonez gero, hautatutako testua itzuliko du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.

</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;izena&quot;]) -&gt; mugikorra

&quot;izena&quot; testu-markoaren lerrotartea (&quot;aurreko tartea&quot;) itzultzen du puntuetan adieraziz.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;izena&quot;]) -&gt; mugikorra

&quot;izena&quot; testu-markoaren zutabe-tartearen tamaina itzultzen du puntuetan adieraziz.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;izena&quot;]) -&gt; osokoa

&quot;izena&quot; testu-markoko zutabeen kopurua lortzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;esaldia&quot;, [&quot;izena&quot;])

&quot;izena&quot; testu-markoko testua &quot;esaldia&quot; testuarekin ezartzen du. 
Testua (edo esaldia) UTF8 kodeketan egon behar du - erabili adibidez 
unicode (testua, &apos;iso-8859-2&apos;). Ikus MEG xehetasun gehiagorako. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation>setFont(&quot;letraTipoa&quot;, [&quot;izena&quot;])

&quot;izena&quot; testu-markoko letra-tipoa &quot;letraTipoa&quot;rekin ezartzen du. 
Testu zati bat hautatuta egonez gero, hautatutako testua 
soilik aldatuko da. &quot;izena&quot; ez bada ematen, unean hautatutako 
elementua erabiliko da. 

ValueError abiaraziko da letra-tipoa ez bada aurkitzen.
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
        <translation>setFontSize(tamaina, [&quot;izena&quot;])

&quot;izena&quot; testu-markoaren letra-tamaina &quot;tamaina&quot;rekin ezarritzen du. 
&quot;tamaina&quot; eremuko balioa puntuetan adierazita balego gisa erabiliko da. 
Testu zatiren bat hautatuta egonez gero, hautatutako testua bakarrik 
aldatuko da. &quot;tamaina&quot;ren barrutia 1 eta 512 arteko da. &quot;izena&quot; ez 
bada ematen, unean hautatutako elementua erabiliko da.

ValueError abiaraziko da letra-tamaina mugetatik kanpo izanez gero.
</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(tamaina, [&quot;izena&quot;])

&quot;izena&quot; testu-markoaren lerroartea (&quot;aurreko tartea&quot;) &quot;tamainarekin&quot; 
ezartzen du. &quot;tamaina&quot; puntuetan adierazitako balioa da. &quot;izena&quot; ez 
bada ematen, unean hautatutako elementua erabiliko da.

ValueError abiaraziko da lerroartea mugetatik kanpo izanez gero.
</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(tamaina, [&quot;izena&quot;])

&quot;izena&quot; testu-markoaren zutabe-tartearen &quot;tamaina&quot; balioarekin ezartzen 
du. &quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.

ValueError abiaraziko da zutabe-tartea mugetatik kanpo izanez 
gero (positiboa izan behar du).
</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(zutKop, [&quot;izena&quot;])

&quot;izena&quot; testu-markoan egongo diren zutabe-kopurua &quot;zutKop&quot; osokoarekin ezatzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.

ValueError abiaraziko da zutabe-kopurua gutxienez bat ez bada.
</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(lerrokatu, [&quot;izena&quot;])

&quot;izena&quot; testu-markoko testua lerrokatzen du zehaztutako &quot;lerrokatu&quot; eremuarekin. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. &quot;lerrokatu&quot; 
eremuan modulo honetan zehaztutako ALIGN_* konstanteetariko bat erabili 
behar da: ikus dir(scribus).

ValueError abiaraziko da baliogabeko konstantea erabiltzen bada lerrokatzeko.</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;izena&quot;])

&quot;izena&quot; testu-markoko testua ezabatzen du. Testu zatiren bat hautatuta egonez, 
gero hautatutako testua ezabatuko du. &quot;izena&quot; ez bada ematen, unean 
hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;kolorea&quot;, [&quot;izena&quot;])

&quot;izena&quot; testu-markoko testuaren kolorea &quot;kolorea&quot;rekin ezartzen du. Testu 
zatiren bat hautatuta egonez gero, hautatutako testua aldatuko du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;kolorea&quot;, [&quot;izena&quot;])

Testuaren trazuaren &quot;kolorea&quot; ezartzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
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
        <translation>setTextShade(itzaldura, [&quot;izena&quot;])

&quot;izena&quot; objektuaren testu-kolorearen itzaldura ezartzen du &quot;itzaldura&quot;rekin. 
Testu zatiren bat hautatuta egonez gero, hautatutako testua aldatuko du. 
&quot;itzaldura&quot; eremuak 0 (argiena) eta 100 (intentsitate osoko kolorea) arteko balioa 
izan behar du. &quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;izenetik&quot;, &quot;izenera&quot;)

Bi testu-marko estekatzen ditu. &quot;izenetik&quot; markoa estekatuko du 
&quot;izenera&quot; markora. Helburuko markoa testu-marko hutsa izan behar du 
eta ezin du beste marko batekin edo batetik estekatuta egon.

ScribusException abiaraziko da estekatzeko arauak bortxatuz gero.
</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;izena&quot;)

Izendatutako objektua kentzen du testu-marko mugikorretik/esteketatik. 
Markoa kate baten erdian balego aurreko eta hurrengo markoak elkarrekin 
konektatu egingo lirateke: adbi. unlinkTextFrames(&quot;b&quot;) exekutatzen bada, 
&apos;a-&gt;b-&gt;c&apos; honakoa izango da:  &apos;a-&gt;c&apos;.

ScribusException abiaraziko da estekatzeko arauak bortxatuz gero.</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;izena&quot;])

&quot;izena&quot; testu-markoa eskemara bihurtzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>getColor(&quot;name&quot;) -&gt; tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>getColor(&quot;izena&quot;) -&gt; tupla

Uneko dokumentuko &quot;izena&quot; kolorearen lau kolore-osagaien (C, M, H, B) tupla 
itzultzen du. Dokumenturik ez balego irekita, dokumentu lehenetsiaren 
koloreetatik izendatutako kolorearen balioa itzultzen du.

NotFoundError abiaraziko da izendatutako kolorerik ez bada aurkitzen.
ValueError abiaraziko da baliogabeko kolore-izena zehazten bada.
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
        <translation>changeColor(&quot;izena&quot;, c, m, h, b)

&quot;izena&quot; kolorea CMHB balioarekin aldatzen du. Kolorearen balioa lau osagaien 
bidez zehazten da (c=cyana, m=magenta, h=horia, b=beltza). Kolorearen 
osagaiek 0 eta 255 arteko balio izan dezakete.

NotFoundError abiaraziko da izendatutako kolorerik ez bada aurkitzen.
ValueError abiaraziko da baliogabeko kolore-izena zehazten bada.
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
        <translation>deleteColor(&quot;izena&quot;, &quot;ordezkoa&quot;)

&quot;izena&quot; kolorea ezabatzen du. Izendatutako kolorearen agerpen bakoitza 
&quot;ordezkoa&quot;rekin ordeztuko da. Ez bada zehazten, &quot;ordezkoa&quot; lehenetsi egingo da 
&quot;Bat ere ez&quot; kolorearekin (gardena).

Dokumenturik ez balego irekita deleteColor dokumentu lehenetsiko koloreetan 
funtzionatuko luke. Kasu horretan, &quot;ordezkoa&quot; zehaztuko balitz, ez luke eraginik 
izango.

NotFoundError abiaraziko da izendatutako kolorerik ez bada aurkitzen.
ValueError abiaraziko da baliogabeko kolore-izena zehazten bada.</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;izena&quot;, &quot;ordezkoa&quot;)

&quot;izena&quot; kolorearen agerpen bakoitza &quot;ordezkoa&quot; kolorearekin ordetuko da.

NotFoundError abiaraziko da izendatutako kolorerik ez bada aurkitzen.
ValueError abiaraziko da baliogabeko kolore-izena zehazten bada.

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
        <translation>messageBox(&quot;epigrafea&quot;, &quot;mezua&quot;,
    ikonoa=ICON_NONE, botoi1=BUTTON_OK|BUTTONOPT_DEFAULT,
    botoi2=BUTTON_NONE, botoi3=BUTTON_NONE) -&gt; osokoa

&quot;epigrafea&quot; titulua duen elkarrizketa-koadroa bistaratzen du, &quot;mezua&quot; 
mezuarekin eta &quot;ikonoa&quot; ikonoarekin, eta gehienez 3 botoirekin. Lehenetsi gisa 
ez da ikonorik erabiltzen eta botoi bakarrak, ADOS, bistaratzen da. Epigrafea eta 
mezua soilik dira beharrezkoak, nahiz eta dagokion ikonoa eta botoia(k) 
ezartzea gomendatzen den. Mezuaren testuak HTML motako etiketa 
sinpleak eduki ditzake.

Erabiltzaileak sakatutako botoiaren zenbakia itzultzen du. Botoien zenbakiak 
1 balioarekin hasten dira.

Ikonoaren eta botoien parametroak aurrez definitutako konstante erabilgarriak 
dira, Qt dokumentazioan agertzen diren izenekin.  Hauek BUTTON_* eta ICON_* 
konstanteak dira, moduloan definitutakoak. Beste bi konstante gehigarri daude 
bit mailan OR (EDO) eragiketa egin dezaketena:

    BUTTONOPT_DEFAULT   Sartu tekla sakatuz botoi hau sakatzen da.
    BUTTONOPT_ESCAPE    Ihes tekla sakatuz botoi hau sakatzen da.

Adibideak:
emaitza = messageBox(&apos;Script-ak huts egin du&apos;,
                    &apos;Script honek testu-markoa hautatuta duzunean soilik funtzionatzen du.&apos;,
                    ICON_ERROR)
emaitza = messageBox(&apos;Zoratu gara!&apos;, &apos;Zerbait oker dabil! &lt;i&gt;Mozkorren bat agian?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Definitutako botoien eta ikonoen konstanteak:
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
        <translation>valueDialog(epigrafea, mezua [,balioLehenetsia]) -&gt; katea

&apos;Eskatu esaldia&apos; motako elkarrizketa-koadroa bistaratzen du, eta 
bere balioa itzultzen du kate gisa.
Parametroak: leihoaren titulua, leihoko testua eta aukerazko &apos;lehenetsia&apos; 
balioa.

Adibidea:  valueDialog(&apos;titulua&apos;, &apos;leihoko testua&apos;, &apos;aukerazkoa&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Uneko dokumentua itxi egiten du, gordetzeari buruz inolako galderarik egin gabe.

NoDocOpenError abiaraziko da dokumenturik irekita ez egonez gero.
</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; boolearra

TRUE (EGIA) itzultzen du dokumenturen bat irekita egonez gero.
</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;izena&quot;)

&quot;izena&quot; dokumentua irekitzen du.

ScribusError abiaraziko da dokumentua ezin izan badu ireki.
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

Uneko dokumentua (bere uneko izenarekin) gordetzen du, eta TRUE (EGIA) itzultzen 
du ongi burutu bada. Dokumentua oraindik ez bada gorde, elkarrizketa-koadroa 
bistaratuko du fitxategia gordetzeko.

Huts egiten badu gordetzean, une honetan ez dago biderik hau jakinarazteko.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;izena&quot;)

Uneko dokumentua &quot;izena&quot; izen berriarekin gordetzen du (bide-izen osoa edo 
erlatiboa izan daiteke).

ScribusError abiaraziko da huts egiten badu gordetzean.
</translation>
    </message>
    <message>
        <source>setInfo(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>setInfo(&quot;egilea&quot;, &quot;info&quot;, &quot;azalpena&quot;) -&gt; boolearra

Dokumentuaren informazioa ezartzen du. &quot;egilea&quot;, &quot;info&quot; eta &quot;azalpena&quot; kateak 
dira.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(ez, es, go, be)

Dokumentuaren marjinak ezartzen ditu. Ezkerreko (ez), eskuineko (es), goiko (go) eta 
beheko (be) marjinak ematen dira dokumentua neurtzeko unitateetan 
(ikus UNIT_&lt;mota&gt; konstanteak).</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(mota)

Dokumentua neurtzeko unitatea aldatzen du. Unitate motaren balio erabilgarriak 
UNIT_&lt;mota&gt; konstante gisa definituta daude.

ValueError abiaraziko da baliogabeko unitatea ematen bada.
</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; osokoa (Scribus-eko unitate konstantea)

Dokumentua neurtzeko ezarritako unitatea itzultzen du. Itzulitako balioa 
UNIT_* konstanteetariko bat izan daiteke:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;fitxategi-izena&quot;)

Paragrafo-estiloak kargatzen ditu uneko dokumentuan 
Scribus dokumentik (&quot;fitxategi-izena&quot;)  .
</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(aurrezAurreOrri, lehenOrriaEzker)

Dokumentu-mota ezartzen du. Ezarri lehenbiziko parametroa 
FACINGPAGES konstantearekin orrialdeak aurrez aurre jartzeko, 
bestela, orrialdeak aurrez aurre ez egoteko erabili NOFACINGPAGES. 
Aurreneko orrialdea ezkerraldean egoteko ezarri bigarren paraemetroa 
FIRSTPAGELEFT balioarekin, bestela erabili FIRSTPAGERIGHT aurrenekoa 
orrialdea eskuinean egoteko.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;izena&quot;]) -&gt; katea

&quot;izena&quot; objektuaren marraren kolorearen izena itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;izena&quot;]) -&gt; osokoa

&quot;izena&quot; objektuaren marraren zabalera itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;izena&quot;]) -&gt; osokoa

&quot;izena&quot; objektuaren marraren kolorearen itzalduraren balioa itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;izena&quot;]) -&gt; osokoa (ikus konstanteak)

&quot;izena&quot; objektuaren marrak elkartzeko estiloa itzultzen du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
Elkartzeko motak honakoak dira:
JOIN_BEVEL (alaka), JOIN_MITTER, JOIN_ROUND (biribila)
</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;izena&quot;]) -&gt; osokoa (ikus konstanteak)

&quot;izena&quot; objektuaren marraren txano-estiloa itzultzen du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
Txanoen motak honakoak dira: 
CAP_FLAT (laua), CAP_ROUND (biribila), CAP_SQUARE (karratua)
</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;izena&quot;]) -&gt; osokoa (ikus konstanteak)

&quot;izena&quot; objektuaren marraren estiloa itzultzen du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
Marra-estiloen konstanteak honakoak dira: 
LINE_DASH (marra), LINE_DASHDOT (marra puntu), 
LINE_DASHDOTDOT (marra puntu puntu), LINE_DOT (puntu), LINE_SOLID (betea)
</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;izena&quot;]) -&gt; osokoa

&quot;izena&quot; objektuaren kolore betegarriaren itzalduraren balioa itzultzen du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;izena&quot;]) -&gt; (x,y)

&quot;izena&quot; irudi-markoaren eskalatze-balioen tupla (x,y) itzultzen du. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;izena&quot;]) -&gt; katea

&quot;izena&quot; irudi-markoaren irudiari dagokion fitxategi-izena itzultzen du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;izena&quot;]) -&gt; (zabalera,altuera)

&quot;izena&quot; objektuaren tamainaren tupla (zabalera, altuera) itzultzen du.
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
Dokumentua neurtzeko uneko unitatea erabiltzen du tamainak 
(ikus UNIT_&lt;mota&gt; xehetasunerako).
</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;izena&quot;]) -&gt; osokoa

&quot;izena&quot; objektuaren biraketa itzultzen du. Balioa gradutan adierazten da, 
eta erlojuaren norantza positiboa da. &quot;izena&quot; ez bada ematen, unean 
hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; zerrenda

Zerrenda itzultzen du, uneko orrialdeko objektu guztien izenen edukiarekin.
</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;izena&quot;])

&quot;izena&quot; objektua lekuz aldatzen du. Dokumentua neurtzeko uneko unitatea erabiltzen 
du koordenatuak (ikus UNIT konstanteak). 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da. 
&quot;izena&quot;  objektua talde bateko kidea bada, talde osoa aldatuko da lekuz.
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
        <translation>rotateObject(bira [, &quot;izena&quot;])

&quot;izena&quot; objektua &quot;bira&quot; gradu biratzen du erlatiboki. 
Objektua biratzeko puntu gisa hautatutako erpinarekiko biratuko da, lehenespenez 
biraketarik eduki gabeko objektuan goiko ezkerreko erpina izan ohi da.
Balio positiboek erlojuaren aurkako norantza adierazten dute, biratzeko puntu 
lehenetsia erabiltzen denean. &quot;izena&quot; ez bada ematen, unean hautatutako 
elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(zabalera, altuera [, &quot;izena&quot;])

&quot;izena&quot; objektua tamainaz aldatzen du emandako zabalera eta altuerarekin. 
&quot;izena&quot; ez bada ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([kop]) -&gt; katea

Hautatutako objektuaren izena itzultzen du. &quot;kop&quot; balioa ematen bada, 
hautatutako objektu-kopurua adierazten du. 0 balioak hautatutako aurreneko 
objektua adierazten du, 1 balioak hautatutako bigarren objektua, e.a.
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; osokoa

Hautatutako objektu-kopurua itzultzen du.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;izena&quot;)

Emandako &quot;izena&quot; objektua hautatzen du.
</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Dokumentu osoko objektu guztiak desautatzen ditu.
</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(zerrenda)

&quot;zerrenda&quot;n agertzen diren objektu guztiak elkartzen ditu. Taldean elkartzea 
nahi diren objektuen izenak eduki behar ditu &quot;zerrenda&quot;k. Zerrendarik ez bada 
ematen, unean hautatutako elementuak erabiliko dira.
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;izena&quot;)

&quot;izena&quot; objektua kide den taldea apurtzen du. Ez bada &quot;izena&quot; ematen, 
unean hautatutako elementua erabiliko da.</translation>
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
        <translation>scaleGroup(faktorea [,&quot;izena&quot;])

&quot;izena&quot; objektuaren taldea eskalatzen du. 1 baino handiagoak diren 
faktoreek taldea handitzen dute, txikiagoak direnek taldea txikiagotzen 
dute. Adib: 0.5 balioko faktore batek jatorrizko tamainaren %50era eskalatzen 
du taldea; 1.5 balioak berriz taldea %150 handiagotzen du. &quot;faktorea&quot; eremuko 
balioa 0 baino handiagoa izan behar du. Ez bada &quot;izena&quot; ematen 
unean hautatutako elementua erabiliko da.

ValueError abiaraziko da faktore baliogabea ematen bada eskalatzeko.
</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;fitxategi-izena&quot; [, &quot;izena&quot;])

&quot;fitxategi-izena&quot; irudia kargatzen du &quot;izena&quot; irudi-markoan. 
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da.

WrongFrameTypeError abiaraziko da helburuko markoa ez bada irudi-marko bat.
</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;izena&quot;])

&quot;izena&quot; irudi-markoko irudia eskalatzeko faktorea ezartzen du. 
Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da. 1 zenbakiak 
%100 adierazten du.

WrongFrameTypeError abiaraziko da helburuko markoa ez bada irudi-marko bat.
</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;izena&quot;]) -&gt; boolearra

&quot;izena&quot; objektua blokeatzen du desblokeatuta egonez gero, edo 
desblokeatu egiten du blokeatuta egonez gero. Ez bada &quot;izena&quot; emanten, 
unean hautatutako elementua erabiliko da. TRUE (EGIA) itzultzen du 
blokeatzen badu.
</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;izena&quot;]) -&gt; boolearra

TRUE (EGIA) itzultzen du &quot;izena&quot; objektua blokeatuta egonez gero. 
Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; zerrenda

Letra-tipo erabilgarri guztien izena duen zerrenda itzultzen du.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; tuplen zerrenda

Letra-tipo handien datuak itzultzen ditu. Tuplen zerrenda da:
[ (Scribus izena, familia, benetako izena, azpimultzoa (1|0), 
kapsulatutako PS (1|0), letra-fitxategia), (...), ... ]
</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; zerrenda

Definitutako geruza guztien izena duen zerrenda itzultzen du.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;izena&quot;)

&quot;izena&quot; geruza ezartzen du geruza aktibo gisa.

NotFoundError abiaraziko da geruza ez bada aurkitzen.
ValueError abiaraziko da geruzaren izena onarkaitza bada.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; katea

Uneko geruza aktiboaren izena itzultzen du.</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;geruza&quot; [, &quot;izena&quot;])

&quot;izena&quot; objektua &quot;geruza&quot; geruzara eramango du. Geruzak existitu egin behar du. 
Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da.

NotFoundError abiaraziko da geruza ezin bada aurkitu.
ValueError abiaraziko da geruzaren izena onarkaitza bada.
</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;geruza&quot;, ikusgai)

&quot;geruza&quot; izeneko geruza ikusgai den, edo ez, ezartzen du. &apos;ikusgai&apos; eremuan 
FALSE (FALTSUA) balioa jartzen bada, geruza ikuskaitza izango da.

NotFoundError abiaraziko da geruza ezin bada aurkitu.
ValueError abiaraziko da geruzaren izena onarkaitza bada.
</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">setLayerPrintable(&quot;geruza&quot;, inprimagarria)

&quot;geruza&quot; izeneko geruza inprimagarria den, edo ez, ezartzen du. 
Inprimagarria eremuan FALSE (FALTSUA) ezartzen bada, ezin izango da geruza inprimatu.

NotFoundError abiaraziko da geruza ezin bada aurkitu.
ValueError abiaraziko da geruzaren izena onarkaitza bada.
</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;geruza&quot;)

&quot;geruza&quot; izeneko geruza ezabatzen du. 
Ez da ezer gertatzen geruza ez bada existitzen, edo dokumentuko 
geruza bakarra izanez gero.

NotFoundError abiaraziko da geruza ezin bada aurkitu.
ValueError abiaraziko da geruzaren izena onarkaitza bada.</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(izena)

&quot;izena&quot; izeneko geruza berria sortzen du.

ValueError abiaraziko da geruzaren izena onarkaitza bada.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; katea

Kate bat itzultzen du -lang (hizkuntza) balioarekin.
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
        <translation>createEllipse(x, y, zabalera, altuera, [&quot;izena&quot;]) -&gt; katea

Elipse berria sortzen du uneko orrialdean, eta bere izena itzultzen du. 
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
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
        <translation>createImage(x, y, zabalera, altuera, [&quot;izena&quot;]) -&gt; katea

Irudi-marko berria sortzen du uneko orrialdean, eta bere izena itzultzen du. 
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
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
        <translation>createText(x, y, zabalera, altuera, [&quot;izena&quot;]) -&gt; katea

Testu-marko berria sortzen du uneko orrialdean, eta bere izena itzultzen du. 
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
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
        <translation>createLine(x1, y1, x2, y2, [&quot;izena&quot;]) -&gt; katea

Marra berria sortzen du 1 puntutik (x1,y1) 2. puntura (x2, y2), eta 
bere izena itzultzen du. 
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
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
        <translation>createPolyLine(zerrenda, [&quot;izena&quot;]) -&gt; katea

Koska-marra berria sortzen du uneko orrialdean, eta bere izena itzultzen du. 
Koska-marraren puntuak zerrenda batean datozte, honako ordenean: 
[x1, y1, x2, y2...xn. yn].
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
ValueError abiaraziko da behar diren puntu-kopurua ez bada ematen, edo emandako 
balioen kopurua ez bada hondarrik gabeko puntuekin elkartzen.
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
        <translation>createPolygon(zerrenda, [&quot;izena&quot;]) -&gt; katea

Poligono berria sortu eta bere izena itzultzen du. 
Poligonoaren puntuak zerrenda batean datozte, honako ordenean: 
[x1, y1, x2, y2...xn. yn]. Gutxienez hiru puntu behar dira, 
eta aurreneko puntua ez dago errepikatu beharrik poligonoa ixteko (osatzeko). 
Poligonoa automatikoki ixten da aurreneko eta azken puntuak konektatuz.
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
ValueError abiaraziko da behar diren puntu-kopurua ez bada ematen, edo emandako 
balioen kopurua ez bada hondarrik gabeko puntuekin elkartzen.
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
        <translation>createBezierLine(zerrenda, [&quot;izena&quot;]) -&gt; katea

Bezier kurba berria sortzen du, eta bere izena itzultzen du. 
Bezier kurbaren puntuak zerrenda batean datozte, honako ordenean: 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn] 
Zerrendako puntuetan x eta y balioek puntuaren x eta y koordenatuak 
adierazten dute, eta kx eta ky balioek berriz kurbaren kontrol-puntuak.
Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada.
ValueError abiaraziko da behar diren puntu-kopurua ez bada ematen, edo emandako 
balioen kopurua ez bada hondarrik gabeko puntuekin elkartzen.</translation>
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
        <translation>createPathText(x, y, &quot;testu-kutxa&quot;, &quot;bezierkurba&quot;, [&quot;izena&quot;]) -&gt; katea

Testu-bide berria sortzen du &apos;testu-kutxa&apos; eta &apos;bezierkurba&apos; batuz, eta bere izena 
itzultzen du. Dokumentua neurtzeko uneko unitateetan ematen dira koordenatuak 
(ikus UNIT konstanteak).  &quot;izena&quot; objektuaren identifikatzaile bakarra izan 
behar du, etorkizunean bere izena erabili beharko baituzu objektu honi 
erreferentzi egiteko. &quot;izena&quot; ez bada ematen, Scribus aplikazioak emango 
dio.

NameExistsError abiaraziko da jadanik erabilita dagoen izena esplizitoki ematen bada. 
NotFoundError abiaraziko da emandako oinarrizko objektuen izen bat edo biak ez badira existitzen.</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;izena&quot;])

&quot;izena&quot; izeneko objektua ezabatzen du. Ez bada &quot;izena&quot; ematen, unean hautatutako 
elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation type="obsolete">textFlowsAroundFrame(&quot;izena&quot; [, egoera])

&quot;izena&quot; objektuaren &quot;Testua markoaren inguruan&quot; funtzioa gaitu/desgaitu egiten du
Erabiltzen diren parametroak izena katea eta aukerako &apos;egoera&apos; boolearra dira. 
Ez bada &apos;egoera&apos; ematen, testuaren jarioa txandakatu egiten da.
</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;izena&quot;]) -&gt; boolearra

Izendatutako objektua dokumentuan existitzen den begiratzen du. 
Aukerako parametroa objektuaren izenda da. Izenik ez denean ematen TRUE 
(EGIA) itzultzen du zerbait hautatuta egonez gero.
</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;estiloa&quot; [, &quot;izena&quot;])

&quot;estiloa&quot; izenekoa aplikatzen dio &quot;izena&quot; objektuari. Ez bada objektuaren 
izenik ematen, hautatutako objektuari aplikatuko zaio.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; zerrenda

Zerrenda itzultzen du, uneko dokumentuan dauden paragrafo-estilo guztien izenekin.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; osokoa

Unean lantzen ari den orrialdearen zenbakia itzultzen du. 
Orrialdeen zenbakiak 1etik gorantz zenbatzen dira, berdin dio dokumentuko 
aurreneko orrialdearen bistaratutako zenbakia zein den.
</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Orrialde guztiak berriro marrazten ditu.
</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;izena&quot;)

Uneko orrialdea EPS gisa gordetzen du &quot;izena&quot; fitxategian.

ScribusError abiaraziko da huts egiten badu gordetzean.
</translation>
    </message>
    <message>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation>deletePage(zbkia)

Emandako orrialdea ezabatzen du. Ez du ezer egiten dokumentuak 
orrialde bakarra badu. Orrialdeen zenbakiak 1etik hasten dira zenbatzen, 
eta berdin dio aurrenekoak zein zenbaki bistaratzen duen.

IndexError abiaraziko da orrialdearen zenbakia barrutitik kanpo egonez gero.
</translation>
    </message>
    <message>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation>gotoPage(zbkia)

&quot;zbkia&quot; orrialdera joaten da (hots, uneko orrialdea &quot;zbkia&quot; egiten du). Jakin 
ezazu gotoPage funtzioak ez duela (oraindik behintzat) erabiltzailearen ikuspegian 
bistaratzen den orrialdea aldatzen, scripteko komandoek lana egiteko orrialdea 
ezartzen du soilik.

IndexError abiaraziko da orrialdearen zenbakia barrutitik kanpo egonez gero.
</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; osokoa

Dokumentuko orrialde-kopurua itzultzen du.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; zerrenda

Gida horizontalen posizioak dituen zerrenda itzultzen du. 
Balioak uneko dokumentuaren unitateetan daude (ikus UNIT_&lt;mota&gt; konstanteak).
</translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(zerrenda)

Gida horizontalak ezartzen ditu. Sarrerako parametroak giden posizioen 
zerrenda izan behar du, dokumentua neurtzeko unitateetan (ikus 
UNIT_&lt;mota&gt; konstanteak).

Adibidea: 
         setHGuides(getHGuides() + [200.0, 210.0] # gida berriak 
                                                                # ezartzen ditu galerarik gabe
         setHGuides([90,250])     # uneko gidak osotasunean ordezten ditu
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Ikus getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Ikus setHGuides.
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tupla

Orrialdearen dimensioen tupla itzultzen du, dokumentuaren uneko 
unitateetan neurtuz. Ikus UNIT_&lt;mota&gt; konstanteak eta getPageMargins()
</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; zerrenda

Uneko orrialdean dauden elementuen tuplen zerrenda itzultzen du. 
Tupla honakoa da:
(izena, objektuMota, ordena) Adib.: [(&apos;Testua1&apos;, 4, 0), (&apos;Irudia1&apos;, 2, 1)] 
&apos;Testua1&apos; izeneko objektua testu-markoa da (4 motakoa) 
eta orrialdeko aurrenekoa dela adierazten du...
</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;kolorea&quot;, [&quot;izena&quot;])

&quot;izena&quot; objektuaren kolore betegarria &quot;kolorea&quot;rekin ezartzen du. 
&quot;kolorea&quot; aurrez definitutako kolore bat da. 
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;kolorea&quot;, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marraren kolorea \&quot;kolorea\&quot;rekin ezartzen du. 
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(zabalera, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marraren zabalera ezartzen du &quot;zabalera&quot; balioarekin. 
&quot;zabalera&quot; balioa 0.0 eta 12.0 artekoa izan behar du, eta puntuetan neurtuta 
dago.
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da.

ValueError abiaraziko da marraren zabalera mugetatik at irtetzen bada.
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
        <translation>setLineShade(itzaldura, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marraren itzaldura ezartzen du &quot;itzaldura&quot; balioarekin. 
&quot;itzaldura&quot; balioa 0 (argiena) eta 100 (intentsitate osoko kolorea) 
artekoa osokoa izan behar du. Ez bada &quot;izena&quot; ematen unean 
hautatutako elementua erabiliko da.

ValueError abiaraziko da marraren itzaldura mugetatik at irtetzen bada.</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(elkartu, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marra elkartzeko estiloa &quot;elkartu&quot;rekin ezatzen du. 
Aurrez definituta daude elkartzeko konstanteak: JOIN_&lt;mota&gt;.
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(amaiMota, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marraren txanoaren estiloa ezartzen du &quot;amaiMota&quot;rekin. 
Aurrez definituta daude txanoen konstanteak: CAP_&lt;mota&gt;.
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(estiloa, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marraren estiloa ezartzen du &quot;estiloa&quot;rekin. 
Aurrez definituta daude estiloen konstanteak: LINE_&lt;estiloa&gt;.
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da. 
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
        <translation>setFillShade(itzaldura, [&quot;izena&quot;])

&quot;izena&quot; objektuaren kolore-betegarriaren itzaldura ezartzen du &quot;itzaldura&quot;rekin. 
&quot;itzaldura&quot; balioa 0 (argitsuena) eta 100 (intentsitate osoko kolorea) artean egon behar du.
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da. 

ValueError abiaraziko da itzalduraren betegarria mugetatik kanpo egonez gero.
</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(erradioa, [&quot;izena&quot;])

&quot;izena&quot; objektuaren izkineko erradioa ezartzen du. Erradioa 
puntutan adierazten da. Ez bada &quot;izena&quot; ematen 
unean hautatutako elementua erabiliko da. 

ValueError abiaraziko da izkineko erradioa negatiboa bada.
</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;estiloIzena&quot;, [&quot;izena&quot;])

&quot;izena&quot; objektuaren marraren estiloa ezartzen du &quot;estiloIzena&quot;rekin. 
Ez bada &quot;izena&quot; ematen unean hautatutako elementua erabiliko da. 

NotFoundError abiaraziko da marra-estiloa ez bada existitzen.
</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Scribus aplikazioko progresio-barrako aurreko ezarpenak gabritzen ditu. 
Progresio-barra berria erabili aurretik deitu ohi da. Ikus progressSet.
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Progresio-barraren urrats kopuru maximoaren balioa ezartzen du 
emandako balioarekin. Ikus progressSet.
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
        <translation>progressSet(zbkia)

Progresio-barraren posizioa &quot;zbkia&quot; baliora ezartzen du, aurrez ezarritako 
progressTotal-arekiko balio erlatiboa. Progresio-barrak urratsen kontzeptua 
erabiltzen du, urratsen kopuru osoa eta landutako urratsen kopurua eman 
eta berak burututako urratsen ehunekoa bistaratzen du. progressTotal() funtzioarekin 
urratsen kopuru osoa ezar dezakezu. Uneko urratsen kopurua 
progressSet() funtzioarekin ezarrita dago. Progresio-barra berrasiera daiteke 
progressReset() funtzioa erabiliz. [Trolltech-eko Qt dokumentazioan 
oinarritutako datuak]
</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[EZ DAGO ONARTUTA!] Gauza batzuk honda ditzakeenez, oraingoz ez erabili.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(boolearra)

Scribus-eko ikono-barran eta Gorde menu-elementuan ikonoa gordetzea 
gaitu/desgaitu egiten du. Funtzio hau erabilgarria da dokumentuz aldatzean, 
Scribus aplikazioak ez baitu automatikoki jakinarazten script bat erabiltzean 
dokumentuz aldatu denik.
</translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation>setScaleImageToFrame(eskalatuMarkora, proportzionala=None, izena=&lt;hautapena&gt;)

Hautapeneko edo zehaztutako irudi-markoan markoaren eskala ezartzen du 
&apos;eskalatuMarkora&apos; balioarekin. &apos;proportzionala&apos; zehazten bada, itxura eskalatzeko
erlazioa finkatu egingo da. &apos;eskalatuMarkora&apos; eta &apos;proportzionala&apos; parametroak 
boolearrak dira.

WrongFrameTypeError abiaraziko da.
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
        <translation type="obsolete">isLayerPrintable(&quot;geruza&quot;) -&gt; boolearra

&quot;geruza&quot; izeneko geruza ikusgai dagoen edo ez itzultzen du. TRUE (EGIA) 
balioak ikusgai dagoela adierazten du, eta era berean FALSE (FALTSUA) balioak 
ez dagoela ikusgai.

NotFoundError abiaraziko da ezin bada geruza aurkitu.
ValueError abiaraziko da geruzaren izena onarkaitza bada.
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
        <translation>isLayerPrintable(&quot;geruza&quot;) -&gt; boolearra

&quot;geruza&quot; izeneko geruza inprima daitekeen edo ez itzultzen du. TRUE (EGIA) 
balioak inprima daitekeela adierazten du, eta era berean FALSE (FALTSUA) balioak 
inprimatzea desgaituta dagoela.

NotFoundError abiaraziko da ezin bada geruza aurkitu.
ValueError abiaraziko da geruzaren izena onarkaitza bada.
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
        <translation>getColorAsRGB(&quot;izena&quot;) -&gt; tupla

Uneko dokumentuko &quot;izena&quot; kolorearen (GBU kolore-gunera bihurtuta) hiru kolore 
osagaien tupla (G, B, U) itzultzen du. Ez badago dokumenturik irekita, 
dokumentu lehenetsiko &quot;izena&quot; kolorearen balioa itzuliko du.

NotFoundError abiaraziko da ezin bada kolorearen izena aurkitu.
ValueError abiaraziko da kolorearen izena baliogabea bada.
</translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yello and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">defineColor(&quot;izena&quot;, c, m, h, b)

&quot;izena&quot; kolorea definitzen du. Kolorearen balioa lau osagaien bidez zehazten da: 
c = ciana, m = magenta, h = horia, b = beltza. Kolorearen 
osagaiak 0tik 255ra artean egon behar dute.

ValueError abiaraziko da kolorearen izena baliogabea bada.

</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation type="obsolete">getCornerRadius([&quot;izena&quot;]) -&gt; osokoa


&quot;izena&quot; objektuaren izkineko erradioa itzultzen du. Erradioa puntutan adierazten da. 
Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da.
</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="obsolete">getPosition([&quot;izena&quot;]) -&gt; (x,y)

&quot;izena&quot; objektuaren posizioaren tupla (x, y) itzultzen du. 
Posizioa dokumentua neurtzeko unitateetan ematen da (ikus UNIT_&lt;mota&gt;). 
Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da.</translation>
    </message>
    <message>
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation>getPropertyCType(objektua,  propietatea, txertatuSuper=TRUE)

&apos;objektua&apos;ren &apos;propietatearen&apos; C motako izena itzultzen du. 
Ikus getProperty() parametroen xehetasunerako.

&apos;txertatuSuper&apos; TRUE (EGIA) bada, heredatutako propietateetan ere bilatzen du.
</translation>
    </message>
    <message>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation>getPropertyNames(objektua, txertatuSuper=TRUE)

&apos;objektua&apos;k onartutako propietateen izenen zerrenda itzultzen du.
&apos;txertatuSuper&apos; TRUE (EGIA) bada, klasearen gurasoek onartutako 
propietateak ere itzultzen ditu.
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
        <translation>getProperty(objektua, propietatea)

&apos;objektua&apos;ren &apos;propietatea&apos;ren balioa itzultzen du.

&apos;objektua&apos; parametroa katea izan daiteke, egoera horretan PageItem 
izenekoaren bilaketa egiten da. PyCObject ere izan daiteke, 
C++ QObject-eko intantzia batera zuzendu dezake.

&apos;propietatea&apos; parametroa kate bat izan behar du, eta &apos;objektua&apos;n bilatuko den 
propietatearen izen da.

Itzultzen den balioa propietate-motaren arabera dago.
</translation>
    </message>
    <message>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation>setProperty(objektua, propietatea, balioa)

&apos;objektua&apos;ren &apos;propietatea&apos; ezartzen du &apos;balioa&apos;rekin. Balioa ezin bada 
&apos;propietatea&apos;k darabilen motaren bateragarri batera bihurtu, salbuespena 
abiaraziko da. Salbuespena ere abiaraziko da azpimarratzeko ezarpenak 
huts egiten badu.

Ikus getProperty() xehetasun gehiagorako.
</translation>
    </message>
    <message>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation>getChildren(objektua, klasearena=None, izenekoa=None, regexpmatch=False, errekurtsiboa=True)

&apos;objektua&apos;ren umeen zerrenda itzultzen du, litekeena &apos;klasearen&apos;a 
umeetara edo &apos;izenekoa&apos; umera murriztea. &apos;errekurtsiboa&apos; TRUE (EGIA) bada, 
umeen, hiloben, e.a. zehar egingo du bilaketa errekurtsiboki.

Ikus QObject::children() funtzioa informazio gehiagorako Qt dokumentazioan.
</translation>
    </message>
    <message>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation>getChild(objektua, ume-izena, klasearena=None, errekurtsiboa=True)

&apos;objektua&apos;ren aurreneko &apos;ume-izena&apos; itzultzen du, litekeena &apos;klasearen&apos;a 
umeetara murriztea. &apos;errekurtsiboa&apos; TRUE (EGIA) bada, 
umeen, hiloben, e.a. zehar egingo du bilaketa errekurtsiboki.

</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">rotateObjectAbs(bira [, &quot;izena&quot;])

&apos;izena&apos; objektuaren biraketa ezartzen du &apos;bira&apos;rekin. 
Balio positiboek erlojuaren aurkako norantza adierazten dute.
Ez bada &apos;izena&apos; ematen, unean hautatutako elementua erabiliko da.
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
        <translation>renderFont(&quot;izena&quot;, &quot;fitxategi-izena&quot;, &quot;lagina&quot;, tamaina, formatua=&quot;PPM&quot;) -&gt; boolearra

&quot;izena&quot; letra-tipoaren aurrebistako irudia sortzen du emandako &quot;lagina&quot; 
testuarekin eta tamainarekin. &quot;fitxategi-izena&quot; ematen bada (ez badago hutsik) 
irudia fitxategi-izenean gordeko da. Bestela irudiaren datua kate gisa 
itzuliko da. Aukerako &quot;formatua&quot; parametroak sortuko den irudiaren 
formatua zehazten du, eta QPixmap.save() funtzioak onartutako 
edozein formatu izan daiteke. Formatu arruntenak PPM, 
JPEG, PNG eta XPM dira.

NotFoundError abiaraziko da emandako letra-tipoa ez bada aurkitzen.
ValueError abiaraziko da lagin edo fitxategi-izen hutsa ematen bada.
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
        <translation>selectText(hasiera, kopurua, [&quot;izena&quot;])

&quot;kopurua&quot; karaktere hautatzen ditu &quot;izena&quot; izeneko testu-markotik, &quot;hasiera&quot; 
karakteretik hasita. Karaktereak 0tik hasten dira zenbatzen. &quot;kopurua&quot; 0 bada, 
edozein testu-hautapen garbitu egingo da. Ez bada &quot;izena&quot; ematen, unean 
hautatutako elementua erabiliko da.

IndexError abiaraziko da hautapena testuaren mugetatik kanpo balego.
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
        <translation>fileDialog(&quot;epigrafea&quot;, [&quot;iragazkia&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -&gt; katea fitxategi-izenarekin

Fitxategia irekitzeko elkarrizketa-koadroa erakusten du &quot;epigrafea&quot; 
epigrafearekin. &quot;iragazkia&quot; katea fitxategiak iragazteko erabiltzen da. 
Fitxategi-izen edo bide-izen lehenetsia ere eman daiteke, utzi &quot;defaultname&quot; 
katea hutsik erabiltzea nahi ez duzunean. &apos;haspreview&apos; parametroan TRUE (egia) 
balioa jartzen bada fitxategia hautatzeko kutxan aurrebista txikia agertzea 
gaitzen da. &apos;issave&apos; parametroan TRUE (egia) balioa emanez, elkarrizketa-koadroak 
&quot;Gorde honela&quot; leiho baten bezala funtzionatuko du, bestela &quot;Fitxategia irekitzeko 
elkarrizketa-koadroa&quot; bezala. &apos;isdir&apos; parametroan TRUE (egia) ematen denean 
elkarrizketa-koadroak direktorioak soilik bistaratzen eta itzultzen ditu.  Aukerako 
parametro hauen balio lehenetsia FALSE (faltsua) izan oih da.

Iragazkia, zehazten bada, &apos;iruzkina (*.mota *.mota2 ...)&apos; erakoa izaten da. Adibidez, 
&apos;Irudiak (*.png *.xpm *.jpg)&apos;.

Begiratu Qt liburutegien dokumentazioko QFileDialog funtzioa iragazkiei buruzko 
xehetasun gehiagorako.

Adibidea: fileDialog(&apos;Ireki sarrera&apos;, &apos;CSV fitxategiak) 
Adibidea: fileDialog(&apos;Gorde txostena&apos;, defaultname=&apos;report.txt&apos;, issave=True)
</translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation>newStyleDialog() -&gt; katea

&apos;Sortu paragrafo-estilo berria&apos; motako elkarrizketa-koadroa erakusten du. 
Funztioak estiloaren benetako izena itzultzen du, edo None (bat ere ez) erabiltzaileak 
elkarrizketa-koadroa bertan behera uzten duenean.
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Orrialdearen marjinak itzultzen ditu (goia, ezkerra, eskuina, behea) tupla gisa 
uneko unitateetan. Ikus UNIT_&lt;mota&gt; konstanteak eta getPageSize().
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
        <translation type="obsolete">insertText(&quot;testua&quot;, pos, [&quot;izena&quot;])

&quot;testua&quot; testua txertatzen du &quot;izena&quot; testu-markoko &apos;pos&apos; posizioan. 
Testuak UTF kodeketa eduki behar du (ikus setText() erreferentzia gisa). 
Aurreneko karaktereak 0 indizea du. -1 posizioan txertatzean markoari testua 
eransten dio. Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da.

IndexError abiaraziko da mugetatik kanpo txertatzen bada.
</translation>
    </message>
    <message>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation>textOverflows([&quot;izena&quot;, estekaGabe]) -&gt; osokoa

&quot;izena&quot; testu-markoan unean gainezka dauden karaktere-kopurua itzultzen du. 
estekaGabe zero ez den beste balio bat badu, marko bakarra hartuko du, ez 
du testu-markoaren estekatzea erabiliko. Parametro hau gabe estekatutako 
guztietan burutuko du bilaketa.

WrongFrameTypeError abiaraziko da helburuko markoa ez bada testu-marko motakoa.
</translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>setPDFBookmark(&quot;aldatu&quot;, [&quot;izena&quot;])

&quot;izena&quot; izeneko testu-markoa laster-marka den (aldatu = 1) 
edo ez ezartzen du. Ez bada &quot;izena&quot; ematen, unean hautatutako elementua 
erabiliko da.

WrongFrameTypeError abiaraziko da helburuko markoa ez bada testu-marko motakoa.
</translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>isPDFBookmark([&quot;izena&quot;]) -&gt; boolearra


&quot;izena&quot; izeneko testu-markoa PDFko laster-marka bada TRUE itzuliko du.
Ez bada &quot;izena&quot; ematen, unean hautatutako elementua erabiliko da.

WrongFrameTypeError abiaraziko da helburuko markoa ez bada testu-marko motakoa.</translation>
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
        <translation>newDoc(tamaina, marjinak, orientazioa, lehenOrriZbkia,
                   unitatea, aurrezAurre, lehenaEzkerAldean) -&gt; boolearra

ABISUA: funtzio zaharkitua! Erabili newDocument honen ordez.

Dokumentu berria sortzen du eta ongi burutzen bada TRUE (egia) 
itzultzen du. Parametroak honako esanahia dute:

    tamaina = tupla bat (zabalera, altuera) dokumentuaren tamaina adierazteko. 
            PAPER_&lt;mota&gt; izeneko konstanteak erabil ditzakezu, adib. PAPER_A3.

    marjinak = tupla bat (ezkerra, eskuina, goia, behea) dokumentuaren marjinak 
            adierazteko.

    orientazioa = orriaren orientazioa. Konstante erabilgarriak: PORTRAIT (bertikala), 
            LANDSCAPE (horizontala).

    lehenOrriZbkia = dokumentuko aurreneko orrialdearen zenbakia ezartzen du, 
            orrialdeak zenbatzeko erabiltzen da. Arruntena 1 zenbakia erabiltzea 
            izaten den arren, hainbat zatitako dokumentua sortzean zenbaki altuagoak 
            erabiltzen dira.

    unitatea = balio honek dokumentua neurtzeko erabiliko den unitatea ezartzen du. 
            Erabili aurrez definitutako konstanteak: UNIT_INCHES, UNIT_MILLIMETERS, 
            UNIT_PICAS, UNIT_POINTS.

    aurrezAurre = erabili honako konstanteak orrialdeak aurrez aurre jartzeko edo ez: 
            FACINGPAGES, NOFACINGPAGES

    lehenaEzkerAldean = dokumentuko aurreneko orrialdea ezkerrean kokatuko den 
           edo ez: FIRSTPAGELEFT, FIRSTPAGERIGHT

Zabalera, altuera eta marjinen balioak dokumentuari ematen zaion unitateetan 
adierazten dira. PAPER_* konstanteak puntuetan adierazten dira. Zure dokumentua 
ez badago puntuetan neurtuta, hau kontutan eduki beharko duzu.

Adibidea: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
</translation>
    </message>
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation>closeMasterPage()

Unean aktibo dagoen orri maisua ixten du, eta edizioa bere egoera 
normalera itzultzen du. Hasi editatzen editMasterPage() funtzioarekin.
</translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation>masterPageNames()

Dokumentuko orri maisu guztien izenen zerrenda itzultzen du.
</translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation>editMasterPage(orriIzena)

Orri maisua editatzea gaitzen du, eta izendatutako orri maisua 
irekitzen du editatzeko. Amaitu editatzea closeMasterPage() funtzioarekin.
</translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation>createMasterPage(orriIzena)

Orri maisua berria sortzen, &apos;orriIzen&apos; izenekoa, eta ireki egiten du 
editatzeko.
</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(orriIzena)

Izendatutako orri maisua ezabatzen du.
</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(bikoitza)

Dokumentuaren zooma lantzen du interfazeko leiho nagusian. Ekintzek 
20.0, 100.0, e.a. bezalako zenbaki osoa dute. Zooma finkatzeko 
-100 erabiltzen da markatzaile gisa.</translation>
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
        <source>getPageType() -&gt; integer

Returns the type of the Page, 0 means left Page, 1 is a middle Page and 2 is a right Page
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
        <source>&amp;About</source>
        <translation>&amp;Honi buruz</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Laguntzaileak:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Egileak</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Itzultzaileak</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Linean</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Garatzaileak:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Dokumentazio ofiziala:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Bestelako dokumentazioa:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Gune nagusia</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Lineako erreferentzia</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Errore-zuzenketa eta funtzionalitate eskerak</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Posta-zerrenda</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Itzulpen ofizialak eta itzultzaileak:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Aurreko itzultzaileak:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Scribus %1 buruz</translation>
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
        <translation>Ghostscript %1 bertsioa erabiltzen</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ez dago Ghostscript bertsiorik erabilgarri</translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation>&lt;b&gt;Scribus %1 bertsioa&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation>Eraikitze-IDa:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">Panel honek bertsioa, eraikitze data eta Scribus konpilatu den liburutegi 
euskarriak erakusten ditu. C-C-T-F honakoa adierazi nahi du: C=littlecms, 
C=CUPS, T=TIFF eta F=Fontconfig euskarria. Azken hizkia C=cairo edo A=libart 
da.  Falta diren liburutegiak * batekin adierazten dira. Scribus-ek detektatu duen 
Ghostscript-en bertsioa ere adierazten du.
</translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation>Mac OS&amp;#174; X Aqua moldaera:</translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation>Windows&amp;#174; moldaera:</translation>
    </message>
    <message>
        <source>May</source>
        <translation type="obsolete">Izan daiteke</translation>
    </message>
    <message>
        <source>Tango Project Icons:</source>
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">Bai</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ez</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation>Fitxategi-izena:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Bertsioa:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Gaituta:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Argitatze-data:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Azalpena:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Egilea(k):</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright-a:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Lizentzia:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: plugin-ei buruz</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+I</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ireki...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gorde &amp;honela...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>&amp;Leheneratu gordetakora</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Bildu &amp;irteerarako...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Eskuratu testua...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Erantsi &amp;testua...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Lortu irudia...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Gorde &amp;testua...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Gorde orria &amp;EPS gisa...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Gorde P&amp;DF gisa...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokumentuaren &amp;konfigurazioa...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>I&amp;nprimatu...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Irten</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desegin</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Berregin</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Elementuaren ekintza modua</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ebaki</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Itsatsi</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Hautatu &amp;denak</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Desautatu denak</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Bilatu/Ordeztu...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Editatu irudia...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Koloreak...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Paragrafo-estiloak...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Marra-estiloa...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>Orri &amp;maisuak...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Hobespenak...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Bestelakoa...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>E&amp;zkerrean</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>E&amp;rdian</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>E&amp;skuinean</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blokeatu</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>Behar&amp;tuta</translation>
    </message>
    <message>
        <source>&amp;%1 %</source>
        <translation>&amp;%1 %</translation>
    </message>
    <message>
        <source>&amp;Normal</source>
        <translation>&amp;Normala</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Azpimarra</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Azpimarratu &amp;hitzak</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Marratua</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>M&amp;aiuskulak</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>Maiuskula-&amp;txikiak</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>G&amp;oi-indizea</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>A&amp;zpi-indizea</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Itzala</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Irudi-efektuak</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatzaileak...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Bi&amp;koiztu</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Bikoizte &amp;anitza</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>E&amp;lkartu</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Banandu</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>B&amp;lokeatuta dago</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Ta&amp;maina blokeatuta dago</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>Eraman &amp;beheraino</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>Eraman &amp;goraino</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Beheratu</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Goratu</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Bidali berri &amp;laburren albumera</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Atributuak...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>I&amp;rudia ikusgai</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>&amp;Eguneratu irudia</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Doitu markoa irudira</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Irudiaren propietate hedatuak</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>Bereizmen &amp;baxua</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>Bereizmen &amp;normala</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>Bereizmen &amp;osoa</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>PDFko &amp;laster-marka da</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>PDF &amp;oharpena da</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Oharpenaren p&amp;ropietateak</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Eremu-&amp;propietateak</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editatu forma...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>E&amp;rantsi testua bideari</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Desuztartu testua bidetik</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Konbinatu poligonoak</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>&amp;Zatitu poligonoak</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezier kurba</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;Irudi-markoa</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Poligonoa</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Testu-markoa</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Glifoa...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Testu-lagina</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Txertatu</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>I&amp;nportatu...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Ezabatu...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopiatu...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Aldatu lekuz...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>A&amp;plikatu orri maisua...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Kudeatu &amp;gidak...</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Kudeatu orriaren propietateak...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation type="obsolete">&amp;Doitu leihora</translation>
    </message>
    <message>
        <source>&amp;50%</source>
        <translation>%&amp;50</translation>
    </message>
    <message>
        <source>&amp;75%</source>
        <translation>%&amp;75</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>%&amp;100</translation>
    </message>
    <message>
        <source>&amp;200%</source>
        <translation>%&amp;200</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Koadro txikiak</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Erakutsi &amp;marjinak</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Erakutsi &amp;markoak</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Erakutsi &amp;irudiak</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Erakutsi &amp;sareta</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Erakutsi &amp;gidak</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Erakutsi &amp;lerromarrak</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Erakutsi &amp;markoen arteko esteka</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Erakutsi karaktere-kontrola</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Erregelak erlatiboak orriarekiko
</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Atxikitu &amp;saretara</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Atxikitu gi&amp;detara</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Propietateak</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Berri laburren albuma</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Geruzak</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Antolatu orrialdeak</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Laster-markak</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Neurketak</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>Ekintzen &amp;historia</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Egiaztatzailea</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Lerrokatu eta banatu</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>T&amp;resnak</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>P&amp;DF tresnak</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Hautatu elementua</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;aula</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Marra</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>&amp;Pultsuko marra</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Biratu elementua</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Handiagotu edo txikiagotu zooma</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zooma handiagotu</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zooma txikiagotu</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Editatu markoaren edukia</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Editatu testua...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Estekatu testu-markoak</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Desestekatu testu-markoak</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>&amp;Tanta-kontagailua</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopiatu elementu-propietateak</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Editatu testua artikulu-editorearekin</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Txertatu testu-markoa</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Txertatu irudi-markoa</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Txertatu taula</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Txertatu forma</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Txertatu poligonoa</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Txertatu marra</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Txertatu Bezier kurba</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Txertatu pultsuko marra</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Kudeatu irudiak</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Zatitu testua</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Batu testua</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Sortu gaien aurkibidea</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>Scribus-i &amp;buruz</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>&amp;Qt-ri buruz</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>&amp;Argibidea</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scribus-en &amp;eskuliburua...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>&amp;Hitz-zatiketa azkarra</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Marratxo zati ezina</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>&amp;Zuriune zati ezina</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Orrialde-&amp;zenbakia</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Lerro berria</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Markoa haustea</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Zutabe-jauzia</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright-a</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Erregistratutako marka</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Marka</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Buleta</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Em marra</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>En marra</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Irudi-marra</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Kuotazio-marra</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apostrofea</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Komatxo bikoitza</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Bakuna ezkerrean</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Bakuna eskuinean</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Bikoitza ezkerrean</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Bikoitza eskuinean</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Bakuna alderantziz</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Bikoitza alderantziz</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>Ezkerreko komatxoa bakarra</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>Eskuineko komatxoa bakarra</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>Ezkerreko komatxoa bikoitza</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>Eskuineko komatxoa bikoitza</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Koma bakun baxua</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Koma bikoitz baxua</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>CJK ezkerrekoa bakarra</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>CJK eskuinekoa bakarra</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>CJK ezkerrekoa bikoitza</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>CJK eskuinekoa bikoitza</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Txandakatu paletak</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Txandakatu gidak</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Inprimatzeko &amp;aurrebista</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScript-ak...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>Bihurtu orri maisuan...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Teilakatua</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Lauza moduan</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>Plugin-ei &amp;buruz</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Informaziko gehiago...</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Inprimatzea aktibatuta</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>Irauli &amp;horizontalki</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>Irauli &amp;bertikalki</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Erakutsi erregelak</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>&amp;Eskema</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Solidoa</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Puntu erdia</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>En zuriunea</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Em zuriunea</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Zuriune txikia</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Zuriune handia</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Erdiko zuriunea</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Zuriune txikitxoa</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Txertatu hitz-zatitzaile azkarra</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Txertatu marratxo zati ezina</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Txertatu zuriune zati ezina</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Txertatu orrialde-zenbakia</translation>
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
        <translation>E&amp;stiloak...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Eskema</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Eskemak</translation>
    </message>
    <message>
        <source>Paste (&amp;Absolute)</source>
        <translation>Itsatsi (&amp;absolutua)</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Garbitu</translation>
    </message>
    <message>
        <source>Save as &amp;EPS...</source>
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
        <source>Show Text Frame Columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Push Button</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Text Field</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Check Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF Combo Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert PDF List Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Text Annotation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Link Annotation</source>
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
    <name>AlignDistributeBase</name>
    <message>
        <source>Align and Distribute</source>
        <translation type="unfinished">Lerrokatu eta banatu</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Lerrokatu</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation type="unfinished">&amp;Hautatutako gida:</translation>
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
        <translation type="unfinished">Banatu</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation type="unfinished">&amp;Distantzia:</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Lerrokatu eta banatu</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Lerrokatu</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Erlatiboa honi:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Lehena hautatua</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Azkena hautatua</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Orrialdea</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marjinak</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Gida</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Hautapena</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation type="obsolete">Lerrokatutako objektuen eskuineko aldea ainguraren ezkerreko aldearekin</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation type="obsolete">Lerrokatutako objektuen ezkerreko aldea ainguraren eskuineko aldearekin</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Lerrokatu behealdeak</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Lerrokatu eskuinaldeak</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation type="obsolete">Lerrokatutako objektuen goiko aldea ainguraren beheko aldearekin</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Zentratu bertikalki</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Lerrokatu ezkerraldeak</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Zentratu horizontalki</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation type="obsolete">Lerrokatutako objektuen beheko aldea ainguraren goiko aldearekin</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Lerrokatu goialdeak</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Hautatutako gida:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation type="obsolete">Banatu</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation type="obsolete">Berdindu objektuen arteko tarte horizontalak</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation type="obsolete">Berdindu objektuen arteko tarte horizontalak zehaztutako balioarekin</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Banatu eskuineko aldeak distantzia berdinetan</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Banatu beheko aldeak distantzia berdinetan</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Banatu erdialdeak horizontalki distantzia berdinetan</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation type="obsolete">Berdindu objektuen arteko tarte bertikalak</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation type="obsolete">Berdindu objektuen arteko tarte bertikalak zehaztutako balioarekin</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Banatu ezkerreko aldeak distantzia berdinetan</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Banatu erdialdeak bertikalki distantzia berdinetan</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Banatu goiko aldeak distantzia berdinetan</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>&amp;Distantzia:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Banatu elementuak zehaztutako distantziarekin</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Ez da bat ere hautatu</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="obsolete">Objektu batzuk blokeatuta daude.</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="obsolete">&amp;Desblokeatu denak</translation>
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
        <translation>Lerrokatu testua ezkerrean</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Lerrokatu testua eskuinean</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Lerrokatu testua erdian</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Lerrokatu testua justifikatuta</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Lerrokatu testua derrigorrez justifikatuta</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Eremu-propietateak</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Mota:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botoia</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Testu-eremua</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kontrol-laukia</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Konbinazio-koadroa</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Zerrenda-koadroa</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propietateak</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation type="obsolete">Argibidea:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testua</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Letra-tipoa PDF 1.3rekin erabiltzeko:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Ertza</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Kolorea:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Zabalera:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Mehea</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normala</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Zabala</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Estiloa:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Lisoa</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Marratxoduna</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Azpimarra</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Alakatua</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Barnekoa</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Bestelakoak</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Irakurtzeko soilik</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Beharrezkoa</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation type="obsolete">Ez esportatu balioa</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Ikusgaitasuna:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Ikusgai</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Ezkutatuta</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Inprimatzerik ez</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Ikuspegirik ez</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Itxura</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Behera joateko botoiaren testua</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Buelta emateko testua</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikonoak</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Erabili ikonoak</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Kendu</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Sakatuta</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Eman buelta</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Ikonoaren kokalekua...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Nabarmendu</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Alderantzikatu</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Kanpoko ertza</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Bultza</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Marra anizkoitza</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Pasahitza</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Muga:</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Karaktere</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Ez korritu</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Ez egiaztatu ortografia</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Egiaztatu estiloa:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Egiaztatu</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Gurutzea</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamantea</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Zirkulua</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Izarra</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Karratua</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Lehenetsia egiaztatuta dago</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Editagarria</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Aukerak</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Joan</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Bidali inprimakia</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Berrezarri inprimakia</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Inportatu datuak</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Gertaera:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Sagua gora</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Sagua behera</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Sagua sartu</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Sagua irten</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Enfokatuta:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Lausotuta</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script-a:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Editatu...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Bidali URLra:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Bidali datuak HTML gisa</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Inportatu datuak hemendik:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Helburua</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Fitxategira:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Aldatu...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Orrialdea:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X posizioa:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y posizioa:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Ekintza</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Eremuak formatu hau du:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Soila</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Zenbakia</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Ehunekoa</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Ordua</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Pertsonalizatua</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Zenbaki-formatua</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Hamartarrak:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Erabili moneta-ikurra</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Erantsi moneta-ikurra</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatua</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Ehuneko-formatua</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Data-formatua</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Ordu-formatua</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Pertsonalizatuak</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formatua:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Tekla-sakatzea:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formatua</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Balioa balidatu gabe dago</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Balioak honen berdina edo handiagoa izan behar du:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>edo honen berdina edo txikiagoa:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Script-aren balidazio pertsonalizatua:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Balidatu</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Balioa kalkulatu gabe dago</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Balioa hau da:</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>batuketa</translation>
    </message>
    <message>
        <source>product</source>
        <translation>biderketa</translation>
    </message>
    <message>
        <source>average</source>
        <translation>batez bestekoa</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>gutxienekoa</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>gehienezkoa</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>eremu hauena:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Aukeratu...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Script-aren kalkulu pertsonalizatua:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Kalkulatu</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Banderari ez ikusi egin zaio PDF 1.3erako</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Sartu komaz bereiztutako eremuen zerrenda hemen</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Gutxienez &apos;Normala&apos;ren ikonoa behar duzu botoien ikonoak erabiltzeko</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Adibidea:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Hautapen-aldaketa</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF fitxategiak (*.pdf);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Irudiak (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Bat ere ez</translation>
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
        <translation>Oharpen-propietateak</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testua</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Esteka</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Kanpoko esteka</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Kanpoko web esteka</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Helburua</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF dokumentuak (*.pdf);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Mota:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Aldatu...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Orrialdea:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X posizioa</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y posizioa:</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Aplikatu orri maisua</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>Orri &amp;maisua:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Aplikatu honi</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Uneko &amp;orrialdea</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>Orri b&amp;ikoitiak</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>Orri b&amp;akoitiak</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Orrialde &amp;guztiak</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>Barrutiaren &amp;barruan</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>to</source>
        <translation>nora:</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation> Alt+C</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Aplikatu hautatutako orri maisua honako barrutiko orrialde bakoiti, bikoiti edo guztiei</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Bat ere ez</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>&amp;Barra-kode sortzailea...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Postscript Osoko Barra-kode Idazlearen Scribus-eko interfazea</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Errorea fitxategia irekitzean: %1</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 edo 13 digitu</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 digitu</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 edo 12 digitu</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 edo 8 digitu</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 digitu</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 digitu</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation type="obsolete">9 edo 10 digitu marrekin bereiztuta</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Karaktere, digitu eta -. *$/+% ikurren kopuru aldagarria</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>ASCII karaktere- eta funtzio berezien ikur-kopuru aldagarria, 
hasierako karaktere egokiarekin hasiz, hasierako karakterea 
ezartzeko. UCC/EAN-128 kodeak derrigorrez FNC 1 ikurra eduki 
behar du hasierako karaktereari berehala jarraituz.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Digitu eta -$:/.+ABCD ikurretarikoen kopuru aldagarria.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Digitu-kopuru aldagarria. ITF-14 14 karakteretakoa da eta ez du digituen kontrol-baturarik.</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Digitu-kopuru aldakorra</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Digitu eta letra maiuskulen kopuru aldakorra</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Karaktere hamaseitarren kopuru aldakorra</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Barra-kode osatugabea</translation>
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
        <translation>Barra-kode sortzailea</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Barra-kodea</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Mota:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Hautatu barra-kode mota erabilgarria</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Kodearen adierazpen numerikoa. Ikus azpiko laguntzako mezua</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Berrezarri barra-kodearen laginak</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Txertatu testua barra-kodean</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Aktibatzen bada, zenbakiak egon daitezke barra-kodean</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Gorde zuriuneak</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Marraztu geziak kodetik gertu lekua egoteko</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Koloreak</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>A&amp;tzeko planoa</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Atzeko planoko kolorea - kode-marren azpian</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Marrak</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+M</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Barra-kodeko marren kolorea</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Testua</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Testuaren eta zenbakien kolorea</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Argibideak eta laguntze erakusten dira</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Emaitzaren aurrebista. 72 dpi lagina.</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>Ko&amp;dea:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>Txe&amp;rtatu kontrol-batura</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Sortu eta txertatu kontrol-batura barra-kodean</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Txertat&amp;u kontrol-baturaren digitoa</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Txertatu kontrol-baturako digitoa barra-kodeko testuan</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Berri laburren albuma</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation type="obsolete">&amp;Kargatu...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Gorde &amp;honela...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">It&amp;xi</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Fitxategia</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="obsolete">&amp;Aurrebista</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Izena aldatu</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Ezabatu</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objektua</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Izena:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Sarrera berria</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>&quot;%1&quot; izena ez da bakarra. 
Aukeratu beste bat.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Aukeratu berri laburren albumaren direktorioa</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Aukeratu direktorioa</translation>
    </message>
    <message>
        <source>Scrapbook (*.scs)</source>
        <translation>berri laburren albuma (*.scs)</translation>
    </message>
    <message>
        <source>Choose a scrapbook file to import</source>
        <translation>Aukeratu berri laburren albumaren fitxategia inportatzeko</translation>
    </message>
    <message>
        <source>&amp;Import Scrapbook File...</source>
        <translation type="obsolete">&amp;Inportatu berri laburren albumaren fitxategia...</translation>
    </message>
    <message>
        <source>Main</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copied Items</source>
        <translation type="unfinished"></translation>
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
        <source>Create a new scrapbook page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load an existing scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the selected scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import an scrapbook file from Scribus &lt;=1.3.2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close the selected scrapbook</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Laster-markak</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Lekuz aldatu laster-marka</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Txertatu laster-marka</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Laster-markak</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Ikonoaren kokalekua</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Diseinua:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Epigrafea bakarrik</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Ikonoa bakarrik</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Ikonoaren azpiko epigrafea</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Ikonoaren gaineko epigrafea</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Ikonoaren eskuineko epigrafea</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Ikonoaren ezkerreko epigrafea</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Epigrafeak ikonoa gainjartzen du</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Eskalatu:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Beti</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Ikonoa txikiegia denean</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Ikonoa handiegia denean</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Inoiz ez</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Eskalatu horrela:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>proportzionalki</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>ez-proportzionalki</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikonoa</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Berrezarri</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktibatu kolore-kudeaketa</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Sistemaren profilak</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation type="obsolete">Kolore &amp;lisoak:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitorea:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>I&amp;nprimagailua:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Errendatze-modua</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Pertzepziokoa</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Koloremetria erlatiboa</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturazioa</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Koloremetria absolutua</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation type="obsolete">M&amp;onitorea:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation type="obsolete">In&amp;primagailua:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>&amp;Simulatu inprimagailua pantailan</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Nabarmendu &amp;gamatik kanpo dauden koloreak</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Erabili &amp;puntubeltza konpentsazioa</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation type="obsolete">Kolore lehenetsiaren profila orriaren kolore lisoentzako</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Hornitzailetik jasotako, edo zuk sortutako, kolore-profila. 
Profila hau zure monitorearentzako zehaztu beharko litzateke, 
eta ez profila orokorra (adib. sGBU).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Hornitzaileak emandako inprimagilu modeloaren kolore-profila. 
Profila hau zure inprimagailuarentzako zehaztu beharko litzateke, 
eta ez profila orokorra (adib. sGBU).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="obsolete">Errendatze lehenetsia, zure monitoreak erabiltzeko. 
Ez badakizu zergaitik aldatu behar den, Koloremetria erlatiboa edo Pertzepziokoa aukeratu beharko litzateke.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="obsolete">Errendatze lehenetsia, zure inprimagailuak erabiltzeko. 
Ez badakizu zergaitik aldatu behar den, Koloremetria erlatiboa edo Pertzepziokoa aukeratu beharko litzateke.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Gaitu &apos;softare probatzea&apos; koloretako dokumentuak nola inprimatuko 
lizaketeen ikusteko, aukeratutako inprimagailuaren profilaren arabera.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Ongi inprimatuko ez diren koloreak pantailan erakusteko metodoa.
Honek profila oso zehatzak eskatzen ditu eta abisu gisa erabiltzeko da soilik.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Puntubeltza konpentsazioa argazkietako kontrastea hobetzeko 
metodoa da. Gomendagarria da hau gaitzea dokumentuan 
argazkirik egonez gero.</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;GBU irudiak:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMHB irudiak:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Kolore-profil lehenetsia CMHB irudiak inportatzeko</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Kolore-profil lehenetsia GBU irudiak inportatzeko</translation>
    </message>
    <message>
        <source>&amp;RGB Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;CMYK Solid Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pictures:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sol&amp;id Colors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert all colors to printer space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for solid RGB colors on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default color profile for solid CMYK colors on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for solid colors. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for images. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Simulate a full color managed environment :
all colors, rgb or cmyk, are converted to printer color space.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Editatu kolorea</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Izena:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Kolore-&amp;modeloa</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMHB</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>GBU</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation>HTML notazioa</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Berria</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Zaharra</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>ÑSB kolore-mapa</translation>
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
        <translation>H:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation>B:</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Kolore-barra dinamikoak</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Kolore-barra estatikoak</translation>
    </message>
    <message>
        <source>R:</source>
        <translation>G:</translation>
    </message>
    <message>
        <source>G:</source>
        <translation>B:</translation>
    </message>
    <message>
        <source>B:</source>
        <translation>U:</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Laguntzako kolorea</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Kolorea kuatrikomian</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Ezin duzu &apos;%1&apos; izeneko kolorea sortu. 
Izena kolore gardenarentzat gordeta dago</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation type="obsolete">Kolorearen izena ez da bakarra</translation>
    </message>
    <message>
        <source>Choosing this will enable printing this on all plates. Registration colors are used for printer marks such as crop marks, registration marks and the like. These are not typically used in the layout itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choosing this will make this color a spot color, thus creating another spot when creating plates or separations. This is used most often when a logo or other color needs exact representation or cannot be replicated with CMYK inks. Metallic and fluorescent inks are good examples which cannot be easily replicated with CMYK inks.</source>
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
        <translation type="obsolete">Ikusmen normala</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Gorriaren itsutasuna</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Berdearen itsutasuna</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Urdinaren itsutasuna</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Kolore osoen itxutasuna</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="unfinished">Koloreak batzen</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="unfinished">Errorea: </translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="unfinished">%1 kolorea badago lehendik ere.</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="unfinished">%1 kolorea erantsita.</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="unfinished">Kolore-kudeatzailea irekitzen.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="unfinished">Koloreak batzea</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished">Ezin izan da eskatutako kolorea aurkitu. Baliteke beltza, grisa edo 
zuria hautatzea. Ezin da kolore hau prozesatu.</translation>
    </message>
</context>
<context>
    <name>CWDialogBase</name>
    <message>
        <source>Color Wheel</source>
        <translation type="unfinished">Kolore-gurpila</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. Its color model depends on the chosen tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMHB</translation>
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
        <translation type="unfinished">H:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="unfinished">B:</translation>
    </message>
    <message>
        <source>RGB:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">GBU</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="unfinished">G:</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="unfinished">B:</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="unfinished">U:</translation>
    </message>
    <message>
        <source>CMYK:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokumentua</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="unfinished">Hautatu metodo bat kolore-eskema sortzeko. Ikus 
dokumentazioa informazio gehiagorako.</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Angelua:</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished">Hautatutako balioaren eta zenbatutakoaren arteko desberdintasuna. 
Ikus dokumentazioa xehetasun gehiagorako.</translation>
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
        <translation type="unfinished">Batu dokumentuko koloreetan sortutako koloreak</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="unfinished">Ordeztu dokumentuko koloreetan sortutako koloreak</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Utzi</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="unfinished">Utzi koloreak ikutu gabe</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="unfinished">Simulatu ikusmen-akats arruntenak. Hautatu akats mota.</translation>
    </message>
    <message>
        <source>Color Scheme Method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>HSV:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="unfinished">ÑSB</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="unfinished">Ñ:</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="unfinished">S:</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="unfinished">B:</translation>
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
        <translation type="obsolete">Koadro txikia ikus daiteke saguaren eskuineko botoia sakatuta mantantzen bada. &apos;Txertatu&apos; teklak glifo bat txertatzen du azpiko 
hautapenean, eta &apos;Ezabatu&apos; teklak sartutako azkena ezabatzen du.</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Hautatu karakterea:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Letra-tipoa:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Karaktere-klasea:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Txertatu</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Garbitu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">It&amp;xi</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Txertatu karaktereak testuko kurtsorean</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Ezabatu uneko hautapena(k).</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Karaktere-multzo osoa</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Oinarrizko latina</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latina-1 osagarria</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latin hedatua-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latin hedatua-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Puntuazio orokorra</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Goi- eta azpi-indizeak</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Moneta-ikurrak</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Letra-tankerako ikurrak</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Zenbaki-formatuak</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Geziak</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Eragile matematikoak</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Koadro-ertzak</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blokeko elementuak</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Irudi geometrikoak</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Hainbat ikur</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbatak</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Formatu txikiko aldaerak</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Hizki-loturak</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Bereziak</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Greziera</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Greziera hedatua</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Zirilikoa</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Ziriliko gehigarria</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabiera</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabiera hedatua A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabiera hedatua B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebreera</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation type="obsolete">&amp;Txertatu kodea:</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation type="obsolete">Itxi leiho hau eta itzuli testuaren ediziora</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation type="obsolete">Idatzi lau unicode digito balio zuzenean hemen</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished">Koadro txikia ikus daiteke saguaren eskuineko botoia sakatuta mantantzen bada. &apos;Txertatu&apos; teklak glifo bat txertatzen du azpiko 
hautapenean, eta &apos;Ezabatu&apos; teklak sartutako azkena ezabatzen du.</translation>
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
        <translation type="unfinished">Errorea</translation>
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
        <translation type="unfinished">Ezabatu</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation>Uneko profila:</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Elementuak</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Arazoak</translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation>Glifoak falta dira</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Testua gainezka</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objektua ez da orria</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Irudia falta da</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation type="obsolete">Irudiaren DPI balioa %1 dpi baino txikiagoa da</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objektua gardentasuna du</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objektua kokatutako PDFa da</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokumentua</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Ez da arazorik aurkitu</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Orrialdea </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Objektu libreak</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Arazoak aurkitu dira</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Egiaztatzailea</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objektua PDF oharpena edo eremua da</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Ez ikusi egin erroreei</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">Ados</translation>
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
    <message>
        <source>Check again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image resolution below %1 DPI, currently %2 x %3 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image resolution above %1 DPI, currently %2 x %3 DPI</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image is GIF</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Aukeratu estiloak</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Estilo erabilgarriak</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Aukeratu direktorioa</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Biltzen...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Abisua</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Ezin dira fitxategi guztiak bildu irteerako fitxategian: 
%1</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Ezin da fitxategia bildu: 
%1</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Koloreak</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>I&amp;nportatu</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>B&amp;ikoiztu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>E&amp;zabatu</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Kendu erabili gabekoa</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Kolore-multzoak</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Uneko kolore-multzoa:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Gorde kolore-multzoa</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Aukeratu kolore-multzoa kargatzeko</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Gorde uneko kolore-multzoa</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Kendu erabili gabeko koloreak uneko dokumentuko 
kolore-multzotik</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Inportatu koloreak dokumentu batetik uneko multzora</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Sortu kolore berria uneko multzoan</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Editatu unean hautatutako kolorea</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Egin unean hautatutako kolorearen kopia</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Ezabatu unean hautatutako kolorea</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Lehenetsi uneko kolore-multzoa</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Izena:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Aukeratu izena</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumentuak (*.sla *.sla.gz *.scd *.scd.gz);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumentuak (*.sla *.scd);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>%1(r)en kopia</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Kolore berria</translation>
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
        <translation>Monokromatikoa</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Analogoa</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Osagarria</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Zatitu osagarria</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Hirukotea</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Laukotea (osagarri bikoitza)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Oinarri-kolorea</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Argi monokromatikoa</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Ilun monokromatikoa</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>1. analogoa</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>2. analogoa</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>1. banaketa</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>2. banaketa</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>3. banaketa</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>4. banaketa</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>1. hirukotea</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>2. hirukotea</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>1. laukotea (aurkako oinarria)</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>2. laukotea (angelua)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>3. laukotea (aurkako angelua)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Normal Vision</source>
        <translation type="obsolete">Ikusmen normala</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Kolore osoen itxutasuna</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation type="obsolete">Ikusmen-akatsa:</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation type="obsolete">Kolore-gurpila</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Kolorea</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Izena</translation>
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
        <translation type="obsolete">H</translation>
    </message>
    <message>
        <source>K</source>
        <translation type="obsolete">B</translation>
    </message>
    <message>
        <source>Select Method:</source>
        <translation type="obsolete">Hautatu metodoa:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation type="obsolete">Angelua (0 - 90 gradu):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation type="obsolete">&amp;Batu koloreak</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation type="obsolete">&amp;Ordeztu koloreak</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation type="obsolete">Batu dokumentuko koloreetan sortutako koloreak</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation type="obsolete">Ordeztu dokumentuko koloreetan sortutako koloreak</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation type="obsolete">Utzi koloreak ikutu gabe</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation type="obsolete">Koloreak batzen</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation type="obsolete">Errorea: </translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation type="obsolete">Kolore-kudeatzailea irekitzen.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation type="obsolete">Koloreak batzea</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation type="obsolete">&amp;Sortu kolorea...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation type="obsolete">&amp;Inportatu existitzen den kolorea...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation type="obsolete">&amp;Batu koloreak</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation type="obsolete">&amp;Ordeztu koloreak</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation type="obsolete">I&amp;rten</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">K&amp;olorea</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="obsolete">Hautatutako balioaren eta zenbatutakoaren arteko desberdintasuna. 
Ikus dokumentazioa xehetasun gehiagorako.</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation type="obsolete">Egin klik gurpilean oinarri-kolorea lortzeko. Ñabardura da ÑSB 
moduan.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation type="obsolete">Egin kolore-eskemaren lagina</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation type="obsolete">Hautatu metodo bat kolore-eskema sortzeko. Ikus 
dokumentazioa informazio gehiagorako.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation type="obsolete">Aukeratutako kolore-eskemako koloreak</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation type="obsolete">Simulatu ikusmen-akats arruntenak. Hautatu akats mota.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Kolore berria</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="obsolete">Ezin izan da eskatutako kolorea aurkitu. Baliteke beltza, grisa edo 
zuria hautatzea. Ezin da kolore hau prozesatu.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation type="obsolete">K&amp;olore-osagaiak...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Gorriaren itsutasuna</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Berdearen itsutasuna</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Urdinaren itsutasuna</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="obsolete">%1 kolorea badago lehendik ere.</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation type="obsolete">%1 kolorea erantsita.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Kolore-gurpila</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Kolore-ezarpenen laguntzailea</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Kolore-hautatzailea (kolore-teoriarekin).</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Aplikatu</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Utzi</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Ados</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Abisua</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Pertsonalizatua</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Alde batetik</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Bi aldetatik</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3ko tolestura</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4ko tolestura</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Ezker-orrialdean</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="obsolete">Erdian</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="obsolete">Erdi-ezkerrean</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="obsolete">Erdi-eskuinean</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Eskuin-orrialdean</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Bai</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Ez</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Bai</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Ez</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>Monday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tuesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wednesday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thursday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Friday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sunday</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>January</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>February</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>March</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>April</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>May</source>
        <translation type="unfinished">Izan daiteke</translation>
    </message>
    <message>
        <source>June</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>July</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>August</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>September</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>October</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>November</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>December</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page</source>
        <comment>Left page location</comment>
        <translation type="unfinished">Ezker-orrialdean</translation>
    </message>
    <message>
        <source>Middle</source>
        <comment>Middle page location</comment>
        <translation type="unfinished">Erdian</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <comment>Middle Left page location</comment>
        <translation type="unfinished">Erdi-ezkerrean</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <comment>Middle Right page location</comment>
        <translation type="unfinished">Erdi-eskuinean</translation>
    </message>
    <message>
        <source>Right Page</source>
        <comment>Right page location</comment>
        <translation type="unfinished">Eskuin-orrialdean</translation>
    </message>
    <message>
        <source>Normal</source>
        <comment>Default single master page</comment>
        <translation type="unfinished">Normala</translation>
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
        <translation type="unfinished">Ikusmen normala</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <comment>Color Blindness - Red Color Blind</comment>
        <translation type="unfinished">Gorriaren itsutasuna</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <comment>Color Blindness - Greed Color Blind</comment>
        <translation type="unfinished">Berdearen itsutasuna</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <comment>Color Blindness - Blue Color Blind</comment>
        <translation type="unfinished">Urdinaren itsutasuna</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <comment>Color Blindness - Full Color Blindness</comment>
        <translation type="unfinished">Kolore osoen itxutasuna</translation>
    </message>
    <message>
        <source>Custom: </source>
        <comment>Custom Tab Fill Option</comment>
        <translation type="unfinished">Pertsonalizatua: </translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Itzaldura:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opakutasuna</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normala</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Gradiente horizontala</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Gradiente bertikala</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Gradiente diagonala</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Gradiente diagonal-gurutzea</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Gradiente biribila</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Gradiente lineal librea</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Gradiente biribila librea</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation>X1 :</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation>Y1 :</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation>X2 :</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation>Y2 :</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Editatu marraren kolorea</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Editatu kolore-betegarria</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Kolorearen saturazioa</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Betegarriaren gradientea edo normala metodoa</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Ezarri hautatutako kolorearen gardentasuna</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Aldatu bektorea lekuz</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Aldatu gradientearen hasierako bektorea lekuz saguaren ezkerreko 
botoia sakatuz, eta aldatu gradientearen amaierako bektorea saguaren eskuineko botoia sakatuz.</translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Offsets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">H:</translation>
    </message>
    <message>
        <source>Scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X-Scale:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y-Scale:</source>
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
        <source>Blend Mode:</source>
        <translation type="unfinished">Nahaste-modua:</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">ilundu</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Argitu</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Biderkatu</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Pantaila</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Gainjarri</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Argi bizia</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Argi suabea</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Diferentzia</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Esklusioa</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Zuritu</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Belztu</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Ñabardura</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Saturazioa</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Kolorea</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Argitasuna</translation>
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
        <translation type="unfinished">Nora:</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Move &amp;Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="unfinished">E&amp;zabatu</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Ados</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Utzi</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>CSV inportatzailearen aukerak</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Eremu-bereizlea:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Balioaren mugatzailea:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Aurreneko errenkada goiburukoa da</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Bat ere ez</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Inprimagailuaren aukerak</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Orrialde-ezarpena</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Orrialde guztiak</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Orrialde bikoitiak soilik</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Orrialde bakoitiak soilik</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Ispilua</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ez</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Bai</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientazioa</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontala</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>N-Up inprimatzea</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Orrialde orriko</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Orrialde orriko</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Aukerak</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Balioa</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Panel honek CUPSeko hainbat aukera bistaratzen ditu inprimatzean. 
Parametro erabilgarriak inprimagailuaren kontrolatzailearen 
mende daude. CUPS euskarria berretsi dezakezu Laguntza &gt; Honi buruz hautatuz. Begiratu zerrendan: C-C-T zera adierazten du: 
C=CUPS, C=littlecms, T=TIFF euskarria. Falta diren liburutegiak 
* ikurrarekin adierazten dira.</translation>
    </message>
</context>
<context>
    <name>CurveWidget</name>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ireki</translation>
    </message>
    <message>
        <source>Curve Files (*.scu);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Gorde honela</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished">Ezin da fitxategia idatzi: 
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
        <translation>&amp;Konprimitu fitxategia</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Gehitu letra-tipoak</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodeketa:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Zure dokumentuaren historiara joaten da.
Hau hobespenetan ezar dezakezu.</translation>
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
        <translation type="obsolete">Ezarri kolore-osagaiak</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="obsolete">CMHB</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="obsolete">GBU</translation>
    </message>
    <message>
        <source>HSV</source>
        <translation type="obsolete">ÑSB</translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="obsolete">Ñ:</translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="obsolete">S:</translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="obsolete">B:</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="obsolete">G:</translation>
    </message>
    <message>
        <source>G:</source>
        <translation type="obsolete">B:</translation>
    </message>
    <message>
        <source>B:</source>
        <translation type="obsolete">U:</translation>
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
        <translation type="obsolete">H:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="obsolete">B:</translation>
    </message>
    <message>
        <source>Set &amp;RGB</source>
        <translation type="obsolete">Ezarri &amp;GBU</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation type="obsolete">Ezarri &amp;CMHB</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation type="obsolete">Ezarri &amp;ÑSB</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Erabiltzaileak bertan behera utzi du</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Ezabatu kolorea</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Ezabatu kolorea:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Ordeztu honekin:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Ezabatu orrialdeak</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>hona:</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Ezabatu hemendik:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Ezabatu estiloa</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Ezabatu estiloa:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Ordeztu honekin:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Estilorik ez</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Huts egin du inportatzean</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Huts egin du Word dokumentua inportatzean 
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokumentuaren informazioa</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titulua:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Egilea:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Gakoak:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>A&amp;zalpena:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Argitaratzailea:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Laguntzaileak:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Data</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Mota:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formatua:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Identifikatzailea:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>I&amp;turburua:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Hizkuntza:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>E&amp;rlazioa:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>E&amp;staldura:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Es&amp;kubideak:</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>I&amp;nformazio gehiago</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Dokumentua eskuragarri egitearen arduraduna (pertsona edo erakundea)</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Dokumentuaren edukian lagundu duten arduradunak (pertsona edo erakundea)</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Dokumentuaren bizitzaren zikloko gertaera batekin elkartutako data, UUUU-HH-EE formatuan, ISO8601 antzera.</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Dokumentuaren edukiaren generoa edo izaera, adib. kategoriak, funtzioak, generoak, e.a.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Erreferentzia ez anbiguoa emandako ISBN edo URI bezalako testu-inguruan</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Erreferentzia bat dokumentu bati, uneko dokumentua eratortzen dena. Adib. ISBN edo URIa</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Erlazionatutako dokumentu baten erreferentzia, ISBN edo URI bezalako identifikatzaile formala erabiliz.</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Dokumentuko edukiaren hedadura edo esparrua, posible bada kokalekua, 
denbora eta jurisdikzioa sartuz.</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Dokumentuak mantentzen dituen eskubideei buruzko informazioa, adib. copyright-a, patentea edo marka erregistratua</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokumen&amp;tua</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Dokumentuaren edukia egitearen arduraduna den pertsona edo erakundea. Eremu hau Scribus-eko dokumentuan 
kapsula daiteke erreferentziatzeko, edota PDF baten 
metadatuan.</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Dokumentuari emandako izena. Eremu hau Scribus-eko dokumentuan kapsula 
daiteke erreferentziatzeko, edota PDF baten metadatuetan</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Dokumentuaren edukiari buruzkoa. Eremu hau dokumentuaren azalpen laburra edo 
laburpena da. PDFan kapsula daiteke esportatzeko.</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Dokumentuaren edukiaren gaia. Eremu hau dokumentuko gakoak PDFan kapsulatzeko da, bilaketetan laguntzeko edo PDF fitxategiak 
indexatzeko</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Dokumentuaren manifestazio digitala edo fisikoa. MIMEren RFC2045, RFC2046 motak ere erabilgarriak dira.</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Dokumentuaren edukia zein hizkuntzatan idatzita dagoen, arruntena ISO-639 hizkuntza-kodea erabiltzea da, estatuen ISO-3166 kodearekin elkartuz: adib. en-GB, eu-ES</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Orriak zenbatzeko atala gehitzen dio dokumentuari. Atal berria unean hautatutako atalaren atzetik gehituko da.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Ezabatu unean hautatutako atala.</translation>
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
        <translation>&lt;b&gt;Izena:&lt;/b&gt; aukerako atalaren izena, adib. Indizea&lt;br/&gt;&lt;b&gt;Erakutsia:&lt;/b&gt; hautatu orrialdeen zenbakiak atal honetan erakutsea gutxienez testu-marko bat horretarako konfiguratuta balego.&lt;br/&gt;&lt;b&gt;Nondik:&lt;/b&gt; atal honen indize-orriaren hasiera.&lt;br/&gt;&lt;b&gt;Nora:&lt;/b&gt; atal honen  indize-orriaren amaiera&lt;br/&gt;&lt;b&gt;Estiloa:&lt;/b&gt; hautatu  orrialde-zenbakiaren estiloa.&lt;br/&gt;&lt;b&gt;Hasiera:&lt;/b&gt; estiloaren barrutiko indizearen hasiera. Adib. Hasiera=2 eta Estiloa=a,b,c... balira, zenbakia &apos;b&apos; hizkiarekin hasiko litzateke. 
Dokumentuaren aurreneko atalean honek aurreneko orrialdeko 
zenbakia ordezten du fitxategi berriko leihoan.</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Orrialde-zenbakia mugetatik kanpo</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Sartutako balioa orrialde-zenbakien barrutitik kanpo dago uneko dokumentuan (%1 - %2) </translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Dokumentuaren atalak</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena</translation>
    </message>
    <message>
        <source>From</source>
        <translation>Nork</translation>
    </message>
    <message>
        <source>To</source>
        <translation>Nori</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Estiloa</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Hasi</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Erakutsita</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Erlatiboa:</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Honen gurasoa da</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Honen umea da</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Testu-markoa</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Irudi-markoa</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Boolearra</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Osokoa</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Katea</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Zenbaki erreala</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokumentuko elementu-atributuak</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Mota</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Balioa</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parametroa</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Erlazioa</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Erlazioa:</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Autogehitu honi</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Gar&amp;bitu</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+B</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Konfiguratu inprimagailua</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Inprimatze-helburua</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fitxategia</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Aukerak...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Fitxategia:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>A&amp;ldatu...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Bestelako inprimagailuen komandoa</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Ko&amp;mandoa:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Barrutia</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Inprimatu &amp;denak</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Inprimatu uneko &amp;orrialdea</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Inprimatu &amp;barrutia</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Txertatu komaz bereiztutako token-en zerrenda, 
token bat * orrialde guztientzako, 1-5 orrialde barruti 
batentzako, edo orrialde bakar batentzako izan daiteke.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Kopia-kopurua:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Aukerak</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Denak</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyana</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Horia</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Beltza</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Inprimatu</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Inprimatu normal</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Inprimatu bereizleak</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>Inprimatu koloretan</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>Inprimatu gris-eskalan</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>PostScript 1. maila</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>PostScript 2. maila</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>PostScript 3. maila</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Orrialdea</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Ispilatu orria(k) horizontalki</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Ispilatu orria(k) bertikalki</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Ezarri euskarri-tamaina</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Kolorea</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Aplikatu azpikoloreak (UCR)</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Bihurtu kolore laguntzaileak kuatrokomiara</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>Aplikatu ICC profilak</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Aukera aurreratuak</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Aurrebista...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Postscript maila ezartzen du. 
1. edo 2. mailarekin ezartzean fitxategi oso handiak sortzen ditu</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>PostScript fitxategiak (*.ps);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Erabili inprimatzeko beste kudeatzailea, kprinter edo gtklp bezalakoa, inprimatzeko aukera gehigarriak erabiltzeko</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Grisen itzaldura itzaltzeko modu bat. Itzaldura hau cyana, 
horia eta magenta koloreekin osatua dago, beltzaren ordez 
erabiliz. UCR metodoak irudi bateko tonu grisetik gertu dauden 
neutralak edota ilunak diren zatiei eragiten die batiz bat. Honen 
erabilpenak zenbait irudiren inprimazioa hobetzen du, eta kasuz 
kasuko esperimentazioa eta probak egin behar izaten dira. 
UCRek gainsaturazioaren posibilitatea gutxitzen du CMH tintekin.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Kolore lagungarriak kuatrikomiako koloretan bihurtzea gaitzen du. Inprimagailu komertzial batean inprimatzea nahi izanez gero, aukera hau gaituta uztea gomendatzen da.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>ICC profilak kapsulatzea baimentzen zaitu inprimatze-korrontean 
kolore-kudeaketa gaituta dagoenean</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Postscript fitxategiaren euskarriaren tamaina esplizitoki ezartzea gaitzen du. Ez da gomendagarria, erabili soilik zure 
inprimagailuak eskatzen badu.</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Moztu orriaren marjinetara</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Huts egin du inprimagailuaren ezarpenak eskuratzean</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Ez erakutsi objekturik inprimatutako orrien marjinetatik kanpo</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
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
        <translation type="unfinished">Eskuinean:</translation>
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
        <translation type="unfinished">Distantzia orriaren goitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Distantzia orriaren azpitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Distantzia orriaren ezkerretik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Distantzia orriaren eskuinetik kostatzeko</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Fitxategia inportatzea: 
%1 
huts egin du</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Errore larria</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Errorea</translation>
    </message>
    <message>
        <source>Importing PostScript</source>
        <translation type="obsolete">PostScript inportatzen</translation>
    </message>
    <message>
        <source>Analyzing PostScript:</source>
        <translation>PostScript aztertzen:</translation>
    </message>
    <message>
        <source>Generating Items</source>
        <translation>Elementuak sortzen</translation>
    </message>
    <message>
        <source>Converting of %1 images failed!</source>
        <translation>Huts egin du %1 irudiak bihurtzean</translation>
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
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Editatu estiloa</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Izena:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Karakterea</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Marrak:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatzaileak eta koskak</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Paragrafo-estiloaren izena</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Hautatutako testu edo objektuaren letra-tipoa</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Letra-tamaina</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Testuaren kolore-betegarria</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Testuaren trazuaren kolorea</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Letra kapitalen altuera zehazten du, altuera lerrotan neurtzen da.</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Paragrafoaren gaineko tartea</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Paragrafoaren azpiko tartea</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Lerrotartea</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Distantziak</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Lerrotarte finkoa</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Lerrotarte automatikoa</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Lerrokatu lerromarrara</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Letra kapitalak</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Distantzia testutik:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Paragrafo-estiloaren aurrebista</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Letra kapitalen eta testuaren arteko tartea zehazten du</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Paragrafo-estiloko testua txandakatzen du</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Estiloaren izena ez da bakarra</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Atzeko planoa</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation type="obsolete">Hautatu kolore argiko testu estiloak errazago irakurtzeko</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Eskuzko jarraipena</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Desplazatu karaktereen lerromarrara</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation>Egin klik lerro arteko tartearen modua hautatzeko</translation>
    </message>
    <message>
        <source>Select for easier reading of light colored text styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Auto</source>
        <translation type="unfinished">Automatikoa</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Editorea</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Ireki...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gorde &amp;honela...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Gorde eta irten</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Irten gorde gabe</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desegin</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Berregin</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ebaki</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Itsatsi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Garbitu</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Lortu eremu-izenak</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fitxategia</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;Fitxategi guztiak (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Irudien efektuak</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Aukerak:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Kolorea:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Itzaldura:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Distira:</translation>
    </message>
    <message>
        <source>Contrast:</source>
        <translation>Kontrastea:</translation>
    </message>
    <message>
        <source>Radius:</source>
        <translation>Erradioa:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation>Balioa:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posterizatu:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Efektu erabilgarriak</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Lausotu</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Distira</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Koloreztatu</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrastea</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gris-eskala</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Alderantzikatu</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posterizatu</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Enfokatu</translation>
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
        <translation>darabilen efektuak</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
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
        <translation type="unfinished">Fitxategia badago lehendik ere. Gainidatzi?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished">badago lehendik ere. Gainidatzi?</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Denak</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Aukeratu direktorioa esportatzeko</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Esportatu irudi gisa</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Esportatzeko direktorioa:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Aldatu...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Aukerak</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Irudi-&amp;mota:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Kalitatea:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Bereizmena:</translation>
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
        <translation>Barrutia</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Uneko orrialdea</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Orrialde &amp;guztiak</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Barrutia</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Esportatu barrutik orrialdeak</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Txertatu komaz bereiztutako token-en zerrenda, 
token bat * orrialde guztientzako, 1-5 orrialde barruti 
batentzako, edo orrialde bakar batentzako izan daiteke.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Esportatu orrialde guztiak</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Esportatu uneko orrialdea soilik</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Irudien bereizmena 
Erabili 72 dpi irudiak pantailan erabiltzeko badira</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Irudien kalitatea - %100 onena, 
%1 kalitate baxuena</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Esportatzeko formatu erabilgarriak</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Irteerako direktorioa - zure irudiak gordeko diren lekua. 
Esportatutako fitxategiaren izena honelakoa izan daiteke: 
&apos;dokumizena-orrizenbakia.fitxatmota&apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Aldatu irteerako direktorioa</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Irudien tamaina. %100 aldaketarik gabe, %200 bi aldiz handiagoa, e.a.</translation>
    </message>
    <message>
        <source>Image size in Pixels</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Irudi-propietate hedatuak</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normala</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>ilundu</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Argitu</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Ñabardura</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturazioa</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Kolorea</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Argitasuna</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Biderkatu</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Pantaila</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Disolbatu</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Gainjarri</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Argi bizia</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Argi suabea</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Diferentzia</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Esklusioa</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Zuritu</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Belztu</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation type="obsolete">Esklusioa</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Nahaste-modua:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opakutasuna:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Atzeko planoa</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Geruzak</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Ez erabili biderik</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Bideak</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Tamaina:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Izenburua:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Izenbururik ez</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Egilea:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ezezaguna</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokumentua</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Bereizmena:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>DPI</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMHB</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>GBU</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Kolore-eskala:</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gris-eskala</translation>
    </message>
    <message>
        <source>Duotone</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Dokumentuan erabilitako zenbait letra-tipo ordeztu egin dira:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> honekin ordeztuta: </translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Erabilgarri dauden letra-tipoak:</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Letra-tipoen ordezteak</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Bide-izen gehigarriak</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Letra-tipoaren izena</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Letra-tipo erabilgarriak</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Ordeztea</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Letra-tipoen ordezpena</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Aldatu...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Gehitu...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Kendu</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Bide gehigarriak</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Aukeratu direktorioa</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Letra-tipoa</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Erabili letra-tipoa</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="obsolete">Hemen kapsulatua:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Azpimultzoa</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Letra-tipo fitxategiaren bide-izena</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="obsolete">Letra-tipoak aurkitzeko bide-izena Hobespenetan bakarrik 
ezar daiteke, eta soilik dokumenturik irekita ez dauden unean. 
Itxi irekita dauden dokumentuak, gero erabili 
Editatu -&gt; Ezarpenak menua letra-tipoak bilatzeko bide-izena 
aldatzeko.</translation>
    </message>
    <message>
        <source>Embed in PostScript</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font search paths can only be set in File &gt; Preferences, and only when there is no document currently open. Close any open documents, then use File &gt; Preferences &gt; Fonts to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Erabiltzailea</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Sistema</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Erantsi hautatutako letra-tipoa Estiloan, Letra-tipo menua</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Irten aurrebistatik</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Laino guztien azpitik eta sasi guztien gainetik</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Hasi bilaketa</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Hautatutako letra-tipoaren tamaina</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Lagina tekla askatu ondoren erakutsiko da</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Testua hemen idaztean letra-tipoen izenetan bilaketa azkarragoa egiten du. Bilaketan maiuskulak eta minuskulak 
ez dira ezberdintzen. Komodinak (*, ?, [...]) ere eman ditzakezu esaldian. Adibidez, t* esaldiarekin t edo T 
hizkiarekin hasten diren letra-tipo guztiak zerrendatuko dira. 
*bold* esaldiarekin berriz &apos;bold&apos; hitza duten izen guztiak, 
adibidez bolder.</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Letra-tipoen aurrebista</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Bilaketa azkarra:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Bilatu</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Letra-tipoa</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Dok</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Mota</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Azpimultzoa</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Sarbidea</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Letra-&amp;tamaina:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testua</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Testu-lagina bistaratzeko</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>E&amp;zarri</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Berrezarri testua</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Erantsi</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+I</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Letra-tipoen aurrebista...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Letra-tipoen aurrebistaren leihoa</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Letra-tipo erabilgarriak bilatzea, ordenatzea eta arakatzea</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Letra-tipo ordezpena</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Jatorriko letra-tipoa</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Ordezko letra-tipoa</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Egin ordezpen hauek betirako</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Dokumentu honek sisteman instalatuta ez dauden letra-tipo 
batzuk erabiltzen ditu, aukeratu hauentzako ordezko 
egokienak. Bertan behera uzteak dokumentua kargatzea 
geldituko luke.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Letra-tipoak ordeztea bertan behera utzi eta 
dokumentua kargatzea gelditzen du.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Hau gaitzen bada. Scribus-ek ordezpen hauek etorkizuneko diseinu guztietan erabiliko ditu betirako. Hau leheneratzeko 
edo aldatzeko joan Editatu &gt; Hobespenak &gt; Letra-tipoak 
menura.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Ados hautatzen bada gorde egingo du, eta ordezpena betirako izango dira dokumentuan.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Kokalekua:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Add, change or remove color stops here</source>
        <translation>Gehitu, aldatu edo kendu kolore-puntuak hemen</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation type="obsolete">Kudeatu gidak</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation type="obsolete">Gida horizontalak</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="obsolete">&amp;Y posizioa:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Gehitu</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="obsolete">&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation type="obsolete">Gida bertikalak</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="obsolete">&amp;X posizioa:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="obsolete">Ge&amp;hitu</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="obsolete">E&amp;zabatu</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="obsolete">&amp;Blokeatu gidak</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation type="obsolete">Errenkadak eta zutabeak - gida automatikoak</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation type="obsolete">&amp;Errenkadak:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation type="obsolete">&amp;Zutabeak:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation type="obsolete">Errenkada &amp;tartea</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation type="obsolete">Zutabe-ta&amp;rtea</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation type="obsolete">Ikus:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Orrialdea</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation type="obsolete">&amp;Marginak</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation type="obsolete">&amp;Hautapena</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">It&amp;xi</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation type="obsolete">&amp;Eguneratu</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation type="obsolete">Ezarri gidak dokumentuan. Gida-kudeatzailea irekita egon arren, aldaketak betirako dira</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation type="obsolete">&amp;Aplikatu orrialde guztiei</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="obsolete">Gida</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="obsolete">Unitatea</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Aurrebista</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation type="obsolete">Badago gida hutsa (0, 0) lehendik ere</translation>
    </message>
    <message>
        <source>Edit Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter a position:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Guide</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GuideManagerBase</name>
    <message>
        <source>Manage Guides</source>
        <translation type="unfinished">Kudeatu gidak</translation>
    </message>
    <message>
        <source>&amp;Single</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontals</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished">Gida</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="unfinished">&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>Verticals</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="unfinished">Ge&amp;hitu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="unfinished">E&amp;zabatu</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="unfinished">&amp;Blokeatu gidak</translation>
    </message>
    <message>
        <source>Appl&amp;y to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Column/Row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Number:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>U&amp;se Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+B</translation>
    </message>
    <message>
        <source>Refer To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>M&amp;argins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;election</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Nu&amp;mber:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use &amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished">Alt+G</translation>
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
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Ez dago eskulibururik erabilgarri. Ikus honako gunea dokumentazio eguneratuak bilatzeko: http://docs.scribus.net
eta www.scribus.net deskarga burutzeko.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Edukia</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Esteka</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Scribus, lineako laguntza</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Edukia</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Bilatu</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>B&amp;ilatu</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>&amp;Ezabatu denak</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>Laster-&amp;markak</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Inprimatu...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>I&amp;rten</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Bilaketak ez ditu maiuskulak/minuskulak ezberdintzen</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Bilatu</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Bilatu hitza:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Laster-marka berria</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Laster-markaren titulu berria:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fitxategia</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Bilatu...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>Bilatu &amp;hurrengoa</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>Bilatu &amp;aurrekoa</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>&amp;Gehitu laster-marka</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>E&amp;zabatu denak</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Laster-markak</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Egokitasuna</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Hitz-zatiketa egin daiteke</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Onartu</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Saltatu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Hizkuntza:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Hitz &amp;txikiena:</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Hitz txikienaren luzera hitz-zatiketa egiteko.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Jarraian egin daitekeen hitz-zatiketen gehienezko kopurua. 
0 balioak mugarik gabeko hitz-zatiketa adierazten du.</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;Hitz-zatiketaren iradokizuna</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Zatitu testua automatikoki &amp;idaztean</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Hitz bakoitzaren zatiketa aukerak erakusten dituen elkarrizketa-koadroa bistaratuko da Hainbat menuko Zatitu testua erabiltzean.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Testua idazten den bitartean hitz-zatiketa automatikoki egitea gaitzen du.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>&amp;Baimendutako hitz-zatiketa jarraiak:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Irudiaren datuak</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Informazio orokorra</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Data / denbora:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>Profila kapsulatua du:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Bai</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ez</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Profil-izena:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>Bide kapsulatuak ditu:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Geruzak ditu:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF datuak</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Artista:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Iruzkina:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Erabiltzailearen iruzkina:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kamera modeloa:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Kamera hornitzailea:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Azalpena:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Copyright-a:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Eskaner modeloa:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Eskaner hornitzailea:</translation>
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
        <translation>Inportatu &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>Inportatu EPS fitxategiak</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>EPS fitxategi gehieneak inporta ditzake uneko dokumentura, 
beraien bektore-datuak Scribus objektuetan bihurtuz.</translation>
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
        <translation>Txertatu orrialdea</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Orrialdea(k)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>orrialdearen aurretik:</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>orrialdearen ondoren:</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>Amaieran</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Txertatu</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Orri maisuak</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>Orri &amp;maisuak:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Orri-tamaina</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntazioa:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontala</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Zabalera:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altuera:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Aldatu objektuak beraien orrialdeekin lekuz</translation>
    </message>
</context>
<context>
    <name>InsertAFrame</name>
    <message>
        <source>&lt;b&gt;Insert a text frame&lt;/b&gt;&lt;br/&gt;A text frame allows you to enter any text in a defined position with the formatting you choose. You may select a text file on the Options tab if you want to immediately import a document into the frame. Scribus supports a wide variety of importable format from plain text to OpenOffice.org.&lt;br/&gt;Your text may be edited and formatted on the page directly or in the simple Story Editor.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Insert an image frame&lt;/b&gt;&lt;br/&gt;An image frame allows you to place an image onto your page. Various image effects may be applied or combined including transparencies, brightness, posterisation that allow retouching or the creation of interesting visual results. Image scaling and shaping is performed with the Properties Palette.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ireki</translation>
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
        <translation type="unfinished">&amp;Testu-markoa</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished">&amp;Irudi-markoa</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation type="unfinished">T&amp;aula</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Poligonoa</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">H:</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Tamaina</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Zabalera:</translation>
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
        <translation type="unfinished">Alt+B</translation>
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
        <source>Link Created Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Ados</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Utzi</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="unfinished">Orrialde guztiak</translation>
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
        <translation>Txertatu taula</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Errenkada-kopurua:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Zutabe-kopurua:</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Editatu JavaScript</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Editatu...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Gehitu...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>Script &amp;berria:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Script berria</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ez</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Bai</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Ziur zaude script hau ezabatzea nahi duzula?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Script berri bat gehitzen du, izen berdineko funtzioa aurredefinitzen du. Script hau &apos;Ekintza irekia&apos; bezala erabiltzea nahi baduzu ziurtatu zaitez funtzioaren izena 
ez duzula aldatzen.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation type="obsolete">Ekintza</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation type="obsolete">Uneko tekla</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation type="obsolete">Hautatu tekla bat ekintza honetarako</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="obsolete">Gakorik e&amp;z</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="obsolete">&amp;Erabiltzaileak definitutako gakoa</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="obsolete">Alt+Maius+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="obsolete">Ezarri &amp;gakoa</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation type="obsolete">Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="obsolete">Ktrl</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="obsolete">Maius</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="obsolete">Maius+</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation type="obsolete">Alt+</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation type="obsolete">Ktrl+</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="obsolete">Laster-tekla multzo kargagarria</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="obsolete">&amp;Kargatu</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="obsolete">&amp;Inportatu...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="obsolete">&amp;Esportatu...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="obsolete">&amp;Berrezarri</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="obsolete">Laster-tekla multzoa eskuragarri dago kargatzeko</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="obsolete">Kargatu hautatutako laster-tekla multzoa</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="obsolete">Inportatu laster-tekla multzoa uneko konfiguraziora</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="obsolete">Esportatu uneko laster-teklak fitxategi inportagarri batera</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="obsolete">Birkargatu Scribus-eko laster-tekla lehenetsiak</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="obsolete">Tekla multzoaren XML fitxategiak (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="obsolete">Tekla-sekuentzia hau jadanik erabiltzen dago</translation>
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
        <translation>Geruzak</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Ezabatu geruza</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Geruzako objektu guztiak ere ezabatzea nahi duzu?</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Gehitu geruza berria</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Ezabatu geruza</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Igo geruza</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Jaitsi geruza</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Nahaste-modua:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normala</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">ilundu</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Argitu</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Biderkatu</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Pantaila</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Gainjarri</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Argi bizia</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Argi suabea</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Diferentzia</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Esklusioa</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Zuritu</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Belztu</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Ñabardura</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Saturazioa</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Kolorea</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation type="unfinished">Argitasuna</translation>
    </message>
    <message>
        <source>Duplicates the current layer</source>
        <translation type="unfinished"></translation>
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
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Editatu marra-estiloak</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>B&amp;ikoiztu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>E&amp;zabatu</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>%1(r)en kopia</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Estilo berria</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ez</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Bai</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumentuak (*.sla *.sla.gz *.scd *.scd.gz);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentuak (*.sla *.scd);; Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Inportatu</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Ziur zaude estilo hau ezabatzea nahi duzula?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>LineStyleWBase</source>
        <translation>Marra-estilo oinarria</translation>
    </message>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Lerro-zabalera:</translation>
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
        <translation>Txano laua</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Txano karratua</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Txano biribila</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Punta-elkartzea</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Alaka-elkartzea</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Biribila-elkartzea</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="obsolete">Marra solidoa</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="obsolete">Marratxoduna</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="obsolete">Puntukatua</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="obsolete">Marratxoak eta puntuak</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="obsolete">Marratxoa puntua puntua</translation>
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
        <translation>Fitxategi guztiak (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Hautatu Lorem Ipsum</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Egilea:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Lortu gehiago:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML fitxategia:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Lorem Ipsum</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Paragrafoak:</translation>
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
        <source>Manage Page Properties</source>
        <translation>Kudeatu orri-propietateak</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Orri-tamaina</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntazioa:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontala</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Zabalera:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altuera:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Aldatu objektuak lekuz beraien orriekin</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Mota:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marginen gidak</translation>
    </message>
    <message>
        <source>Other Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Behean:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Goian:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Eskuinean:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Ezkerrean:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Goiko marjinaren gida eta orriaren ertzaren arteko 
distantzia</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Beheko marjinaren gida eta orriaren ertzaren arteko 
distantzia</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Barruan:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Kanpoan:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Diseinu aurredefinituak:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="obsolete">Aplikatu marjinen ezarpenak orrialde guztiei</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Aplikatu marjinen aldaketak dokumentuan dauden orrialde guztiei</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Ezkerreko marjinaren gida eta orriaren ertzaren arteko 
distantzia. Aurrez aurreko orrialdeak hautatuta egonez gero, 
marjinaren tartea erabil daiteke loturetan marjin 
egokiak lortzeko.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Eskuineko marjinaren gida eta orriaren ertzaren arteko 
distantzia. Aurrez aurreko orrialdeak hautatuta egonez gero, 
marjinaren tartea erabil daiteke loturetan marjin 
egokiak lortzeko.</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Inprimagailuaren marjinak...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Inportatu marjinak inprimagailu erabilgarrietatik 
(hautatutako orri-tamainarentzako).</translation>
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
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Distantzia orriaren goitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Distantzia orriaren azpitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Distantzia orriaren ezkerretik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Distantzia orriaren eskuinetik kostatzeko</translation>
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
        <translation type="unfinished">Eskuinean:</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Editatu orri maisuak</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Ziur zaude orri maisua ezabatzea nahi duzula?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ez</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Bai</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Izena:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Orri maisu berria</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">%1(r)en kopia</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Orri maisu berria</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopia: %1 / </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Bikoiztu hautatutako orri maisua</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Ezabatu hautatutako orri maisua</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Gehitu orri maisu berria</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Inportatu orri maisuak beste dokumentutik</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>%1 orri maisu berri</translation>
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
        <translation>Kopia bat baino gehiago</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>Kopia-&amp;kopurua:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Despl. &amp;horizontala:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Despl. &amp;bertikala:</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Distantziak</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation>X1 :</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation>Y1 :</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation>X2 :</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation>Y2 :</translation>
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
        <translation>Angelua:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Luzera:</translation>
    </message>
    <message>
        <source>pt</source>
        <translation>ptu</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Page(s)</source>
        <translation>Inportatu orria(k)</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> 0tik</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Sortu orria(k)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumentuak (*.sla *.sla.gz *.scd *.scd.gz);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentuak (*.sla *.scd);; Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> %1(e)tik</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Dokumentutik:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>A&amp;ldatu...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Inportatu orria(k):</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Orriaren aurretik</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Orriaren atzetik</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Amaieran</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Inportatu</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Inportatu orri maisua</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Inportatu orri maisua</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Txertatu komaz bereiztutako token-en zerrenda, 
token bat * orrialde guztientzako, 1-5 orrialde barruti 
batentzako, edo orrialde bakar batentzako izan daiteke.</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Letra-tipoa falta da</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>%1 letra-tipoa ez dago instalatuta.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Erabili</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>horren ordez</translation>
    </message>
</context>
<context>
    <name>ModeToolBar</name>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Tresnak</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="unfinished">Propietateak...</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Lekuz aldatu orrialdeak</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopiatu orria</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Lekuz aldatu orrialdea(k):</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Aldatu orria(k) lekuz</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Orriaren aurrera</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Orriaren atzera</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Amaierara</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Nora:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Kopia-kopurua:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Propietateak</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Testua</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Irudia</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Marra</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Koloreak</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometria</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X posizioa:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y posizioa:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Zabalera:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altuera:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Biraketa:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Oinarri-puntua:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Maila</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forma:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editatu forma...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>&amp;Biribidu
ertzak:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distantzia testutik</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Zu&amp;tabeak:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">T&amp;artea:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Goian:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Behean:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Ezkerrean:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Eskuinean:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>Ta&amp;bulatzaileak...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Testu-bideen propietateak</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Erakutsi kurba</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Desplazamendua:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distantzia kurbatik:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="obsolete">Testua markoaren inguruan</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Erabili &amp;muga-koadroa</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Erabili ingerada-marrak</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>Est&amp;iloa:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation type="obsolete">&amp;Hizkuntza:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Eskalatze &amp;librea</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;X eskala:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Y eskala:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Eskalatu markoaren &amp;tamainara</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proportzionala</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Sarrera-profila:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Errendatze-modua:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Pertzepziokoa</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Koloremetria erlatiboa</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturazioa</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Koloremetria absolutua</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Ezkerreko puntua</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Amaierako puntuak</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Oinarri-puntua:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Marra-&amp;mota:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Marraren &amp;zabalera:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Puntuko lotura</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Alakako lotura</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Lotura biribila</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>E&amp;rtzak:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Txano laua</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Txano karratua</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Txano biribila</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Amaierakoak:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Estilorik ez</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Gelaxken marrak</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Marra goian</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Marra ezkerrean</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Marra eskuinean</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Marra behean</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Hautatutako objektuaren izena</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Uneko oinarri-puntuaren posizio horizontala</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Uneko oinarri-puntuaren posizio bertikala</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Zabalera</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Altuera</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Objektuaren biraketa uneko oinarri-puntuan</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Puntua (neurketak edo biraketako angeluak lantzeko)</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Hautatu goiko ezkerreko oinarri-puntua</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Hautatu goiko eskuineko oinarri-puntua</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Hautatu beheko ezkerreko oinarri-puntua</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Hautatu beheko eskuineko oinarri-puntua</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Hautatu erdiko oinarri-puntua</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Irauli horizontalki</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Irauli bertikalki</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Eraman maila bat gora</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Eraman maila bat behera</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Eraman goraino</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Eraman beheraino</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Objektua kokatuta dagoen maila adierazten du, 0 balioak objektua maila beherenean dagoela 
adierazten du.</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Blokeatu edo desblokeatu objektua</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Blokeatu edo desblokeatu objektuaren tamaina</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Gaitu edo desgaitu objektua inprimatzea</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="obsolete">Jarri marko baxuenetako testua objektuaren formaren inguruan</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Erabili inguratze-kutxa testuarentzako, markoaren formaren ordez</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Erabili bigarren marra, jatorrian markoaren forman oinarritutakoa, testu-jarioarentzat</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Hautatutako testu edo objektuaren letra-tipoa</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Letra-tamaina</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Karaktereen zabalera eskalatzea</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Testu-trazuaren kolorearen saturazioa</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Testuaren kolorearen saturazioa</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Lerroen arteko tartea</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Uneko paragrafoaren estiloa</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="obsolete">Markoaren hitz-zatiketaren hizkuntza</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Aldatu ezker edo amaierako puntuen ezarpenak</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Marra-eredua</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Marraren lodiera</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Marra-lotura motak</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Marraren amaiera mota</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Uneko objektuaren marra-estiloa</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Aukeratu markoaren forma...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Editatu markoaren forma...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Ezarri izkineko biribiltze-erradioa</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Testu-markoko zutabe-kopurua</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Tarte eta zubatearen zabalera artean aldatzen du</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Zutabe arteko distantzia</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Testuaren distantzia markoaren goitik</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Testuaren distantzia markoaren behetik</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Testuaren distantzia markoaren ezkerretik</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Testuaren distantzia markoaren eskuinetik</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Editatu testu-markoaren tabulatzailea...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Baimendu irudiak markoaren tamaina ezberdina edukitzea</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Irudiaren desplazamendu horizontala markoan</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Irudiaren desplazamendu bertikala markoan</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Aldatu irudiaren tamaina horizontalki</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Aldatu irudiaren tamaina bertikalki</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Mantendu X eta Y, berdin eskalatuz</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Mantendu itxuraren erlazioa</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Doitu irudia markoaren tamainara</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Erabili irudiaren proportzioak, markoarenak baino</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Irudiaren profilaren iturburua</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Irudia errendatzearen saiakera</translation>
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
        <source>Column width</source>
        <translation>Zutabe-zabalera</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Eskuinetik ezkerrerako idazkera</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Hasierako gezia:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Amaierako gezia:</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Lerrotarte finkatua</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Lerrotarte automatikoa</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Lerrokatu lerromarrara</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>Uneko X-dpi:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Uneko Y-dpi:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Desplazatu karaktereen lerromarrara</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Karaktereen altuera eskalatzea</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Eskuzko jarraipena</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>&quot;%1&quot; izena ez da bakarra. Aukeratu beste bat.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Erregela osoa</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Bikoitia/Bakoitia</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Ez zero</translation>
    </message>
    <message>
        <source>Color of text stroke and/or drop shadow, depending which is chosen.If both are chosen, then they share the same color.</source>
        <translation>Testuaren trazua edota itzalaren kolorea, aukeratzen denaren arabera. 
Biak aukeratzen badira, kolore berdina partekatuko dute.</translation>
    </message>
    <message>
        <source>Color of selected text. If Outline text decoration is enabled, this color will be the fill color. If Drop Shadow Text is enabled, then this will be the top most color.</source>
        <translation>Hautatutako testuaren kolorea. Testu-dekorazioaren eskema gaitzen bada, kolore hau kolore-betegarria izango da. Testua itzaldurarekin gaitzen bada, honek kolore gorena edukiko du.</translation>
    </message>
    <message>
        <source>Click to select the line spacing mode</source>
        <translation type="obsolete">Egin klik lerro arteko tartearen modua hautatzeko</translation>
    </message>
    <message>
        <source>Transparency Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">E&amp;lkartu</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation type="unfinished">Nahaste-modua:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normala</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation type="unfinished">ilundu</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation type="unfinished">Argitu</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation type="unfinished">Biderkatu</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation type="unfinished">Pantaila</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation type="unfinished">Gainjarri</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation type="unfinished">Argi bizia</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation type="unfinished">Argi suabea</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation type="unfinished">Diferentzia</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation type="unfinished">Esklusioa</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation type="unfinished">Zuritu</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation type="unfinished">Belztu</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation type="unfinished">Ñabardura</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Kolorea</translation>
    </message>
    <message>
        <source>Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Zabalera:</translation>
    </message>
    <message>
        <source>Text &amp;Flow Around Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Desgaituta</translation>
    </message>
    <message>
        <source>Use Frame &amp;Shape</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Effects</source>
        <translation type="unfinished">Irudien efektuak</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overprinting</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Knockout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overprint</source>
        <translation type="unfinished"></translation>
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
        <source>Auto</source>
        <translation type="unfinished">Automatikoa</translation>
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
        <translation>Editatu estiloa</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Txano laua</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Txano karratua</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Txano biribila</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Punta-elkartzea</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Alaka-elkartzea</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Angelu-elkartzea</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Lerro-zabalera:</translation>
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
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Marra jarraitua</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Marratxoduna</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Puntukatua</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Marratxoak eta puntuak</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Marra puntua puntua</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>&quot;%1&quot; ez da bakarra.&lt;/br&gt;Aukeratu beste bat.</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Progresioa</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Progresio osoa:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Utzi</translation>
    </message>
</context>
<context>
    <name>MultipleDuplicate</name>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished">Despl. &amp;horizontala:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">Despl. &amp;bertikala:</translation>
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
        <translation type="unfinished">Kopia bat baino gehiago</translation>
    </message>
    <message>
        <source>&amp;By Number of Copies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation type="unfinished">Kopia-&amp;kopurua:</translation>
    </message>
    <message>
        <source>&amp;Shift Created Items By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+B</translation>
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
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished">Despl. &amp;horizontala:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">Despl. &amp;bertikala:</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Ados</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Utzi</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Nire &amp;plugin-a</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - nire plugin-a</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Plugin-ak funtzionatzen du!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Dokumentu berria</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Orrialde-tamaina</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientazioa:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Horizontala</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Zabalera:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altuera:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Marjinen gidak</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Aukerak</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Unitate lehenetsia:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Testu-marko &amp;automatikoak</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Tartea:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Zu&amp;tabeak:</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Dokumentuaren orri-tamaina, tamaina estandarra edo pertsonalizatua</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Dokumentuko orrien orientazioa</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentuko orrien zabalera, editagarria orriaren tamaina pertsonalizatua 
hautatu baduzu.</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Dokumentuko orrien altuera, editagarria orriaren tamaina pertsonalizatua 
hautatu baduzu.</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Neurketaren unitate lehenetsia dokumentua editatzeko</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Sortu testu-markoak automatikoki orrialde berriak gehitzean</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Zutabe-kopurua automatikoki sortutako testu-markoetan sortzeko</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Automatikoki sortutako zutabeen arteko distantzia</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Ez erakutsi elkarrizketa-koadro hau aurrerantzean</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Dokumentuaren orrialdeen hasierako kopurua</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;Orrialdeak:</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>Dokumentu &amp;berria</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Ireki &amp;existitzen den dokumentua</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Ireki azken &amp;dokumentua</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation type="unfinished">Aurreneko orrialdea:</translation>
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
        <translation>Berria &amp;txantiloitik...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Kargatu dokumentuak aurrez definitutako diseinuarekin</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Hasi dokumentua beste erabiltzaile batek, edo zeuk, sortutako 
txantiloitik (adib. estilo konstanteko dokumentuentzako).</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nodoak</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Koordenatu absolutuak</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X posizioa:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y posizioa:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Editatu ingerada-marrak</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Berrezarri ingerada-marrak</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Amaitu editatzea</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Lekuz aldatu nodoak</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Lekuz aldatu kontrol-puntuak</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Gehitu nodoak</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Ezabatu nodoak</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Mugitu kontrol-puntuak bakarka</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Mugitu kontrol-puntuak simetrikoki</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Berrezarri kontrol-puntuak</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Berrezarri kontrol-puntu hau</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Ireki poligonoa edo moztu Bezier kurba</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Itxi Bezier kurba</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Ispilatu bidea horizontalki</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Ispilatu bidea bertikalki</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Zizailatu bidea horizontalki eskuinera</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Zizailatu bidea horizontalki ezkerrera</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Zizailatu bidea bertikalki gora</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Zizailatu bidea bertikalki behera</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Biratu bidea erlojuaren aurkako norantzan</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Biratu bidea erlojuaren norantzan</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Handitu bidearen tamaina % eremukoarekin</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Biraketa-angelua</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktibatu ingerada-marrak editatzeko modua</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Berrezarri ingerada-marrak markoaren jatorrizko formara</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Egiaztatuta dagoenean orrialdearekiko korrdenatu erlatiboak erabiltzen ditu, bestela koordenatuak objektuarekiko erlatiboak dira.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Txikitu bidearen tamaina % eremukoarekin</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Bidearen tamaina txikitzen du erakutsitako balioarekin</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Bidearen tamaina handitzen du erakutsitako balioarekin</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>% balioa handitzeko edo txikitzeko</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Balioa handitzeko edo txikitzeko</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Ez dirudi dokumentu hau OpenOffice.org Draw fitxategia denik.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Inportatu &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>OpenOffice.org Draw-eko marrazki-fitxategiak inportatzen ditu</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>OpenOffice.org Draw fitxategi gehienak inportatzen ditu 
uneko dokumentuan, beraien bektoreen datuak Scribus-eko 
objektuetan bihurtuz.</translation>
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
        <translation>OpenDocument inportatzailearen aukerak</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Gainidatzi paragrafo-estiloak</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Hau gaituz dauden estiloak gainidatziko dira uneko Scribus dokumentuan</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Batu paragrafo-estiloak</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Batu paragrafo-estiloak atributuen arabera. Honen ondorioz, paragrafo-estilo 
kopurua gutxiagotu egingo da, baina estiloen atributuak mantenduko ditu, nahiz 
eta jatorrizko dokumentuko estiloei ezberdin deitu.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Erabili dokumentuaren izena aurrizki gisa paragrafo-estiloetan</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Jarri aurretik dokumentuaren izena paragrafo-estiloaren izenari Scribus-en.</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ez galdetu berriro</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Jarri ezarpen hauek lehenetsi gisa, eta ez galdetu berriro OASIS OpenDocument 
inportatzean.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus dokumentua</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x dokumentua</translation>
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
        <translation type="unfinished">Tamaina</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Zabalera:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation type="unfinished">Luzera:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Angelua:</translation>
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
        <translation>Marra-zabalera</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Gorde PDF gisa</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Irteerako fitxategia:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Al&amp;datu...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Gorde fitxategi batean &amp;orrialdeko</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF fitxategiak (*.pdf);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Honek dokumentuko orrialde bakoitza PDF fitxategi bakoitzean esportatzea gaitzen du. 
Orrialde-zenbakiak automatikoki gehitzen dira. Oso erabilgarria da PDFak inprimazio 
komertzialetako.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Gorde botoia desgaitu egingo da PDF/X-3 formatura esportatzen saiatzean, eta informazioaren katea galdu egingo da PDF/X-3 fitxatik.</translation>
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
        <translation type="unfinished">PDF tresnak</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>PDF gordetzen</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation type="obsolete">Orri maisuak esportatzen:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="obsolete">Orrialdeak esportatzen:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Uneko orrialdeko elementuak esportatzen:</translation>
    </message>
    <message>
        <source>Exporting Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="unfinished">Orrialdea:</translation>
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
        <translation>Inprimatzeko aurrebista</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="obsolete">&amp;Testua antialias-ekin</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="obsolete">&amp;Grafikoak antialias-ekin</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Bistaratu &amp;gardentasuna</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Azpikoloreak (UCR)</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Bistaratu CMHB</translation>
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
        <translation>&amp;H</translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation>&amp;B</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Erakutsi gardentasuna eta elementu gardenak dokumentuan. Ghostscript 7.07 edo 
berriagoa behar da.</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Inprimatzeko aurrebista ematen du CMHB tinta orokorren simulazioa landuz , BGU koloreak erabili ordez</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Gaitu/desgaitu C (cyana) tinta</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Gaitu/desgaitu M (magenta) tinta</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Gaitu/desgaitu H (horia) tinta</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Gaitu/desgaitu B (beltza) tinta</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Denak</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Banatze-izena</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyana</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Horia</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Beltza</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Eskalatzea:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Inprimatu...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Testu-elementuen ikuspegi goxoagoa eskaintzen du ikustailean, 
aurrebistaren prestaketa motelduz. Type 1 motako letra-tipoei 
eragiten die soilik</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">TrueType eta OpenType letra-tipoen, EPS, PDF eta grafiko bektorialen ikuspegi goxoagoa eskaintzen du ikuspegian, 
aurrebistaren prestaketa motelduz.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Grisen itzaldura itzaltzeko modu bat. Itzaldura hau cyana, 
horia eta magenta koloreekin osatua dago, beltzaren ordez 
erabiliz. UCR metodoak irudi bateko tonu grisetik gertu dauden 
neutralak edota ilunak diren zatiei eragiten die batiz bat. Honen 
erabilpenak zenbait irudiren inprimazioa hobetzen du, eta kasuz 
kasuko esperimentazioa eta probak egin behar izaten dira. 
UCRek gainsaturazioaren posibilitatea gutxitzen du CMH tintekin.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Aldatu orriaren eskalaren tamaina.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Itxi</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fitxategia</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation type="unfinished">Bistaratze-ezarpenak</translation>
    </message>
    <message>
        <source>Enable &amp;Antialiasing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished">Ispilatu orria(k) horizontalki</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished">Ispilatu orria(k) bertikalki</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">Moztu orriaren marjinetara</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished">Inprimatu gris-eskalan</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert Spot Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished">Aplikatu ICC profilak</translation>
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
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished">Kolore lagungarriak kuatrikomiako koloretan bihurtzea gaitzen du. Inprimagailu komertzial batean inprimatzea nahi izanez gero, aukera hau gaituta uztea gomendatzen da.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished">ICC profilak kapsulatzea baimentzen zaitu inprimatze-korrontean 
kolore-kudeaketa gaituta dagoenean</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation type="obsolete">Orri maisuak prozesatzen:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="obsolete">Esportatzeko orrialdeak:</translation>
    </message>
    <message>
        <source>Processing Master Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Page:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Propietateak</translation>
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
        <translation type="unfinished">Letra kapitalak</translation>
    </message>
    <message>
        <source>Parent&apos;s Drop Cap Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabulatzaileak eta koskak</translation>
    </message>
    <message>
        <source>Ch&amp;aracter Style</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Irudia</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testua</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Lerroa</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Poligonoa</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Koska-marra</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Trazua</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Honen kopia</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>Relates To</source>
        <translation>Erlatiboa:</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Honen gurasoa da</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Honen umea da</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Bat ere ez</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Orriko elementu-atributuak</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Mota</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Balioa</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parametroa</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Erlazioa</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Erlazioa:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Gar&amp;bitu</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Ados</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Utzi</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Orriaren diseinua</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Aurreneko orrialdea:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Bi aldetatik</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Erdi-eskuinean</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Arrastatu orrialdeak edo orri maisuak zakarrontzira ezabatzeko</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Hemen daude orri maisu guztiak. Orri berria sortzeko, arrastatu orri maisua azpiko orri-ikuspegira</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Antolatu orrialdeak</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Orri maisu erabilgarriak:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Dokumentuaren orrialdeak:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>%1 of %2</source>
        <translation>%1 / %2</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 / %1</translation>
    </message>
</context>
<context>
    <name>PageSize</name>
    <message>
        <source>Quarto</source>
        <translation type="obsolete">Laurdena</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="obsolete">Folioa</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Gutuna</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation type="obsolete">Gutun ofiziala</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legala</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Liburu nagusia</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Exekutiboa</translation>
    </message>
    <message>
        <source>Post</source>
        <translation type="obsolete">Posta</translation>
    </message>
    <message>
        <source>Crown</source>
        <translation type="obsolete">Koroa</translation>
    </message>
    <message>
        <source>Large Post</source>
        <translation type="obsolete">Large Post</translation>
    </message>
    <message>
        <source>Demy</source>
        <translation type="obsolete">Demy</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Tartekoa</translation>
    </message>
    <message>
        <source>Royal</source>
        <translation type="obsolete">Royal</translation>
    </message>
    <message>
        <source>Elephant</source>
        <translation type="obsolete">Elephant</translation>
    </message>
    <message>
        <source>Double Demy</source>
        <translation type="obsolete">Demy bikoitza</translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation type="obsolete">Demy laukoitza</translation>
    </message>
    <message>
        <source>STMT</source>
        <translation type="obsolete">STMT</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="obsolete">A</translation>
    </message>
    <message>
        <source>B</source>
        <translation type="obsolete">B</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="obsolete">C</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="obsolete">D</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="obsolete">E</translation>
    </message>
</context>
<context>
    <name>PatternDialog</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Aukeratu direktorioa</translation>
    </message>
    <message>
        <source>Loading Patterns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ireki</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation type="obsolete">Emaitza</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="obsolete">Bilaketaren emaitzak: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Aurrebista</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="obsolete">Hautatu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Utzi</translation>
    </message>
</context>
<context>
    <name>PicSearchBase</name>
    <message>
        <source>Result</source>
        <translation type="unfinished">Emaitza</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="unfinished">Bilaketaren emaitzak: </translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="unfinished">&amp;Aurrebista</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Select</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished">Alt+B</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Name</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Bide-izena</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Orrialdea</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Inprimatu</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Egoera</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Joan</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Bai</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Falta da</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Bilatu</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Utzi bilaketa</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Kudeatu irudiak</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus - irudi bilaketa</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Bilaketak huts egin du: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Ez da &quot;%1&quot; izeneko irudirik aurkitu.</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Hautatu oinarri-direktorioa bilaketarako</translation>
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
        <source>Search Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Itxi</translation>
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
        <translation>Gorde &amp;irudi gisa...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Esportatu irudi gisa</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Esportatu hautatutako orrialdeak bitmap irudi gisa.</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="unfinished">Gorde irudi gisa</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished">Errorea irteerako fitxategia(k) idaztean.</translation>
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
        <translation>Ezin da plugin-a aurkitu</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>errore ezezaguna</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Ezin da ikurra aurkitu (%1)</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Plugin-a: %1 kargatzen</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>hasieratzeak huts egin du</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>plugin mota ezezaguna</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>Plugin-a: %1 kargatuta</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>Plugin-a: huts egin du %1 kargatzean: %2</translation>
    </message>
    <message>
        <source>There is a problem loading %1 of %2 plugins. %3 This is probably caused by some kind of dependency issue or old plugins existing in your install directory. If you clean out your install directory and reinstall and this still occurs, please report it on bugs.scribus.net.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin: %1 initialized ok </source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Plugin: %1 failed post initialization</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>Plugin-kudeatzailea</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Plugin-a</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Nola exekutatu</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Mota</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Kargatu?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>Plugin-aren IDa</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fitxategia</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Bai</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ez</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Aplikazioa berrabiarazi behar da aldaketak eragina izateko.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Poligonoaren propietateak</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Iz&amp;kinak:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Biraketa:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Aplikatu &amp;faktorea:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Faktorea:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Poligonoen izkina-kopurua</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Poligonoak biratzeko graduak</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplikatu ganbila/ahurra faktorea poligonoen forma aldatzeko.</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Poligono-lagina</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Balio negatiboak poligonoa ahurra (edo izar formakoa) izatea eragiten du, balio positiboak berriz ganbila.</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Hobespenak</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Orokorra</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokumentua</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Gidak</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografia</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Tresnak</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Bistaratu</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="obsolete">GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="obsolete">&amp;Gaia:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="obsolete">&amp;Gurpil-jauzia:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="obsolete">&amp;Azken dokumentuak:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="obsolete">Bide-izenak</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="obsolete">&amp;Dokumentuak:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="obsolete">&amp;Aldatu...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="obsolete">&amp;ICC profilak:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="obsolete">A&amp;ldatu...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="obsolete">&amp;Script-ak:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="obsolete">Al&amp;datu...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Orrialde-tamaina</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Horizontala</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntazioa:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Zabalera:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Altuera:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Marjinen gidak</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Behean:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Goian:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Eskuinean:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Ezkerrean:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Gorde automatikoki</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Bitartea:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">Bistaratu area inprima ezina marjineko kolorearekin</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="obsolete">&amp;Doitu pantailaren tamaina</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="obsolete">&amp;Exekutagarria:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="obsolete">&amp;Testua antialias-ekin</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="obsolete">&amp;Grafikoak antialias-ekin</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="obsolete">Irudia lantzeko tresna</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="obsolete">Exe&amp;kutagarria:</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="obsolete">Menu eta leihoen letra-tamaina lehenetsia</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="obsolete">Neurtzeko unitate lehenetsia dokumentuaren edizioan</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="obsolete">Scribus-ek korrituko dituen lerro-kopurua 
saguaren gurpileko jauzi bakoitzeko</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="obsolete">Azken aldian editatutako dokumentu-kopurua 
Fitxategia menuan erakusteko</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="obsolete">Dokumentuen direktorio lehenetsia</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="obsolete">Script-en direktorio lehenetsia</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="obsolete">Orri-tamaina lehenetsia, tamaina estandarra edo pertsonalizatua</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="obsolete">Dokumentuko orrialdeen orientazio lehenetsia</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Dokumentuko orrialdeen zabalera, editagarria da 
orrialdeen tamaina pertsonalizatua aukeratuz gero</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Dokumentuko orrialdeen altuera, editagarria da 
orrialdeen tamaina pertsonalizatua aukeratuz gero</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="obsolete">Denbora-tartea automatikoki gordetzeko</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Paperaren kolorea</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Maskaratu marjinen kanpoko area marjin-koloreekin</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="obsolete">Ezarri zoomaren maila lehenetsia</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="obsolete">Testua antialias-ekin EPSak eta PDFak pantailan errendatzeko</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="obsolete">Grafikoak antialias-ekin EPSak eta PDFak pantailan errendatzeko</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Aukeratu direktorioa</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">Al&amp;datu...</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="obsolete">&amp;Hizkuntza:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="obsolete">Unitateak:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="obsolete">Desegin/Berregin</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="obsolete">Ekintzen historiaren luzeera</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Hitz-zatitzailea</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Letra-tipoak</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Kolore-kudeaketa</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF esportatzea</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Laster-teklak</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Orri-ikuspegia</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Kolorea:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Erakutsi irudiak</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Erakutsi markoen arteko esteka</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Erakutsi markoak</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="obsolete">Laneko area</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Plugin-ak</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">Markoak bistaratzea aktibatu edo desaktibatu</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Irudiak bistaratzea aktibatu edo desaktibatu</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation type="obsolete">&amp;Txantiloiak:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Egiaztatzailea</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokumentuko elementu-atributuak</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Gaien aurkibidea eta Indizeak</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Erakutsi testu-kontroleko karaktereak</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Erregelak erlatiboak orriarekiko
</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Orrialdeen arteko tartea</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Horizontala:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Bertikala:</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="obsolete">Pantaila doitzeko arrastatu azpiko erregela graduatzailearekin</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation type="obsolete">dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="obsolete">Bereizmena:</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Kanpoko tresnak</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation type="obsolete">Galdetu letra-tipoak aldatu aurretik dokumentua kargatzean</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="obsolete">Uneko paragrafo-estiloaren aurrebista estiloak editatzean</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="obsolete">Erakutsi elkarrizketa-koadroa abiaratzean</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="obsolete">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="obsolete">Erabili beti Lorem Ipsum estandarra</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="obsolete">Paragrafo-kopurua:</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Hainbat</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Bistaratu inprima ezin diren karaktereak, paragrafo-markatzaileak bezalakoak, testu-markoetan</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="obsolete">Txantiloien direktorio gehigarria</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="obsolete">Kokatu erregela pantailan eta arrastatu graduatzailea zoomaren 
maila ezartzeko, honela Scribus-ek orrialdeak eta hauen objektuak 
tamaina egokiarekin bistaratuko ditu</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Dokumentuko oihalaren ezkerreko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Dokumentuko oihalaren eskuineko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Dokumentuko oihalaren gaineko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="obsolete">Dokumentuko oihalaren azpiko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation type="obsolete">Kokatu Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="obsolete">Kokatu irudi-editorea</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="obsolete">PostScript interpretatzailea</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="obsolete">Gaitu edo desgaitu estekatutako markoak bistaratzea.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="obsolete">Hautatu hizkuntza lehenetsia Scribus-ek erabiltzeko. Utzi hau hutsik
inguruneko aldagaien arabera aukeratzeko. Erabilera hau 
gainezartzeko komando-lerroko aukera erabil dezakezu Scribus abiaraztean</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="obsolete">Letra-tamaina (&amp;menuak):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="obsolete">Letra-tamaina (&amp;paletak):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="obsolete">Aukeratu leihoko dekorazio eta itxura lehenetsia. Scribus aplikazioak 
KDEren edo Qt gai erabilgarriak jasotzen ditu, baldin eta Qt 
konfiguratuta badago KDEko osagaiak bilatzeko.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="obsolete">Tresnen leihoen letra-tamaina lehentesia</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="obsolete">ICC profilen direktorio lehentesia. Hau ezin da irekitako dokumetu 
batekin aldatu. Lehenetsi gisa, Scribus-ek sistemako direktorioetan 
begiratuko du Mac OSX eta Windows sistemetan. GNU/Linux eta Unix sistemetan $home/.color/icc,/usr/share/color/icc 
eta /usr/local/share/color/icc direktorioetan begiratuko du.</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="obsolete">Gaituta dagoenean Scribus-ek zure fitxategiaren babeskopia bat gordeko du &apos;.bak&apos; luzaperarekin denbora bat iragatzen den 
bakoitzean</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Ekintzen historiaren luzera ezartzen du urratsetan. 0 balioak 
ekintzen kopuru infinitua gorde egingo dela ezartzen du.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="obsolete">Editore grafikoaren kokalekua sisteman. Gimp erabiltzen bada, eta 
banaketak ekartzen badu, &apos;gimp-remote&apos; erabiltzea aholkatzen da. 
Honela, irudi bat exekutatzen ari den gimp-en instantzian edita 
daiteke.</translation>
    </message>
    <message>
        <source>Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.</source>
        <translation type="obsolete">Gehitu Ghostscript interpretatzailearen bide-izena. Windows 
sisteman &apos;gswin32c.exe&apos; izeneko programa erabili behar duzu, eta 
INOLA ERE EZ &apos;gswin32.exe&apos;. Bestela Scribus abiatzean ordenagailua 
blokea baitaiteke.</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">Berri laburren albuma</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Inprimagailua</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Lehenetsiak</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation type="obsolete">Gorde...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Gorde hobespenak</translation>
    </message>
    <message>
        <source>Export...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Aplikatu</translation>
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
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Migrate Old Scribus Settings?</source>
        <translation>Migratu Scribus zaharreko ezarpenak?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus 1.2 bertsioko hobespeneko fitxategiak aurkitu dira. 
Nahi duzu hauek migratzea Scribus-en bertsio berrira?</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Ezin izan da hobespenen &apos;%1&apos; fitxategia ireki idazteko: %2</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Huts egin du hobespenen &apos;%1&apos; fitxategian idaztean: QIODevice-ren 
egoerako %2 kodea</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Huts egin du hobespenen &apos;%1&apos; fitxategia irekitzean: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Huts egin du hobespenen &apos;%1&apos; XML irakurtzean: 
%2 %3. lerroan, %4. zutabea</translation>
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
        <translation>Errorea hobespenak idaztean</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus-ek ezin izan du bere hobespenak gorde: 
&lt;br&gt;%1 
&lt;br&gt;Egiaztatu fitxategi eta direktorioko baimenak eta disko 
gogorreko leku erabilgarria.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Errorea hobespenak kargatzean</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus-ek ezin izan du bere hobespenak kargatu: 
&lt;br&gt;%1 
&lt;br&gt;Ezarpen lehenetsiak kargatuko dira.
</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>Magazine</source>
        <translation>Aldizkaria</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Urrezko izena</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Bederatziak bat</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Aurrez definitutako orri-diseinua hauta dezakezu. &apos;Bat ere ez&apos; aukerak marjinak dauden 
bezala uzten ditu, &apos;Gutenberg&apos; aukera marjin klasikoak ezartzen ditu. &apos;Aldizkaria&apos;k berriz 
marjina guztiak balio berdinarekin uzten ditu.</translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Bat ere ez</translation>
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
        <translation>&amp;Ireki...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gorde &amp;honela...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>I&amp;rten</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fitxategia</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>E&amp;xekutatu</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>Exekutatu &amp;kontsolan gisa</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>&amp;Gorde irteera...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script-a</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribus-eko Python-en kontsola</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Python-en kontsola estandarretik datorrenez, zenbait murriztapen ditu, zuriuneen kasuak bederen. Irakurri Scribus-eko eskuliburua informazio gehiagorako.</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Script kontsola</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Idatzi komandoak. Hautapena script gisa landuko da</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Script-aren irteera</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python-en script-ak (*.py)</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Gorde Python-en komandoak fitxategian</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Abisua</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Testu fitxategiak (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Gorde uneko irteera</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Ireki Python script fitxategia</translation>
    </message>
    <message>
        <source>Line: %1 Column: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>&amp;Ñab.:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sat.:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Bal.:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Gorria:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Berdea:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Urdina:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>&amp;Alfa kanala:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Oinarrizko koloreak</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Kolore &amp;pertsonalizatuak</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definitu kolore pertsonalizatuak &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Gehitu kolore pertsonalizatuei</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Hautatu kolorea</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopiatu edo mugitu fitxategira</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Irakurri: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Idatzi: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Fitxategi-&amp;izena:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Fitxategi-&amp;mota:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Direktorio bat gora</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Fitxategi denak (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaina</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Mota</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atributuak</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>&amp;Begiratu:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Atzera</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Sortu karpeta berria</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Zerrenda-ikuspegia</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Ikuspegi xehea</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Aurrikusi datuak</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Aurrikusi edukia</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Irakurtzeko/Idazteko</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Irakurtzeko soilik</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Idazteko soilik</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Eskuragarri ezina</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Estekatu fitxategiari</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Estekatu direktorioari</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Estekatu bereziari</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fitxategia</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Direktorioa</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Berezia</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Ireki</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>Izena &amp;aldatu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Birkargatu</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Ordenatu i&amp;zenaren arabera</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Ordenatu &amp;tamainaren arabera</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Ordenatu &amp;dataren arabera</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>Ordenatu &amp;gabe</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Ordenatu</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Erakutsi &amp;ezkutuko fitxategiak</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>fitxategia</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>direktorioa</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>esteka</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Ezabatu %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ziur zaude %1 &quot;%2&quot; ezabatzea nahi duzula?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Bai</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ez</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Karpeta berri 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Karpeta berria</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>%1 karpeta berri</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Bilatu direktorioa</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Direktorioak</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Gorde</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Errorea</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1 
Fitxategia ez da aurkitu. 
Egiaztatu bide-izena eta fitxategi-izena.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Fitxategi guztiak (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Hautatu direktorioa</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Direktorioa:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Letra-tipoa</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>Letra-&amp;estiloa</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaina</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efektuak</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>&amp;Marratua</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Azpimarratua</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Kolorea</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Lagina</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Scr&amp;ipt-a</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplikatu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Itxi</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Hautatu letra-tipoa</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Garbitu</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Hautatu dena</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desegin</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Berregin</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ebaki</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Itsatsi</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Antolatu</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Pertsonalizatu...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Qt buruz&lt;/h3&gt; 
&lt;p&gt;Programa honek Qt %1 bertsioa erabiltzen du.&lt;/p&gt; 
&lt;p&gt;Qt plataforma anitzetako C++ tresna-multzoa da GUI aplikazioak 
garatzeko.&lt;/p&gt; 
&lt;p&gt;Qt liburutegiak iturburu bakarreko aplikazioak MS&amp;nbsp;Windows, Mac&amp;nbsp;OSX, GNU/Linux eta Unix-ean oinarritutako hainbat sistema komertzialetara bihurtzea eskaintzen du. &lt;br&gt;Qt kapsulatutako gailuetan ere erabil daiteke.&lt;/p&gt; 
&lt;p&gt;Qt liburutegia Trolltech produktua da. Ikus &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; informazio gehiagorako.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation>Testua inportatzen</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Onartutako formatu guztiak</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Hasieratzen...</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Atzeko planoa</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Abisua</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Benetan gainidatzi nahi duzu fitxategia:
%1?</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Berripaperrak</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Liburuxkak</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalogoak</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Esku-orriak</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Kartelak</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Txartela</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Idazpuruak</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Gutun-azalak</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Bisita-txartela</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Egutegiak</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Publizitateak</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiketak</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menuak</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programak</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF inprimakiak</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF aurkezpenak</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Aldizkariak</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Posterrak</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Iragarkiak</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Testu-dokumentuak</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Diptikoak</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Txantiloi propioak</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="obsolete">Gorde irudi gisa</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="obsolete">Errorea irteerako fitxategia(k) idaztean.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="obsolete">Ongi esportatu da.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="obsolete">Fitxategia badago lehendik ere. Gainidatzi?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="obsolete">badago lehendik ere. Gainidatzi?</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ez</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Bai</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="obsolete">Bai guztiei</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Onartutako formatu guztiak (*.eps, *EPS, *.ps, *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG irudiak (*.svg *.svgz);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG irudiak (*.svg);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Komaz bereiztutako balioen fitxategiak</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CSV datuak</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>CSV goiburukoa</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML fitxategiak</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Kanpoko estekak</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Testu-fitxategiak</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>%1 letra-tipoa hondatuta dago, baztertu egingo da</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Testu-iragazkiak</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Euskarri-kasuak</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albaniera</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Euskara</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgariera</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation type="obsolete">Brasildarra</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalana</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Txinera</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Txekiera</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Daniera</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Nederlandera</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Ingelesa</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Ingelesa (Britaniarra)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperantoa</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemana</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandiera</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Frantsesa</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galiziera</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grekoa</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hungariera</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesiera</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiera</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreera</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituaniera</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation type="obsolete">Norvegiera (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norvegiera (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norvegiera</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Poloniera</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Errusiera</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Suediera</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Gaztelera</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Gaztelera (Latina)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovakiera</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Esloveniera</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbiera</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da letra-tamaina lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da letra-tipoa lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testu-tamaina lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da zutabe-kopurua lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da lerrotartea lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da zutabe-tartea lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testua lortu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testua ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testua txertatu testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Lerrokadura barrutitik kanpo. Erabili Scribus-eko ALIGN* konstanteetariko bat.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Hautapenaren indizea mugetatik kanpo</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Unitatea barrutitik kanpo. Erabili Scribus-eko UNIT_* konstanteetariko bat.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Helburukoa ez da irudi-markoa.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Izkinaren erradioa zenbaki positiboa izan behar da.</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Ezin da izen hutseko kolorerik lortu.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Ezin da izen hutseko kolorerik aldatu.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Ezin da izen hutseko kolorerik sortu.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Ezin da izen hutseko kolorerik ezabatu.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Ezin da izen hutseko kolorerik ordeztu.</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer dokumentuak</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Ez da kolorea aurkitu - python errorea</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Konfigurazio pertsonalizatua (aukerakoa): </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Konfigurazio estandarra:</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Laburdurak prozesatzen. Itxaron...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Laburdurak prozesatzen. Eginda.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaansa</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turkiera</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukraniera</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Galesa</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Fitxategi-izena katea izan behar da.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Ezin dira irudi-motaren ezarpenak ezabatu.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Irudi-mota katea izan behar da.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>&apos;motaGuztiak&apos; atributua IRAKURTZEKO SOILIK da.</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Huts egin du irudia esportatzean</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Ezin da %0 eskalatu.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Zehaztutako elementua ez da irudi-markoa.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Ez da letra-tipoa aurkitu.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Ezin da lagina huts bat errendatu.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Ezin da izen hutseko geruzarik eduki.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Ez da geruza aurkitu.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Ezin da azken geruza kendu.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Ezin da izenik gabeko geruza sortu.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Txertatzeko indizea mugetatik kanpo.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testuaren lerrokadura ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Letra-tamaina mugetatik kanpo - 1 &lt;= tamaina &lt;= 512 beharko luke izan.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da letra-tamaina ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da letra ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Lerrotartea mugetatik kanpo, &gt;=0.1 baino handiagoa izan behar du.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da lerrotartea ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Zutabe-tartea mugetatik kanpo, positiboa izan behar du.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da zutabe-tartea ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Zutabe-kopurua mugetatik kanpo, 1 &gt; izan behar du.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da zutabe-kopurua ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Ezin da testurik hautatu testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testua ezabatu testu-markoa ez den batetik.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testuaren kolorea ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testuaren trazua ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da testuaren itzaldura ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Testu-markoak soilik esteka ditzake.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Helburuko markoa hutsik egon behar du.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Helburuko markoak beste marko bati estekatzen dio.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Helburuako markoari beste marko batek estekatzen dio.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Iturburukoa eta helburukoa objektu berdinak dira.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin zaio esteka kendu testu-markoa ez dan bati.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Ezin da eskematara bihurtu testu-markoa ez den bat.</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Orrialdea</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Scribus, garapeneko bertsioa</translation>
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
        <translation>ptu</translation>
    </message>
    <message>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation>atz</translation>
    </message>
    <message>
        <source>p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Puntuak (ptu)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Hazbete (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Pika (p)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Fitxategia badago</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Ordeztu...</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Denak</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Huts egin du dokumentua irekitzean. 
</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Huts egin du dokumentua gordetzean.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argumentua orrialdeko elementuaren izena izan behar du, edo PyCObject instantzia</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Ez da propietatea aurkitu</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Ez da umea aurkitu</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Ezin izan da &apos;%1&apos; emaitz-mota bihurtu.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>&apos;%1&apos; propietate-mota ez dago onartuta</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Ezin izan da &apos;%1&apos; bihurtu &apos;%2&apos; propietate-motara</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Motak bat datoz, baina huts egin du propietatea ezartzean.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Ezin da pixmapa gorde</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Badago objektu bat lehendik ere eskatutako izenarekin.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Puntuen zerrendak gutxienez bi puntu eduki behar ditu (lau balio).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Puntuen zerrendak balio-kopuru bikoita eduki behar du.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Puntuen zerrendak gutxienez hiru puntu (sei balio) eduki behar ditu.</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Puntuen zerrendak gutxienez lau puntu (zortzi balio) eduki behar ditu.</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Puntuen zerrendak sei balioen multiploa eduki behar du.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objektua ez da aurkitu.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Estiloa ez da aurkitu.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ezin da estiloa ezarri testu-markoa ez den batean.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Huts egin du EPS gordetzean.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Orri-kopurua barrutitik kanpo.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argumentua ez da zerrenda: balio mugikorren zerrenda izan behar du.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argumentuak balio ez-numerikoak ditu: balio mugikorren zerrenda izan behar du.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argumentuak balio ez-numerikoak ditu: balio mugikorren zerrenda izan behar du.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Marraren zabalera mugetatik kanpo dago, 0 &lt;= zabalera &lt;= 12 izan behar du.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Marraren itzaldura mugetatik kanpo dago, 0 &lt;= zabalera &lt;= 100 izan behar du.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Itzaldura mugetatik kanpo dago, 0 &lt;= zabalera &lt;= 100 izan behar du.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Marra-estiloa ez da aurkitu.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objektua ez da estekatutako testu-markoa, ezin zaio esteka kendu.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objektua serie bateko azken markoa da, ezin zaio esteka kendu. Horren ordez, 
kendu aurreko markoaren esteka.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Testu-markoak soilik egiazta daitezke gainezkazean</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Exekutatu script-a...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Erakutsi &amp;kontsola</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>Script-ari &amp;buruz...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>&amp;Script-a</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scribus-eko script-a</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Azken script-ak</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Script-ari buruz</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroaziera</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugesa</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugesa (BR)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus kraskadura</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus kraskatu egin da %1. seinaleagatik</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Ados</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Orri maisua </translation>
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
        <translation>Ezin izan da irteerako %1 fitxategia ireki</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Irteerako korrontea ez da idazkorra</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Huts egin du ezarpenak egiaztatzean: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Ezin izan da sarrerako %1 fitxategia ireki</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Ezin da XML ezarpenak irakurri:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (%2. lerroa,  %3. zutabea)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Ezin da XML ezarpenak irakurri: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>erroko nodo nulua</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; baliogabea</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>%1 &lt;%2&gt; nodo aurkituak, 1 behar da.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>ustegabeko &lt;%2&gt; nodo nulua</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;%1&gt; nodoa ez da elementu bat</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;%1&gt; elementuak ez du &apos;balioa&apos; atributua</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;%1&gt; elementuaren balioa &apos;true&apos; edo &apos;false&apos; izan behar du</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;lpiSettingsEntry&gt; elementuak ez du `izena&apos; atributua</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Freetype2 liburutegia ez dago erabilgarri</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation type="obsolete">%1 letra-tipoa hautsita dago, ez kapsulatuta</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>%1 letra-tipoa hondatuta dago (korrontea irakurtzean), ez dago kapsulatua</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation type="obsolete">%1 letra-tipoa hondatuta dago (FreeType2), baztertu egingo da</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>%1 letra-tipoa hondatuta dago (no Face), baztertu egingo da</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>%1 letra-tipoak %2 glifoa hondatua du (%3 kodea)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation type="obsolete">%1 letra-tipoa hondatuta dago eta baztertu egingo da</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>%1 letra-tipoa ezin da irakurri, ez eta kapsulatu ere</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Huts egin du %1 letra-tipoa kargatzean . letra-tipo ezezaguna</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>%1 leta-tipoa %2(e)ndik (%3) kargatuta</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>%1 (%2) letra-tipoa %3(r)en bikoitza da</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>%1 letra-tipoa kargatzen (fontconfig erabiltzen aurkitua)</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Huts egin du letra-tipoa kargatzean - freetype2 ezin izan da letra-fitxategian aurkitu</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation type="obsolete">%1 letra-tipoa hondatuta dago (FreeType), baztertu egingo da</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation type="obsolete">%1 letra-tipoak baliogabeko %2 (%3 kodea) glifoa du, baztertu egingo da</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>%1 kanporatzen %2 letra-tipotik (despl.=%3, nTaula=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>memcpy goiburukoa: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>&apos;%1&apos; taula</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>memcpy taula: %1 %2%3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy despl.: %1 %2 %3</translation>
    </message>
    <message>
        <source> cm</source>
        <translation> zm</translation>
    </message>
    <message>
        <source> c</source>
        <translation> z</translation>
    </message>
    <message>
        <source>cm</source>
        <translation>zm</translation>
    </message>
    <message>
        <source>c</source>
        <translation>z</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Milimetro (mm)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Zentimetro (zm)</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>orria</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Dokumentu-txantiloia: </translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Kolorea ez da aurkitu.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Kolorea ez da dokumentuan aurkitu.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Kolorea ez da aurkitu kolore lehenetsietan.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Ezin da elkartu bi elementu baino gutxiago</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Ezin da elkartu bi elementu baino gutxiago</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Hautapena edo elementuen zerrendaren argumentua behar da elkartzeko</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Ezin da laster-markarik ezarri testu-markoa ez den batean</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Ezin da informaziorik lortu testu-markoa ez den batetik</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Fitxategi-izena ezin da kate huts izan.</translation>
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
        <translation>Scribus-eko Python interfazeko moduloa

Modulo hau Scribus-en Python interfazea da. Scribus kontrolatzeko eta oihaleko 
objektuak kontrolatzeko funtzioak eskaintzen ditu. Funtzio bakoitza azpian dokumentatuta 
dago. 

Gauza gutxi batzuk komunak dira interfazeko gehienetan zehar. 

Funtzio gehienak markoen gain eragiten dute. Markoak beraien izenen (katea) arabera 
identifikatzen dira, ez dira ordea egiazko Python objektukak. Funtzio askok aukerazko 
parametroa (ez dira gakoak) hartzen dute: markoaren izena. Salbuespen ugari 
ere komunak dira funtzio gehienetan. Hauek ez daude dokumentatuta funtzio 
bakoitzeko dokumentuaren esaldian. 
- Funtzio askok NoDocOpenError abiaraziko dute hauek erabiltzen saiatzen ari bazara eta eragiteko dokumenturik ez badago. 
- Ez baduzu markoaren izenik ematen behar duen funtzio bati, honek unean hautatutako 
markoa erabiliko du, edo hautapenik ez egonez gero, NoValidObjectError abiaraziko du 
non eragitekorik ez badu. 
- Funtzio askok WrongFrameTypeError abiaraziko dute eragiteko sentzurik ez daukaten marko-mota ematen bazaie. Adibidez, testuaren kolorea irudi-marko batean saiatzeak 
ez du sentzurik, eta ondorioz salbuespena abiaraziko da. 
-Azpiko maila batean dagoen Python-eko APIari deitzean gertatzen diren erroreak 
diren bezala bistaratuko dira, eraldatu gabe. Beraz, edozein funtzioren 
salbuespenen zerrenda, hemen ematen dena, osatu gabe dago. 

Funtzio bakoitzak sortzen duen salbuespenari buruzko xehetasunak funtzioen 
dokumentazioan ematen da, nahiz eta Python-eko kodeko erroreak ez egon behar 
bezala dokumentatuta egon funtzioetan.</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument testu-dokumentuak</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>%1. kopia / </translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Beltza</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cyana</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Horia</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Kolore-gurpila</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Letra-tipoen aurrebista</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Nire plugin-a</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Berria txantiloitik</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation type="obsolete">Esportatu irudi gisa</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS inportatzailea</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Gorde txantiloi gisa</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Script motorra</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Laburdurak</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVGra esportatu</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>SVGtik inportatu</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Draw inportatzailea</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus kraskatu egin da honako salbuespenagatik: %1</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Letra-cachea sortzen</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Letra-tipo berria aurkitu da, egiaztatzen...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Aldatutako letra-tipoa aurkitu da, egiaztatzen...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Letra-cachea irakurtzen</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Idaztea eguneratua letra-cachean</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Letra-tipoak bilatzen</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="obsolete">Scribus 1.3.x garapeneko bertsioa exekutatzen ari zara. Lantzen ari zaren 
dokumentua Scribus 1.2.3 edo bertsio baxuagoarekin sortua izan zen. 
Dokumentua gordetzean Scribus 1.2.3 bertsioan erabiltzea eragotziko du, baldin 
eta ez baduzu Fitxategia -&gt; Gorde honela erabiltzen. Ziur zaude eragiketa 
honekin aurrera jarraitu nahi duzula?
</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Dokumentuko aldaketak ez dira gorde, eta leheneratzeko eskatu duzu. 
Jarraitzea nahi duzu?</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>&apos;%1&apos; fitxategia bagado lehenedik ere. &lt;br/&gt;Nahi duzu gordetzen ari zaren fitxategiarekin ordeztea?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>lehenOrriOrdena baimendutakoa baino handiagoa da</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>.sla formatu zaharra onartua</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Alemaniera (trad.)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>PostScript fitxategia esportatzen</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Fitxategia inprimatzen</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Uneko dokumentuan dauden orrialde-kopurua, orrialde aktibotik 
zenbatuz, baino gehiago inportatzen saiatzen ari zara.&lt;/p&gt;Aukeratu 
hauetariko bat:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Sortu&lt;/b&gt; falta diren orrialdeak 
&lt;/li&gt;&lt;li&gt;&lt;b&gt;Inportatu&lt;/b&gt; orrialdeak azkenerarte&lt;/li&gt;&lt;li&gt;&lt;b&gt;Utzi&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>&amp;Sortu</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Inportatu</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Thailandiera</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Barra-kode sortzailea</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd *.odg);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word dokumentuak</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB dokumentuak</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB datuak</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>PDB inportatu</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Ezin izan da %1 fitxategia ireki</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="obsolete">Fitxategi hau ez da PDB dokumentu gisa ezagutu. Ziur bazaude PDB 
motakoa dela, eskertuko genizuke arazo honen berri emango bazenu.</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Luxenburgtar</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabiera</translation>
    </message>
    <message>
        <source>Estonian</source>
        <translation>Estoniera</translation>
    </message>
    <message>
        <source>Japanese</source>
        <translation>Japoniarra</translation>
    </message>
    <message>
        <source>Given master page name does not match any existing.</source>
        <comment>python error</comment>
        <translation>Emandako orri maisuaren izena ez dator bat existitzen den inorekin.</translation>
    </message>
    <message>
        <source>Breton</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dzongkha</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>English (American)</source>
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
        <translation type="unfinished">Laurdena</translation>
    </message>
    <message>
        <source>Foolscap</source>
        <translation type="unfinished">Folioa</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Gutuna</translation>
    </message>
    <message>
        <source>Govt. Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legala</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished">Liburu nagusia</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished">Exekutiboa</translation>
    </message>
    <message>
        <source>Post</source>
        <translation type="unfinished">Posta</translation>
    </message>
    <message>
        <source>Crown</source>
        <translation type="unfinished">Koroa</translation>
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
        <translation type="unfinished">Tartekoa</translation>
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
        <translation type="unfinished">Demy bikoitza</translation>
    </message>
    <message>
        <source>Quad Demy</source>
        <translation type="unfinished">Demy laukoitza</translation>
    </message>
    <message>
        <source>STMT</source>
        <translation type="unfinished">STMT</translation>
    </message>
    <message>
        <source>A</source>
        <translation type="unfinished">A</translation>
    </message>
    <message>
        <source>B</source>
        <translation type="unfinished">B</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">C</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="unfinished">D</translation>
    </message>
    <message>
        <source>E</source>
        <translation type="unfinished">E</translation>
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
        <source>%1 may be corrupted : missing resolution tags</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.x.  Saving the current file under 1.3.x renders it unable to be edited in Scribus 1.2.x versions. To preserve the ability to edit in 1.2.x, save this file under a different name and further edit the newly named file and the original will be untouched. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1(%2) is broken</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2</source>
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
        <source>+tracking %1 </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>-tracking </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>+baseline %1 </source>
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
        <source>Cannot get number of lines of non-text frame.</source>
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
        <translation>Garbitu</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Hautatu dena</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desegin</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Berregin</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ebaki</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Itsatsi</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Sistemako menua</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Itzaldura</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Desitzaldura</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizatu</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Ikonotu</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximizatu</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Itxi</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Berrezarri</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Aldatu lekuz</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaina</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>I&amp;konotu</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximizatu</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Egon &amp;gainean</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Ikonotu</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Itzuli behera</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Itxi</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>It&amp;zaldura</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Desitzaldura</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Dokumentuaren konfigurazioa</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Marjinen gidak</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Goian:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Ezkerrean:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Behean:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Eskuinean:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Orrialde-tamaina</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Horizontala</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntazioa:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Zabalera:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Altuera:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="obsolete">&amp;Unitatea:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Autogorde</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Bitartea:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokumentua</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Gidak</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="obsolete">Orri-ikuspegia</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Kolorea:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">Bistaratu area inprima &amp;ezina marjin-kolorerekin</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="obsolete">Alt+E</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="obsolete">Erakutsi irudiak</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="obsolete">Erakutsi markoen arteko esteka</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="obsolete">Erakutsi markoak</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Bistaratu</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografia</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Tresnak</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Hitz-zatitzailea</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Letra-tipoak</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF esportatzea</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Kolore-kudeaketa</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="obsolete">Markoak bistaratzea aktibatu edo desaktibatu</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="obsolete">Irudiak bistaratzea aktibatu edo desaktibatu</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Paperaren kolorea</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Maskaratu marjinen kanpoko area marjin-koloreekin</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation>Dokumentuaren informazioa</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="obsolete">Erakutsi testu-kontroleko karaktereak</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation type="obsolete">Erregelak erlatiboak orriarekiko
</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation type="obsolete">Lanerako gutxieneko lekua</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation type="obsolete">Orrialdeen arteko tartea</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="obsolete">Horizontala:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="obsolete">Bertikala:</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Egiaztatzailea</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Dokumentuko elementu-atributuak</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Gaien aurkibidea eta Indizeak</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="obsolete">Bistaratu inprima ezin diren karaktereak, paragrafo-markatzaileak bezalakoak, testu-markoetan</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Koloreak doitzen</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="obsolete">Gaitu edo desgaitu estekatutako testu-markoak ikustea.</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="obsolete">Aplikatu tamainaren ezarpenak orrialde guztiei</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Atalak</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="obsolete">Aplikatu orrialdearen tamainaren aldaketak dokumentuko orrialde guztiei</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python script-ak (*.py);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Exekutatu script-luzapen gisa</translation>
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
        <translation>Estilo-kudeatzailea</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation type="obsolete">1. zutabea</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation type="obsolete">&amp;Klonatu</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation type="obsolete">A&amp;dos</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation type="obsolete">&amp;Aplikatu</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation type="obsolete">&amp;Utzi</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+T</translation>
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
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Aplikatu</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Berrezarri</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMCStylePage</name>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
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
        <translation type="unfinished">Itzaldura</translation>
    </message>
</context>
<context>
    <name>SMCharacterStyle</name>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Propietateak</translation>
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
        <translation type="unfinished">Estilo berria</translation>
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
        <translation>Propietateak</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="obsolete">Marrak</translation>
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
        <translation type="unfinished">Estilo berria</translation>
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
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">Marratxoduna</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">Puntukatua</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">Marratxoak eta puntuak</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="unfinished"> pt </translation>
    </message>
</context>
<context>
    <name>SMPStyleWidget</name>
    <message>
        <source>Fixed Linespacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation type="unfinished">Lerrotarte automatikoa</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation type="unfinished">Lerrokatu lerromarrara</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
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
        <translation type="unfinished">Letra kapitalak</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">Tabulatzaileak eta koskak</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Propietateak</translation>
    </message>
    <message>
        <source>Character Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="unfinished">&amp;Marrak:</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation type="unfinished">Distantzia testutik:</translation>
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
        <translation type="unfinished">Paragrafo-estiloa</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Estilo berria</translation>
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
        <translation type="unfinished">Kendu</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Ados</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation type="unfinished">&amp;Utzi</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SMRowWidget</name>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Estilorik ez</translation>
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
        <translation>Estilo-ezarpenak</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Uneko paragrafoaren estiloa</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Kolore-betegarriaren ezarpenak</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Testuaren kolorea</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Testu-kolorearen saturazioa</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Trazu-kolorearen ezarpenak</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Testu-trazuaren kolorea</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Testu-trazuaren kolorearen saturazioa</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Letra-tipoaren ezarpenak</translation>
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
        <translation>Hautatutako testuaren letra-tipoa</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Letra-tamaina</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Karaktereen zabalera eskalatzea</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Karaktereen altuera eskalatzea</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Karaktere-ezarpenak</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Eskuzko jarraipena</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Gorde orrialdea &amp;SVG gisa...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Esportatu SVG fitxategiak</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Esportatu uneko orrialdea SVG fitxategian.</translation>
    </message>
    <message>
        <source>Save as &amp;SVG...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Inportatu &amp;SVG...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Inportatu SVG fitxategiak</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>SVG fitxategi gehieneak inporta ditzake uneko dokumentura, 
beraien bektore-datuak Scribus objektuetan bihurtuz.</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>SVG (Bektore grafiko eskalakorrak)</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>SVG fitxategiak onartu gabeko zenbait ezaugarri ditu</translation>
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
        <translation>Laburdurak</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Aplikatu apurtu ezineko lekua:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Hautatutako markoak</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Orrialde aktiboa</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Elementu guztiak</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Prozesatutako hautatutako markoak soilik.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Prozesatutako uneko orrialdea soilik</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Prozesatutako dokumentuko elementu guztiak</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Erabiltzailearen ezarpenak</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Konfigurazio orokorra</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Gorde</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Berrezarri</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Gorde erabiltzailearen konfigurazioa</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Birkargatu konfigurazio orokorra eta kendu erabiltzaileak definitutakoa</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Editatu konfigurazio pertsonalizatua. Gordetzen baduzu, konfigurazio orokorraren 
gainean erabiliko da.</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Laburdurak</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Erabiltzailearen konfigurazioa badago lehendik ere. Gainidaztea nahi duzu?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Ezin da %1 fitxategia idatzi.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Erabiltzailearen ezarpenak gordeta</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Konfigurazio orokorra birkagatua</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Ezin da %1 fitxategia ireki</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Gorde &amp;txantiloi gisa...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Gorde dokumentua txantiloi gisa</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Gorde dokumentua txantiloi gisa. Bide erosoa da hasierako dokumentuek itxura konstantea edukitzeko</translation>
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
        <translation type="unfinished">&amp;Ados</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Utzi</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Iraunkorra</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Ekintza</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Kargatu/Gorde/Inportatu/Esportatu</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ezezaguna</translation>
    </message>
</context>
<context>
    <name>ScProgressBar</name>
    <message>
        <source>%1 of %2</source>
        <translation type="unfinished">%1 / %2</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Goian</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Eskuinean</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Behean</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Ezkerrean</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Baimendu atrakatzea...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontala</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Bertikala</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Orientazio mugikorra...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Inprimatzen...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">%1. kopia /</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Atzeko planoa</translation>
    </message>
</context>
<context>
    <name>Scribus12Format</name>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished">Scribus 1.2.x dokumentua</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Atzeko planoa</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Document Colors</source>
        <translation type="obsolete">Dokumentuaren koloreak</translation>
    </message>
</context>
<context>
    <name>ScribusCore</name>
    <message>
        <source>Initializing Plugins</source>
        <translation type="unfinished">Plugin-ak hasieratzen</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished">Laster-teklak hasieratzen</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="unfinished">Hobespenak irakurtzen</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="unfinished">ICC profilak irakurtzen</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Letra-tipoak bilatzen</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="unfinished">Ez da letra-tiporik aurkitu sisteman.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="unfinished">Irtetzen.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="unfinished">Errore larria</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished">Letra-tipoen sistema hasieratzen</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Geruza berria</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokumentua</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Atzeko planoa</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Ziur zaude testu osoa garbitzea nahi duzula?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Ezin da erabiltzen dagoen elementua ezabatu</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>%1 elementua Artikulu-editorearekin editatzen ari da. Ezabatzeko eragiketa bertan behera utzi egingo da</translation>
    </message>
    <message>
        <source>An error occurred while opening icc profiles, color management is not enabled.</source>
        <translation type="obsolete">Errorea gertatu da icc profilak irekitzean, kolore-kudeaketa ez dago gaituta.</translation>
    </message>
    <message>
        <source>An error occurred while opening ICC profiles, color management is not enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Koloreak doitzen</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished">&amp;Desblokeatu denak</translation>
    </message>
    <message>
        <source>&amp;Skip locked objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished">Kopia-kopurua: %1
Despl.horizontala: %2
Despl-bertikala: %3</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal gap: %2
Vertical gap: %3</source>
        <translation type="unfinished"></translation>
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
        <source>Initializing Plugins</source>
        <translation type="obsolete">Plugin-ak hasieratzen</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="obsolete">Laster-teklak hasieratzen</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Hobespenak irakurtzen</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Artikulu-editorea hasieratzen</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">ICC profilak irakurtzen</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Hitz-zatitzailea hasieratzen</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Berri laburren albuma irakurtzen</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Laster-bideak ezartzen</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fitxategia</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Editatu</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Letra-tipoak bilatzen</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="obsolete">Ez da letra-tiporik aurkitu sisteman.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="obsolete">Irtetzen.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Errore larria</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Letra-tipoen sistema hasieratzen</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fitxategia</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>A&amp;zken fitxategiak</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Inportatu</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Esportatu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>E&amp;stiloa</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Kolorea</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaina</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Itzaldura</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Letra-tipoa</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efektua</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Elementua</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Aurrebistaren ezarpenak</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Maila</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Bidali &amp;geruzara</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF aukerak</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Intentsitatea</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;Bihurtu hona</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Txertatu</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Karakterea</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Aipua</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Zuriunea</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Orria</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Ikusi</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Hainbat</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Lei&amp;hoak</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Laguntza</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Lerrokatu</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Prest</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Orrialdeak inportatzea...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Inportatu orrialdea(k)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Inportatzea burutua</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Ez da ezer aurkitu inportatzeko</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>%1 fitxategia ez dago onartu daitekeen formatuan</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Kargatzen...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>PostScript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Dokumentuak erabiltzen dituen ICC profila batzuk ez daude instalatuta:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> honek ordeztu du: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(bihurtuta)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Onartutako formatu guztiak</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Ezin da fitxategia idatzi: 
%1</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumentuak (*.sla *.sla.gz *.scd *.scd.gz);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumentuak (*.sla *.scd);; Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Gordetzen...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus-ek errore batzuk antzeman ditu. Agian egiaztatzailea erabili beharko zenuke hauek zuzentzeko</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ez ikusi egin</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>&amp;Bertan behera utzi</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Inprimatzen...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokumentua</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Inprimatzeak huts egin du!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Ezin da erabiltzen dagoen elementua ebaki</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>%1 elementua Artikulu-editorean editatzen ari da. Ebakitzeko eragiketa bertan behera utzi egingo da</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Qt-i buruz</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus-en eskuliburua</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Testu-fitxategiak (*.txt);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normala</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>Bihurtu orrialdea orri maisu</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaina</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Itzaldura:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Itzaldura</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Estilorik ez</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Honako programak falta dira:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript : ezin duzu EPS irudiak edo inprimatzeko aurrebista erabili</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Denak</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus-ek zenbait errore antzeman ditu. 
Agian egiaztatzailea erabili beharko zenuke hauek zuzentzeko.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS fitxategiak (*.eps);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Zenbait errore antzeman dira. 
Agian egiaztatzailea erabili beharko zenuke hauek zuzentzeko.</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-%1orrialdea</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Objektu batzuk blokeatuta daude</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>&amp;Blokeatu denak</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>&amp;Desblokeatu denak</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Informazioa</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>%1 programa jadanik exekutatzen ari da.</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>%1 programa falta da.</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Hautatutako kolorea ez dago dokumentuaren kolore-multzoan. Sartu kolore berri honen 
izena.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Kolorea ez da aurkitu</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Emandako izena badago lehendik ere. Sartu beste izena kolore berri honentzako.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Maila</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Bidali geruzara</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>Aurre&amp;bistaren ezarpenak</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>T&amp;resnak</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X posizioa:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y posizioa:</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="obsolete">Zuriuneak eta jauziak</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation type="obsolete">Lokailua</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>%1 orri maisu berria</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="obsolete">Kopia-kopurua: %1
Despl.horizontala: %2
Despl-bertikala: %3</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript : ezin duzu EPS irudiak edo PostScript inprimatzeko aurrebista erabili</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript falta da: Postscript inprimatzeko aurrebista ez dago erabilgarri</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Ziur zaude existitzen den irudia ordeztea nahi duzula?</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Edukia</translation>
    </message>
    <message>
        <source>Liga&amp;ture</source>
        <translation>&amp;Lokailua</translation>
    </message>
    <message>
        <source>Paste Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;paces &amp;&amp; Breaks</source>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Baliogabeko argumentua: </translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>%1 fitxategia ez da existitzen, abortatzen.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Erabilera: scribus [aukera ... ] [fitxategia]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Aukerak:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Bistaratu laguntza (mezu hau) eta irten</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Hizkuntza adierazteko xx erabiltzen du. Adib. &apos;en&apos; edo &apos;eu&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Unean instalatutako interfazeko hizkuntzen zerrenda</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Erakutsi informazioa kontsolan letra-tipoak kargatzen ari direnean</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Ez erakutsi iragarkia abioan</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Bistaratu bertsioaren informazioa eta irten</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Erabili eskuinetik ezkerrerako botoi-ordena (adib. Utzi/Ez/Bai ordena Bai/Ez/Utzi ordez)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>fitxategi-izena</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Erabili fitxategi-izena bide-izen gisa erabiltzaileak emandako hobespenetan</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Instalatuta dauden Scribus-en interfazeko hizkuntzak:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Hizkuntza lehenetsia gainezartzeko aukeratu:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx edo scribus --lang xx, aukeraren hizkuntza den.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribus bertsioa</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, iturburu irekiko mahaigaineko argitaratzea</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Gune nagusia</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation>Dokumentazioa</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Issues</source>
        <translation>Zenbakiak</translation>
    </message>
    <message>
        <source>Stop the showing of the splashscreen on startup. Writes an empty file called .neversplash in ~/.scribus.</source>
        <translation type="unfinished"></translation>
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
        <translation> %</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation type="obsolete">Geruza</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopiatu hemen</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Ekarri hona</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Itsatsi</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Irudia</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Fitxategia: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Jatorrizko bereizmena: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Uneko bereizmena: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Estekatutako testua</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Testuaren markoa</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Testua bidean</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragrafoak: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Hitzak: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Karak.: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Inprimatu: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Gaituta</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Desgaituta</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Datuak</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF aukerak</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Bidali &amp;geruzara</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>&amp;Maila</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Bihu&amp;rtu:</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Aurrebistaren ezarpenak</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Estekatu testu-markoak</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Betetako marko bati estekatzen saiatzen ari zara, edo markoa bere buruari</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Ezin da erabiltzen dagoen elementua bihurtu</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>%1 elementua Artikulu-editorean editatzen dago. Elementua eskemetara bihurtzeko 
eragiketa saltatu egingo da</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>%1 - %2 orrialdeak</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Kolore-eskala: </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Ezezaguna</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation>GBU</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation>CMHB</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gris-eskala</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Edukia</translation>
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
        <translation type="obsolete">Ikusmen normala</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="obsolete">Gorriaren itsutasuna</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="obsolete">Berdearen itsutasuna</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="obsolete">Urdinaren itsutasuna</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation type="obsolete">Kolore osoen itxutasuna</translation>
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
        <source>Document:</source>
        <translation>Dokumentua:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>aldatu egin da azken aldiz gorde zenetik.</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Baztertu</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Kapsulatutako Python script-en euskarria.</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Script motorra</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Script errorea</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Script ofizial bat exekutatzen ari bazara, eskertuko genizuke 
erroreari buruzko mezua &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; 
helbidera bidaliko bazenu.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Mezu hau arbelean ere badago. Erabili Ktrl+V erroreen arazketa sisteman itsasteko.</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Barneko errorea gertatu da sartutako komandoa probatzean. Xehetasunak 
erroreen irteera estandarrean (stderr) daude.</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Aztertu script-a</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Huts egin du Python-en plugin-a ezartzean. Errorearen xehetasunak stderr irteeran 
zerrendatu dira. </translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="obsolete">Python script-ak (*.py);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Dokumentzioa:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Script-a</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation> ez du inolako laguntzarik.</translation>
    </message>
    <message>
        <source>Python Scripts (*.py *.PY);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Script-hobespenak</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Gaitu script luzapenak</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Luzapenak</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Kontsola</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Abioko script-a:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Erroreak:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Iruzkinak:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Gako-hitzak:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Zeinuak:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Zenbakiak:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Kateak:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Oinarriko testua:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Hautatu kolorea</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Aldatu...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Kokatu abioko script-a</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Erakutsi orrialdeen aurrebistak</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Bilatu/Ordeztu</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Bilatu:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Testua</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Paragrafo-estiloa</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Letra-tipoa</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Letra-tamaina</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Letra-efektuak</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Kolorea</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Itzaldura</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Trazu-kolorea</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Trazu-itzaldura</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Ezkerrean</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Erdian</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Eskuinean</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blokeatu</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Behartuta</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Ordeztu honekin:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>Hitz &amp;osoa</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Ez &amp;bereizi maiuskulak eta minuskulak</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Bilatu</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Ordeztu</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Ordeztu &amp;denak</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Itxi</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Bilaketa burutua</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Garbitu</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Bilaketa amaitua, bat datozen %1 aurkituta</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Hautatu eremuak</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Erabilgarri dauden eremuak</translation>
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
        <translation>Hautatutako eremuak</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Bestelakoak...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Itzaldura:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Itzaldura</translation>
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
        <translation>X despl.</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y despl.</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Laburdurak...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Laburdurak</translation>
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
        <translation type="unfinished">Gakorik e&amp;z</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">&amp;Erabiltzaileak definitutako gakoa</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished">Alt+Maius+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">Ezarri &amp;gakoa</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation type="unfinished">Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="unfinished">Ktrl</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="unfinished">Maius</translation>
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
        <translation type="unfinished">Maius+</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation type="unfinished">Alt+</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation type="unfinished">Ktrl+</translation>
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
        <translation>Estilorik ez</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Editatu estiloak...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Estilorik ez</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Editatu estiloak</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Bi&amp;koiztu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>%1(r)en kopia</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Estilo berria</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumentuak (*.sla *.sla.gz *.scd *.scd.gz);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumentuak (*.sla *.scd);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Inportatu</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Artikulu-editorea</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Birkargatu testua markotik</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Gorde fitxategian...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Kargatu fitxategitik...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Gorde &amp;dokumentua</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Eguneratu testu-markoa eta irten</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Irten testu-markoa eguneratu gabe</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Hautatu &amp;denak</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ebaki</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopiatu</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Itsatsi</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Garbitu</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Bilatu/Ordeztu...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Editatu estiloak...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Letra-tipoen aurrebista...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Eguneratu testu-markoa</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Atzeko planoa...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Bistaratu letra-tipoa...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Fitxategia</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editatu</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Ezarpenak</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Fitxategia</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Uneko paragrafoa:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Hitzak: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Karak.: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Guztira:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragrafoak: </translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Nahi duzu aldaketak gordetzea?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Ireki</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Testu-fitxategiak (*.txt);;Fitxategi guztiak (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Gorde honela</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>Testu-hautapen &amp;azkarra</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Txertatu glifoa...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Garbitu testu osoa</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Artikulu-editorea  - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Ziur zaude aldaketa guztiak galtzea nahi dituzula?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Ziur zaude testu guztia garbitzea nahi duzula?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Txertatu</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Karakterea</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Aipua</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Zuriuneak eta jauziak</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Lokailua</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Zuriunea</translation>
    </message>
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation>Automatikoa</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Desplazamendua</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Marra-zabalera</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation type="obsolete">Hainbat elementu hautatuta</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Izena:</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Berrezarri</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Aplikatu</translation>
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
        <translation type="unfinished">&amp;Berria</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Clone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Berria</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Ezabatu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editatu</translation>
    </message>
    <message>
        <source>&amp;Done</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Editatu</translation>
    </message>
    <message>
        <source>New %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished">Tekla-sekuentzia hau jadanik erabiltzen dago</translation>
    </message>
    <message>
        <source>More than one style selected</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Small Caps</source>
        <translation>Maiuskula txikiak</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Azpi-indizea</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Goi-indizea</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Maiuskulak</translation>
    </message>
    <message>
        <source>Underline Text. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Azpimarratu testua. Eduki botoia sakatuta une batez marraren zabalera eta desplazamenduaren aukerak ezartzeko.</translation>
    </message>
    <message>
        <source>Underline Words Only. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Azpimarratu hitzak soilik. Eduki botoia sakatuta une batez marraren zabalera eta desplazamenduaren aukerak ezartzeko.</translation>
    </message>
    <message>
        <source>Strike Out. Hold down the button momentarily to set line width and displacement options.</source>
        <translation>Marratu. Eduki botoia sakatuta une batez marraren zabalera eta desplazamenduaren aukerak ezartzeko.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <translation type="obsolete">Eskema. Eduki botoia sakatuta une batez marraren zabalera eta desplazamenduaren aukerak ezartzeko.</translation>
    </message>
    <message>
        <source>Shadowed Text. Hold down the button momentarily to enable the offset spacing.</source>
        <translation>Itzaldura. Eduki botoia sakatuta une batez marraren zabalera eta desplazamenduaren aukerak ezartzeko.</translation>
    </message>
    <message>
        <source>Outline. Hold down the button momentarily to change the outline stroke width.</source>
        <comment>Text Style Selector</comment>
        <translation type="unfinished">Eskema. Eduki botoia sakatuta une batez marraren zabalera eta desplazamenduaren aukerak ezartzeko.</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Erabili dokumentuaren izena aurrizki gisa paragrafo-estiloetan</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Ez galdetu berriro</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Writer inportatzailearen aukerak</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Gainidatzi paragrafo-estiloak</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Hau gaitzen bada, uneko dokumentuan dauden estiloak gainidatzi egingo dira</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Batu paragrafo-estiloak</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Batu paragrafo-estiloak atributuen arabera. Honen ondorioz, paragrafo-estilo 
kopurua gutxiagotu egingo da, baina estiloen atributuak mantenduko ditu, nahiz 
eta jatorrizko dokumentuko estiloei ezberdin deitu.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Jarri aurretik dokumentuaren izena paragrafo-estiloaren izenari Scribus-en.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Jarri ezarpen hauek lehenetsi gisa, eta ez galdetu berriro OpenOffice.org 1.x 
bertsioko dokumentua inportatzean.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Hasieran</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Amaieran</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Ez erakutsi</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Gaien aurkibidea eta Indizeak</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Gaien aurkibidea</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Kokatu aurkibidearen markoa hemen:</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Kokatutako orri-kopurua:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Elementua atributuaren izena:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Atributu hau markoei aplikatuko zaie, sarrerak sortzeko oinarritzat erabiltzen direnei</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Kokatu sarreren orri-zenbakiak lerroaren hasieran edo amaieran, edo inon ere ez</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Inprima ezineko sarreren zerrenda</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Gehitu inprima ezineko markoak</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Paragrafo-estiloak, sarrerako lerroetan erabilitakoak</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Paragrafo-estiloa:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Helburuko markoa:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="obsolete">&amp;Indizea</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Gaien aurkibidea eta Indizeak</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Gaien aurkibidea</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Gehitu</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Kokatu aurkibidearen markoa hemen:</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Kokatutako orri-kopurua:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Elementua atributuaren izena:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Atributu hau markoei aplikatuko zaie, sarrerak sortzeko oinarritzat erabiltzen direnei</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Kokatu sarreren orri-zenbakiak lerroaren hasieran edo amaieran, edo inon ere ez</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Inprima ezineko sarreren zerrenda</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Gehitu inprima ezineko markoak</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Paragrafo-estiloak, sarrerako lerroetan erabilitakoak</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Paragrafo-estiloa:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Helburuko markoa:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation type="obsolete">&amp;Indizea</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Ez ikusi egin errore guztiei</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Egiaztatu automatikoki inprimatu edo esportatu aurretik</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Egiaztatu glifoak falta diren</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation type="obsolete">Egiaztatu orrialdean ez dauden objektuak</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Egiaztatu testu-markoak gainezkatu diren</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation type="obsolete">Egiaztatu gardentasunak</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Egiaztatu irudiak falta diren</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Egiaztatu irudiaren bereizmena</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Baimendutako bereizmen baxuena</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Egiaztatu PDF oharpenak eta eremuak</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Gehitu profila</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Kendu profila</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Egiaztatu kokatutako PDF fitxategiak</translation>
    </message>
    <message>
        <source>Highest allowed resolution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for GIF images</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Paperaren kolorea</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Maskaratu marjinen kanpoko area marjin-koloreekin</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation type="unfinished">Gaitu edo desgaitu estekatutako markoak bistaratzea.</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation type="unfinished">Bistaratu inprima ezin diren karaktereak, paragrafo-markatzaileak bezalakoak, testu-markoetan</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation type="unfinished">Markoak bistaratzea aktibatu edo desaktibatu</translation>
    </message>
    <message>
        <source>Turns the display of layer indicators on or off</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation type="unfinished">Irudiak bistaratzea aktibatu edo desaktibatu</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Dokumentuko oihalaren ezkerreko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Dokumentuko oihalaren eskuineko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Dokumentuko oihalaren gaineko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation type="unfinished">Dokumentuko oihalaren azpiko tarte-kopurua arbel bat bezala 
definitzen du, elementuak sortu eta eraldatzeko, eta hauek orrialde 
aktibora arrastatzeko</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="unfinished">Ezarri zoomaren maila lehenetsia</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished">Kokatu erregela pantailan eta arrastatu graduatzailea zoomaren 
maila ezartzeko, honela Scribus-ek orrialdeak eta hauen objektuak 
tamaina egokiarekin bistaratuko ditu</translation>
    </message>
</context>
<context>
    <name>TabDisplayBase</name>
    <message>
        <source>Form2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Orokorra</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished">Orri-ikuspegia</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation type="unfinished">Erakutsi irudiak</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation type="unfinished">Erakutsi testu-kontroleko karaktereak</translation>
    </message>
    <message>
        <source>Rulers Relative to Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation type="unfinished">Erakutsi markoen arteko esteka</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation type="unfinished">Erakutsi markoak</translation>
    </message>
    <message>
        <source>Show Layer Indicators</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished">Laneko area</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Ezkerrean:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Eskuinean:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Goian:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Behean:</translation>
    </message>
    <message>
        <source>Gaps Between Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation type="unfinished">Horizontala:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation type="unfinished">Bertikala:</translation>
    </message>
    <message>
        <source>Adjust Display Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation type="unfinished">Pantaila doitzeko arrastatu azpiko erregela graduatzailearekin</translation>
    </message>
    <message>
        <source>textLabel16</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Koloreak</translation>
    </message>
    <message>
        <source>Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation type="unfinished">Kolore-betegarria:</translation>
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
        <translation type="obsolete">Pertsonalizatua</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Tamaina:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Bertikala</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Horizontala</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Units:</source>
        <translation type="unfinished">Unitateak:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Altuera:</translation>
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
        <source>Margin Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Bitartea:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation type="unfinished">Desegin/Berregin</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation type="unfinished">Ekintzen historiaren luzeera</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Dokumentuko orrialdeen zabalera, editagarria da 
orrialdeen tamaina pertsonalizatua aukeratuz gero</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Dokumentuko orrialdeen altuera, editagarria da 
orrialdeen tamaina pertsonalizatua aukeratuz gero</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished">Orri-tamaina lehenetsia, tamaina estandarra edo pertsonalizatua</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="unfinished">Dokumentuko orrialdeen orientazio lehenetsia</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished">Gaituta dagoenean Scribus-ek zure fitxategiaren babeskopia bat gordeko du &apos;.bak&apos; luzaperarekin denbora bat iragatzen den 
bakoitzean</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="unfinished">Denbora-tartea automatikoki gordetzeko</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished">Ekintzen historiaren luzera ezartzen du urratsetan. 0 balioak 
ekintzen kopuru infinitua gorde egingo dela ezartzen du.</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished">Aplikatu orrialdearen tamainaren aldaketak dokumentuko orrialde guztiei</translation>
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
        <translation type="unfinished">Kokatu Ghostscript</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation type="unfinished">Kokatu irudi-editorea</translation>
    </message>
    <message>
        <source>Locate your web browser</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabExternalToolsWidgetBase</name>
    <message>
        <source>External Tools</source>
        <translation type="unfinished">Kanpoko tresnak</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation type="unfinished">PostScript interpretatzailea</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished">&amp;Exekutagarria:</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Add the path for the Ghostscript interpreter. On Windows, please note it is important to note you need to use the program named gswin32c.exe - NOT gswin32.exe. Otherwise, this maybe cause a hang when starting Scribus.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Change..</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished">&amp;Testua antialias-ekin</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="unfinished">Testua antialias-ekin EPSak eta PDFak pantailan errendatzeko</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished">&amp;Grafikoak antialias-ekin</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished">Alt+G</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="unfinished">Grafikoak antialias-ekin EPSak eta PDFak pantailan errendatzeko</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation type="unfinished">Bereizmena:</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="unfinished">Irudia lantzeko tresna</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished">Exe&amp;kutagarria:</translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for graphics editor. If you use gimp and your distribution includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Aldatu...</translation>
    </message>
    <message>
        <source>Web Browser</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Web Browser to launch with links from the Help system</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;File system location for your web browser. This is used for external links from the Help system.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rescan</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rescan for the external tools if they do not exist in the already specified location</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGeneral</name>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation type="unfinished">Hautatu hizkuntza lehenetsia Scribus-ek erabiltzeko. Utzi hau hutsik
inguruneko aldagaien arabera aukeratzeko. Erabilera hau 
gainezartzeko komando-lerroko aukera erabil dezakezu Scribus abiaraztean</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished">Azken aldian editatutako dokumentu-kopurua 
Fitxategia menuan erakusteko</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished">Scribus-ek korrituko dituen lerro-kopurua 
saguaren gurpileko jauzi bakoitzeko</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished">Aukeratu leihoko dekorazio eta itxura lehenetsia. Scribus aplikazioak 
KDEren edo Qt gai erabilgarriak jasotzen ditu, baldin eta Qt 
konfiguratuta badago KDEko osagaiak bilatzeko.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished">Menu eta leihoen letra-tamaina lehenetsia</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished">Tresnen leihoen letra-tamaina lehentesia</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="unfinished">Dokumentuen direktorio lehenetsia</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished">ICC profilen direktorio lehentesia. Hau ezin da irekitako dokumetu 
batekin aldatu. Lehenetsi gisa, Scribus-ek sistemako direktorioetan 
begiratuko du Mac OSX eta Windows sistemetan. GNU/Linux eta Unix sistemetan $home/.color/icc,/usr/share/color/icc 
eta /usr/local/share/color/icc direktorioetan begiratuko du.</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished">Script-en direktorio lehenetsia</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished">Txantiloien direktorio gehigarria</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Aukeratu direktorioa</translation>
    </message>
</context>
<context>
    <name>TabGeneralBase</name>
    <message>
        <source>Form1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Hizkuntza:</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished">&amp;Gaia:</translation>
    </message>
    <message>
        <source> ms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Time before a Move or Resize starts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished">Letra-tamaina (&amp;paletak):</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished">Letra-tamaina (&amp;menuak):</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished">&amp;Gurpil-jauzia:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished">&amp;Azken dokumentuak:</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation type="unfinished">Erakutsi elkarrizketa-koadroa abiaratzean</translation>
    </message>
    <message>
        <source>Show Splashscreen on Startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished">Al&amp;datu...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document &amp;Templates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished">&amp;Dokumentuak:</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished">&amp;ICC profilak:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">Al&amp;datu...</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="unfinished">&amp;Script-ak:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+H</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Aldatu...</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Ezarpen arruntak</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Dokumentuetan kokatzea</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>Atzeko planoan</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>Aurreko planoan</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Atxikitzea</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Atxikitze-distantzia:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Hautapen-erradioa:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Erakutsi gidak</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Erakutsi marjinak</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Erakutsi orri-sareta</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Sareta nagusia</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Kolorea:</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Tartea:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>2. mailako sareta</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Erakutsi lerromarrak</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>&amp;Lerromarrak:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Lerromarren &amp;despl.:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>2. mailako saretako marren arteko distantzia</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Sareta nagusiko marren arteko distantzia</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Distantzia objektu bat kokatutako gidei atxikitzeko</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Arearen erradioa, bertatik objektua har daitekeena kudeatzeko</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>2. mailako saretako marren kolorea</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Sareta nagusiko marren kolorea</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Txertatutako gida-marren kolorea</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Margina-marren kolorea</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Lerroen oinarrizko marrak aktibatu/desaktibatu</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Saretako marrak aktibatu/desaktibatu</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Gidak aktibatu/desaktibatu</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Marjinak aktibatu/desaktibatu</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Lerromarren ezarpenak</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Gidak ez dira ikusgai objektuetan zehar orrialdean</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Gidak ikusgai dira objektuen gainetik orrialdean</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Lerromarren kolorea</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Lerromarren arteko distantzia</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Lehen lerromarraren distantzia orriaren gainetik</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidget</name>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation type="unfinished">Tekla multzoaren XML fitxategiak (*.ksxml)</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation type="unfinished">Alt</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="unfinished">Ktrl</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="unfinished">Maius</translation>
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
        <translation type="unfinished">Maius+</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation type="unfinished">Alt+</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation type="unfinished">Ktrl+</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished">Tekla-sekuentzia hau jadanik erabiltzen dago</translation>
    </message>
</context>
<context>
    <name>TabKeyboardShortcutsWidgetBase</name>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation type="unfinished">Laster-teklak</translation>
    </message>
    <message>
        <source>Search:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action</source>
        <translation type="unfinished">Ekintza</translation>
    </message>
    <message>
        <source>Shortcut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shortcut for Selected Action</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="unfinished">Gakorik e&amp;z</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">&amp;Erabiltzaileak definitutako gakoa</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">Ezarri &amp;gakoa</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>CTRL+ALT+SHIFT+W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation type="unfinished">Laster-tekla multzo kargagarria</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation type="unfinished">Laster-tekla multzoa eskuragarri dago kargatzeko</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation type="unfinished">&amp;Kargatu</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation type="unfinished">Kargatu hautatutako laster-tekla multzoa</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation type="unfinished">&amp;Inportatu...</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation type="unfinished">Inportatu laster-tekla multzoa uneko konfiguraziora</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation type="unfinished">&amp;Esportatu...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation type="unfinished">Esportatu uneko laster-teklak fitxategi inportagarri batera</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished">&amp;Berrezarri</translation>
    </message>
    <message>
        <source>Alt+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation type="unfinished">Birkargatu Scribus-eko laster-tekla lehenetsiak</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Kudeatu tabulatzaileak</translation>
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
        <translation type="unfinished">Galdetu letra-tipoak aldatu aurretik dokumentua kargatzean</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation type="unfinished">Uneko paragrafo-estiloaren aurrebista estiloak editatzean</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation type="unfinished">Lorem Ipsum</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation type="unfinished">Erabili beti Lorem Ipsum estandarra</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation type="unfinished">Paragrafo-kopurua:</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Esportatu barrutia</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>Orrialde &amp;guztiak</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>&amp;Aukeratu orrialdeak</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Biraketa:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Fitxategi-aukerak</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Bateragarritasuna:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Lotura:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Ezkerreko marjina</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Eskuineko marjina</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Sortu koadro &amp;txikiak</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Gorde &amp;estekatutako testua PDF artikulu gisa</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Gehitu laster-markak</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>&amp;Konprimitu testua eta bektore-grafikoak</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatikoa</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Gehienez</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Altua</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Tartekoa</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Baxua</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Gutxienekoa</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Orokorra</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Kapsulatu letra-tipo guztiak</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">&amp;Elkartu letra-tipo guztiak</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Kapsulatzen</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Erabilgarri dauden letra-tipoak:</translation>
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
        <translation>Kapsulatzeko letra-tipoak:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Letra-tipoak elkartzeko:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Letra-tipoak</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Gaitu &amp;aurkezpen-efektuak</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Orrialdea</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Erakutsi orrien &amp;aurrebistak</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efektuak</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Bistaratze-iraupena:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>&amp;Efektu-iraupena:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Efektu-&amp;mota:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Marra-mugikorrak:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;Nondik:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Norabidea:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> seg</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Efekturik ez</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Pertsianak</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Kutxa</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Disolbatu</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Distira</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Zatitu</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Ezabatu</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontala</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Bertikala</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Barruan</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Kanpoan</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Ezkerretik eskuinera</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Goitik behera</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Behetik gora</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Eskuinetik ezkerrera</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Goi-ezkerretik behe-eskuinera</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Aplikatu efektuak orrialde guztietan</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Hainbat</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>Erabili en&amp;kriptazioa</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Pasahitzak</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Erabiltzailea:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Jabea:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Ezarpenak</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Baimendu dokumentua &amp;inprimatzea</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Baimendu dokumentua &amp;aldatzea</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Baimendu testu eta grafikoak &amp;kopiatzea</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Baimendu &amp;oharpenak eta eremuak gehitzea</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Segurtasuna</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Orokorra</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Irteera &amp;hona:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Pantaila / Web gunea</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Inprimagailua</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Gris-eskala</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Erabili errendatze-ezarpen pertsonalizatuak</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Errendatze-ezarpenak</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Maiztasuna</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Angelua:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Trama:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Puntu soila</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Marra</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Biribila</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipsea</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Kolore lisoak:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Erabili ICC profila</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profila:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Asmoa errendatzen:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Pertzepziokoa</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Koloremetria erlatiboa</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturazioa</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Koloremetria absolutua</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Irudiak:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Ez erabili kapsulatutako ICC profilak</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>K&amp;olorea</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 irteera</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Informazio-katea:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Irteerako &amp;profila:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Mozte-koadroa</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Erakutsi orrialdeen aurrebista, gainean zerrendatutakoak.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Bistaratzeko efektu-mota.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Marra-mugikorren efektuaren norabidea efektuak zatitu eta lotzeko.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Hasierako posizioa kutxa- eta zaitze-efektuentzat.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Distira- edo ezabatze-efektuen norabidea.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Aplikatu hautatutako efektua orrialde guztietan.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Esportatut orrialde guztiak PDFra</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Esportatu orri-barrutia PDFra</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Sortu PDF artikuluak, PDF batean estekatutako artikuluetan zehar erosoki nabigatzeko.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (puntu hazbeteko) irudia esportatzeko.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Aukeratu erabiltzaileen pasahitzak zure PDFa irakur ahal izateko.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Baimendu PDFa inprimatzea. Ez bada aktibatzen, inprimatzea galarazita egongo da.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Baimendu PDFa eraldatzea. Ez badago aktibatuta, PDFa aldatzea galarazita egongo da.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Kapsulatu kolore lisoen kolore-profila</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Kolore lisoen kolore-profila</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Kolore lisoak errendatzeko saiakera</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Kapsulatu irudien kolore-profila</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Ez erabili kolore-profilak, irudien iturburuetan kapsulatuta daudenak</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Irudien kolore-profila</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Irudiak errendatzeko saiakaera</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Irteerako profila inprimatzeko. Posible bada, begiratu zure inprimagailuko dokumentazioan.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Distantzia orriaren goitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Distantzia orriaren azpitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Distantzia orriaren ezkerretik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Distantzia orriaren eskuinetik kostatzeko</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Ispilatu orria(k) horizontalki</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Ispilatu orria(k) bertikalki</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>EPS grafikoen bereizmena:</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Bihurtu kolore laguntzaileak kuatrokomiara</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Konpresioaren &amp;kalitatea:</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="obsolete">Kompresioaren kalitatearen mailak galera duten konpresio-metodoentzat: minimoa (%25), 
baxua (%5), tartekoa (%75), gehienezkoa (%95). Ohartu kalitatearen mailak ez duela 
emaitzako irudiaren tamainarekin zuzeneko zerikusia. Bai tamaina bai galeradun kalitatea 
irudiaren arabera daude.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Baimendu testua edo grafikoak PDFtik kopiatzea. Ez bada aktibatzen, testua eta grafikoak ezin izango dira kopiatu.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Baimendu oharpenak eta eremuak PDFan gehitzea. Ez bada aktibatzen, oharpenak eta eremuak editatzea galaraziko da.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Kolore lagungarriak kuatrikomiako koloretan bihurtzea gaitzen du. Inprimagailu komertzial batean inprimatzea nahi izanez gero, aukera hau gaituta uztea gomendatzen da.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>Gehitu &amp;geruzak</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Konpresio-&amp;metodoa:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Berritu irudiaren &amp;dimentsioa:</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Kapsulatu letra-tipoak PDFan. Letra-tipoak kapsulatzeak dokumentuaren diseinua eta itxura mantenduko du.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Efektua exekutatzek denbora. Denbora motz batek efektua abiazkartuko du, handiago batek moteldu egingo du.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Txertatu komaz bereiztutako token-en zerrenda, 
token bat * orrialde guztientzako, 1-5 orrialde barruti 
batentzako, edo orrialde bakar batentzako izan daiteke.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Orrien loturak zehazten ditu PDFan. Aldatzeko beharreik ez baduzu, utzi aukera lehenetsiarekin: ezkerra.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Orrialde bakoitzaren koadro txikika sortzen du PDFan. Ikustaile batzuk koadro txikiak erabil ditzakete nabigatzeko.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Kapsulatu dokumentuan sortutako laster-markak. Oso erabilgarriak dira PDF dokumentu luzeetan nabigatzeko.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Esportatu testu eta bektore-grafikoen bereizmena. Honek ez du eraginik argazkiak bezalako bitmapa irudien bereizmenean.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Testu eta grafikoen galerarik gabeko konpresioa gaitzen du. Ez baduzu beste arrazoirik, utzi hau egiaztatuta. Honek PDF fitxategiaren tamaina gutxiagotzen du.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Gaitu segurtasun funtzioak esportatutako PDFan. PDF 1.3 aukeratzen baduzu, PDFa 40 bit-eko enkriptazioarekin babestuko da. PDF 1.4 aukeratuz enkriptatzean 128 bit erabiliko dira PDFa babesteko. Ezespena: PDFa enkriptatzea ez da GPG edo PGP enkripzio bezain fidagarria eta zenbait murriztapen ditu.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Aukeratu pasahitz nagusia segurtasun funtzioak gaitu edo desgaitzeko esportatutako PDFan</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Hau ezarpen aurreratu da, lehenetsi gisa gaitzen ez dena. Ezarpen hau soilik gaitu baldin eta inprimagailuak bereziki eskatzen badu eta behar dituen zehaztasun guztiak ematen baditu. Bestela, esportatutako PDFa ez da ongi inprimatuko eta sistemetan zehar ez da eramangarria izango.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>PDF/X-3 moduaren derrigorrezko katea, bestela PDFak huts egingo du PDF/X-3 betetzean. Dokumentuaren titulua erabiltzea gomendatzen dugu.</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Bistaratze-ezarpenak</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation>Orri-diseinua</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Alde batetik</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Etengabea</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Orrialde bikoitza ezkerra</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Orrialde bikoitza eskuina</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Itxura bisuala</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Erabili ikustaileen parametro lehenetsiak</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Erabili pantaila osoko modua</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Bistaratu laster-marken fitxak</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Bistaratu koadro txikiak</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Bistaratu diseinuen fitxak</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Ezkutatu ikustaileen tresna-barra</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Ezkutatu ikustaileen menu-barra</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Egin orrien zooma ikustailearen leihoa doitzeko</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Ekintza bereziak</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Script-ik ez</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Ikustailea</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Moztu orriaren marjinetara</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Galerarekin - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Galerarik gabea - Zip</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Irudiaren konpresio metodoa</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Exekutatutako den JavaScript kodea
PDF dokumentuan irekitzean:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Aurkezpeneko efektuak gaitzen ditu pantaila osoko modua duten 
Adobe&amp;#174; Reader&amp;#174; eta beste PDF ikustaileak erabiltzean</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">PDFren bateragarritasuna zehazten du. Lehenetsia PDF 1.3 da, bateragarritasun 
handiena eskaintzen duena. Aukeratu PDF 1.4 dokumentuak gardentasunak edo 128 
bit-eko enkriptazioa bezalako ezaugarriak erabiltzen baditu. PDF 1.5 beharrezkoa da 
PDF barruko objektuak geruza ezberdinetan bereiztuta egotea nahi izanez gero. 
PDF/X-3 modua egoera honetan erabili: PDFak esportatzean inprimagailu komertzial baten kolore-kudeaketa GBUrekin landu eta hautagarria denean kolore-kudeaketa aktibatutakoan. Inprimagailuak jakinarazitakoan edo 4 koloretako laserreko inprimagailuan noizbehinka inprimatzean.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Dokumentuko geruzak PDFra esportatzen dira. PDF 1.5 aukeran soilik erabilgarria.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options. This only affects JPEG images</source>
        <translation type="obsolete">Irudiak konprimitzeko erabiliko den metodoa. Automatikoak metodo egokiena 
hautatzea baimentzen dio Scribus-i. ZIP galerarik gabekoa da, eta kolore lisoak 
dituzten irudientzako egokia da. JPEG hobeagoa da argazkiak dituzten PDF fitxategi 
txikiagoak sortzeko (irudiaren kalitate-galera pittin batekin). Utzi aukera &apos;Automatikoa&apos; 
gisa, konpresioko aukera berezirik ez bada behar bederen. Honek JPEG motako irudiei 
eragiten die soilik.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Aldatu bitmapa motako irudiaren dimentsioak hautatutako DPIra. Egiaztatu gabe utziz 
gero, jatorrizko bereizmenarekin errendatuko dira. Hau gaitzen bada, memoriaren erabilpena areagotuz bat esportatzea motelduko du.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>PDFaren irteerako kolore-modeloa. Aukeratu &apos;Pantaila/Web gunea&apos; pantailan bistaratuko diren PDFentzako eta tintaz inprimatzeko. Aukeratu &apos;Inprimagailua&apos; egiazko 4 koloredun CMHB motako inprimagailuan inprimatzeko. Aukeratu &apos;Gris-eskala&apos; PDF dokumentua gris-eskalan egotea nahi denean.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Ez erakutsi marjinetatik kanpo dauden objektuak esportatutako fitxategian</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page. Setting 0 will disable automatic page transition.</source>
        <translation>Denbora orrialdea erakusteko aurkezpena hautatutako orrialdean hasi aurretik. 
0 balioarekin ezartzen bada, orrialdeen transizio automatikoa desgaituko da.</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.&lt;br/&gt;The default is &lt;b&gt;PDF 1.3&lt;/b&gt; which gives the widest compatibility.&lt;br/&gt;Choose &lt;b&gt;PDF 1.4&lt;/b&gt; if your file uses features such as transparency or you require 128 bit encryption.&lt;br/&gt;&lt;b&gt;PDF 1.5&lt;/b&gt; is necessary when you wish to preserve objects in separate layers within the PDF.&lt;br/&gt;&lt;b&gt;PDF/X-3&lt;/b&gt; is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
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
        <translation type="unfinished">Eskuinean:</translation>
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
        <translation type="unfinished">Distantzia orriaren goitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Distantzia orriaren azpitik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Distantzia orriaren ezkerretik kostatzeko</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Distantzia orriaren eskuinetik kostatzeko</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation type="unfinished">Ez erakutsi objekturik inprimatutako orrien marjinetatik kanpo</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished">Erabili inprimatzeko beste kudeatzailea, kprinter edo gtklp bezalakoa, inprimatzeko aukera gehigarriak erabiltzeko</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation type="unfinished">Postscript maila ezartzen du. 
1. edo 2. mailarekin ezartzean fitxategi oso handiak sortzen ditu</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Grisen itzaldura itzaltzeko modu bat. Itzaldura hau cyana, 
horia eta magenta koloreekin osatua dago, beltzaren ordez 
erabiliz. UCR metodoak irudi bateko tonu grisetik gertu dauden 
neutralak edota ilunak diren zatiei eragiten die batiz bat. Honen 
erabilpenak zenbait irudiren inprimazioa hobetzen du, eta kasuz 
kasuko esperimentazioa eta probak egin behar izaten dira. 
UCRek gainsaturazioaren posibilitatea gutxitzen du CMH tintekin.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished">Kolore lagungarriak kuatrikomiako koloretan bihurtzea gaitzen du. Inprimagailu komertzial batean inprimatzea nahi izanez gero, aukera hau gaituta uztea gomendatzen da.</translation>
    </message>
    <message>
        <source>Enables global Overprint Mode for this document, overrides object settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished">ICC profilak kapsulatzea baimentzen zaitu inprimatze-korrontean 
kolore-kudeaketa gaituta dagoenean</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished">Postscript fitxategiaren euskarriaren tamaina esplizitoki ezartzea gaitzen du. Ez da gomendagarria, erabili soilik zure 
inprimagailuak eskatzen badu.</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Fitxategia</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Denak</translation>
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
        <translation type="unfinished">Inprimatze-helburua</translation>
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
        <translation type="unfinished">Aukerak</translation>
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
        <translation type="unfinished">Inprimatu koloretan</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation type="unfinished">Inprimatu gris-eskalan</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Orrialdea</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation type="unfinished">Ispilatu orria(k) horizontalki</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation type="unfinished">Ispilatu orria(k) bertikalki</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished">Ezarri euskarri-tamaina</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished">Moztu orriaren marjinetara</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Kolorea</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation type="unfinished">Aplikatu azpikoloreak (UCR)</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished">Bihurtu kolore laguntzaileak kuatrokomiara</translation>
    </message>
    <message>
        <source>Force Overprint Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation type="unfinished">Aplikatu ICC profilak</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Orokorra</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation type="unfinished">Inprimatu normal</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation type="unfinished">Inprimatu bereizleak</translation>
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
        <translation type="unfinished">Eskuinean:</translation>
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
        <translation>Letra-tipoa:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Tamaina:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Bat ere ez</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Kolore-betegarria:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Trazuaren kolorea:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Zutabeak:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Tartea:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Laino guztien azpitik eta sasi guztien gainetik</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>&amp;Marra-kolorea:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Itzaldura:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>&amp;Kolore-betegarria:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>&amp;Itzaldura:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Marra-estiloa:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Marra-&amp;zabalera:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Marra-&amp;estiloa:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Geziak:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Hasiera:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Amaiera:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Eskalatze &amp;librea</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Eskalatze &amp;horizontala:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Eskalatze &amp;bertikala:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Eskalatu irudia markoaren tamainara</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>&amp;Mantendu aspektu-erlazioa</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>&amp;Kolore-betegarria:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Gu&amp;txienekoa:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>G&amp;ehienezkoa:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Urratsa:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Testu-markoen propietateak</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Irudi-markoen propietateak</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Formen propietateak</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Bistaratze-propietateak</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Marren propietateak</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Poligonoen propietateak</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Letra-tipoa testu-marko berrietan</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Letra-tamaina testu-marko berrietan</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Letra-kolorea</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Zutabe-kopurua testu-markoan</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Zutabe arteko tartea testu-markoan</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Letra-tipoaren lagina</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Irudi-markoak baimentzen du irudiak edozein tamainara eskalatzea</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Irudien eskalatze-horizontala</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Irudien eskalatze-bertikala</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Mantendu eskalatze-proportzioa</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Irudi-markoetako irudiak markoaren tamainara eskalatuko dira</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Automatikoki eskalatutako irudiek jatorrizko proportzioak mantentzen dute</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Irudi-markoen kolorea</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Kolorearen saturazioa</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Formen marraren kolorea</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Marren kolorearen saturazioa</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Formen kolorea</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Formen marra-estiloa</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Formen marra-zabalera</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Gutxieneko handitzea baimendua</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Gehienezko handitzea baimendua</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Zoom-urratsa</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Marren kolorea</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Kolorearen saturazioa</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Marren estiloa</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Marren zabalera</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Tabulazio-karakterea:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Tabulazio-zabalera:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Erabili kapsulatutako bide-mozketa</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Aurrebista</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Bereizmen osoko aurrebista</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Bereizmen arrunteko aurrebista</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Bereizmen baxuko aurrebista</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="obsolete">Pertsonalizatua:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="obsolete">Pertsonalizatua: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Testu-kolorea:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Itzaldura:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Testu-trazua:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Puntua</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Marra</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Azpimarra</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Pertsonalizatua</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Testua</translation>
    </message>
    <message>
        <source>Shapes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lines</source>
        <translation type="unfinished">Marrak</translation>
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
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished">Bat ere ez</translation>
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
        <translation>Azpi-indizea</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Desplazamendua:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Eskalatzea:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Goi-indizea</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>De&amp;splazamendua:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Es&amp;kalatzea:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Maiuskula txikiak</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Esk&amp;alatzea:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Letra-tipoaren lerromarraren gaineko desplazamendua</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Goi-indizearen tamaina erlatiboa letra-tipo normalarekiko</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Letra-tipoaren lerromarraren azpiko desplazamendua</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Azpi-indizearen tamaina erlatiboa letra-tipo normalarekiko</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Maiuskula txikien tamaina erlatiboa letra-tipo normalarekiko</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Azpimarra</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Desplazamendua:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Automatikoa</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Lerro-zabalera:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Marratua</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>&amp;Lerrotarte automatikoa</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Lerrotartea:</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Ehunekoa letra-tamaina handitzeko lerrotartean</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Letra-tipo normalaren lerromarraren azpiko desplazamendua letra-tamaina gutxitzeko ehunekotan adierazita</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Lerro-zabalera letra-tamainaren ehunekotan adierazita</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Letra-tipo normalaren lerromarraren gaineko desplazamendua letra-tamaina gutxitzeko ehunekotan adierazita</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Ezkerrean</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Eskuinean</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Puntua</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Koma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Erdian</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Posizioa:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Ezabatu denak</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Paragrafoko aurreneko lerroaren koska</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Paragrafo osoko koska ezkerretik</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Ezabatu tabultatzaile guztiak</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Bat ere ez</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Puntua</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Marra</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Azpimarra</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Pertsonalizatua</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Karakterea:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation type="obsolete">Pertsonalizatua:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation type="obsolete">Pertsonalizatua: </translation>
    </message>
    <message>
        <source>None</source>
        <comment>tab fill</comment>
        <translation type="unfinished">Bat ere ez</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>External Web Browser Failed to Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus was not able to start the external web browser application %1. Please check the setting in Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Eskema</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Elementua</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Taldea </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Objektu askeak</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Orrialdea </translation>
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
        <translation>Automatikoa</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Displacement</source>
        <translation>Desplazamendua</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Marra-zabalera</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Gehitu gida bertikala</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Gehitu gida horizontala</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Kendu gida bertikala</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Kendu gida horizontala</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Aldatu gida bertikala lekuz</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Aldatu gida horizontala lekuz</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Blokeatu gidak</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Desblokeatu gidak</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Lekuz aldatu</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Aldatu tamaina</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Biratu</translation>
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
        <translation>Z1: %1, A1: %2
Z2: %3, A2: %4</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Hautapena</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Elkartu</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Hautapena/Taldea</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Sortu</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
Z: %3, A: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Lerrokatu/Banatu</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Zerikusitako elementuak</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Utzi</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Ezarri kolorea</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>1. kolorea: %1, 1. kolorea: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Ezarri kolore-itzaldura</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Ezarri marra-kolorea</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Ezarri marra-kolorearen itzaldura</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Irauli horizontalki</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Irauli bertikalki</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Blokeatu</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Desblokeatu</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Blokeatu tamaina</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Desblokeatu tamaina</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Banandu</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Ezabatu</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Izena aldatu</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>%1(e)tik 
%2(e)ra</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Itsatsi</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Ebaki</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Ezarri kolorearen gardentasuna</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Ezarri marra-kolorearen gardentasuna</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Ezarri marra-estiloa</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Ezarri amaiera-marraren estiloa</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Ezarri lotura-marraren estiloa</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Ezarri marra-zabalera</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Estilorik ez</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Ezarri marra-estilo pertsonalizatua</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Ez erabili marra-estilo pertsonalizatua</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Ezarri hasierako gezia</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Ezarri amaierako gezia</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Sortu taula</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Err.: %1; Zut.: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Ezarri letra-tipoa</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Ezarri letra-tamaina</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Ezarri letra-zabalera</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Ezarri letra-kolorea</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Ezarrik letra-trazuaren kolorea</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Ezarri letra-kolorearen itzaldura</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Ezarri letra-trazuaren kolorearen itzaldura</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Ezarri karaktere-tartea</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Ezarri lerrotartea</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Ezarri paragrafo-estiloa</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Ezarri hizkuntza</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Lerrokatu testua</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Ezarri letra-efektua</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Irudiaren markoa</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Testu-markoa</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Poligonoa</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bezier kurba</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Koska-marra</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Bihurtu:</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Inportatu SVG irudia</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Inportatu EPS irudia</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Laneko area</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Testua markoaren inguruan</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Testua muga-koadro inguruan</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Testua ingerada-marra inguruan</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Testu-jariorik ez</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Muga-koadrorik ez</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Ingerada-marrarik ez</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>%1 orrialdea</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Ezarri irudiaren eskala</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Markoaren tamaina</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Eskalatze librea</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Mantendu aspektu-erlazioa</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Hautsi aspektu-erlazioa</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Editatu ingerada-marra</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Editatu forma</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Berrezarri ingerada-marra</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Aplikatu orri maisua</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Ezarri letra-altuera</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>Inportatu OpenOffice.org Draw irudia</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Gehitu orrialdea</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Gehitu orrialdeak</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Ezabatu orrialdea</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Ezabatu orrialdeak</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Gehitu geruza</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Ezabatu geruza</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Aldatu geruzaren izena</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Igo geruza</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Jaitsi geruza</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Bidali geruzara</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Gaitu geruza inprimatzea</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Desgaitu geruza inprimatzea</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Aldatu geruzaren izena</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Lortu irudia</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Testua bidean</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Gaitu elementua inprimatzea</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Desgaitu elementua inprimatzea</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Bikoizte anitza</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Aldatu irudiaren desplazamendua</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Aldatu irudiaren eskala</translation>
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
        <source>No object frame</source>
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
        <translation type="unfinished">&amp;Desegin</translation>
    </message>
    <message>
        <source>&amp;Redo: %1</source>
        <comment>f.e. Redo: Move</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Berregin</translation>
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
        <translation>Ekintzen historia</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Erakutsi hautatutako objektuak soilik</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desegin</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Berregin</translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation>Hasierako egoera</translation>
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
        <translation>%1 orri-tamainaren gutxieneko marjinak</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Erabili inprimagailuko marjinak</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>Hautatu &amp;inprimagailua:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Marjinak</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Eskuinean:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Goian:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Behean:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Ezkerrean:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Ados</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Utzi</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+U</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Txertatu balioa</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Sartu balioa, gero sakatu Ados.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Sartu balioa, gero sakatu Ados</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Bidali balioa script-ari</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Tresnak</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="obsolete">Propietateak...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation type="obsolete">PDF tresnak</translation>
    </message>
    <message>
        <source>Button</source>
        <translation type="obsolete">Botoia</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation type="obsolete">Testu-eremua</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation type="obsolete">Kontrol-laukia</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation type="obsolete">Konbinazio-koadroa</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation type="obsolete">Zerrenda-koadroa</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="obsolete">Txertatu PDF eremuak</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Testua</translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="obsolete">Esteka</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation type="obsolete">Txertatu PDF oharpenak</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Aukeratu inportatzailea</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatikoa</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Inportatu testua formaturik gabe</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Inportatzailea:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodeketa:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Inportatu testua soilik</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Ireki</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Aukeratu inportatzailea</translation>
    </message>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Gogoratu elkartasuna</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Gogoratu fitxategi-luzapena eta inportatzailearen arteko elkartasuna, eta ez 
galdetu berriro inportatzailea hautatzeko fitxategi-mota honentzako.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Berria txantiloitik</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Kendu</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Ireki</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Denak</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Orrialde-tamaina</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Koloreak</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Azalpena</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Erabilera</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Sorrera-tresna:</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Egilea</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Txantiloiak deskargatzen</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Txantiloiak deskargatzeko atalean aurki ditzakezu honako gunean: 
http://www.scribus.net</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Txantiloiak instalatzea</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Erauzi paketea uneko erabiltzailearen txantiloien ~/.scribus/templates direktorioan edo AURREFINKATUA/share/scribus/templates sistemako 
erabiltzaile guztientzat.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Txantiloia prestatzen</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Ziurtatu irudiak eta letra-tipoak libreki erabil ditzakezula. Letra-tipoak ezin badira partekatu, ez itzazu bildu txantiloi gisa gordetzean.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Txantiloiaren sortzaileak ziurtatu behar du arestiko &apos;Txantiloiak instalatzea&apos; 
atalak beraren txantiloiei ere aplikatzen zaiela. Honek zera adierazten du: 
erabiltzaile batek gai izan behar duela txantiloi pakete bat deskargatzeko, 
txantiloien direktorioan erauzi eta erabiltzeko.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Txantiloia kentzen</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>&apos;Berria txantiloitik&apos; elkarrizketa-koadrotik txantiloi bat kentzean template.xml fitxategiko sarrera ezabatzen du, ez ditu dokumentuak ezabatzen. Kentzeko laster-menua bakarrik erakutsiko da baldin eta erabiltzaileak template.xmlk fitxategian idazteko baimena badu.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>template.xml itzultzen</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Existiten den &apos;template.xml&apos; fitxategia template.hizk_ESTATUA.xml fitxategian 
kopiatzen du (hizk hizkuntzaren kodea da, adib. &apos;eu&apos; euskararen kodea 
litzateke), adibidez template.eu.xml. Kopiatutako fitxategia 
jatorrizko &apos;template.xml&apos; fitxategiaren direktorio berdinean kokatu behar du, 
Scribus-ek kopiako fitxategia kargatzeko.</translation>
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
        <translation type="unfinished">Kendu</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">Ados</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Utzi</translation>
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
        <translation>Gorde txantiloi gisa</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Izena:</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategoria</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Orrialde-tamaina</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Koloreak</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Azalpena</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Erabilera</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Egilea</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Helb. el.</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Xehetasun gehiago</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Ados</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Xehetasun gutxiago</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legala</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Gutuna</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloidea</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>horizontala</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>bertikala</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>pertsonalizatua</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Sortu iragazkia</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Garbitu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Ezabatu</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Aukeratu aurrez gordetako iragazkia</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Eman iragazkiaren izena gordetzeko</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Eman izena gordetzeko</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Desgaitu edo gaitu iragazkia</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Kendu iragazkia</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Gehitu iragazki berria</translation>
    </message>
    <message>
        <source>to</source>
        <translation>nora</translation>
    </message>
    <message>
        <source>and</source>
        <translation>eta</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>kendu bateragarria</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>ez kendu bateragarria</translation>
    </message>
    <message>
        <source>words</source>
        <translation>hitzak</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Kendu</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Ordeztu</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplikatu</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Ezkerreko balioa adierazpen erregularra da</translation>
    </message>
    <message>
        <source>with</source>
        <translation>honekin</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>paragrafo-estiloa</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>honen instantzia guztiak:</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>paragrafo guztiak</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>paragrafoak hasierarekin:</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>paragrafoak gutxiago baino:</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>paragrafoak gehiago baino:</translation>
    </message>
</context>
</TS>
