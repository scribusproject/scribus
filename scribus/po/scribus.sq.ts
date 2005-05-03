<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Përgjigjet me një listë që përmban emrat e tërë ngjyrave të përcaktuara për dokumentin.
Nëse nuk është hapur ndonjë dokument, përgjigjet me një listë ngjyrash për dokumentin parazgjedhje.
</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Shfaq kutinë e Dialogut &quot;Dokument i Ri&quot;. Krijon një dokument te ri nëse përdoruesi
pranon rregullimet. Nuk krijon dokument nëse përdoruesi shtyp Anulo.
Përgjigjet me &quot;true&quot; nëse dokumenti i ri u krijua.
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

Krijon një dokument të ri dhe përgjigjet me një &quot;true&quot; nëse ka sukses. Parametrat kanë 
domethënien vijuese:

    size = Një &quot;tuple&quot; (gjerësi, lartësi) që përshkruan madhësinë e dokumentit. Mund
    të përdorni konstante të paracaktuara të emërtuara PAPER_&lt;lloj_letre&gt; p.sh. PAPER_A4 etj.

    margins = Një &quot;tuple&quot; (majtas, djathtas, sipër, poshtë) që përshkruan mënjanat 
    e dokumentit

    orientation = drejtimi i faqes - constants PORTRET, SË GJERI

    firstPageNumber = është numri i faqes së parë te dokumenti përdorur për
    numërim faqesh. Edhe pse do ta donit zakonisht 1, është e dobishme të keni vlera
    më të mëdha nëse krijoni një dokument me disa pjesë.

    unit: kjo vlerë cakton njësitë matëse të përdorura nga dokumenti. Përdorni për
    këtë një konstante të paracaktuar, një prej: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    facingPages = FACINGPAGES, NOFACINGPAGES

    firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

Vlerat për gjerësi, lartësi dhe mënjana shprehen në njësinë e dhënë për
dokumentin. Konstantet PAPER_* shprehen në pikë. Nëse dokumenti juaj
nuk është në pikë, bëni ndryshimin përkatës.

shembull: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Përgjigjet me emrin e ngjyrës së mbushjes për objektin &quot;emër&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;])

Zhvendos objektin &quot;emër&quot; me dx dhe dy në lidhje me pozicionin e vet të çastit. 
Hapësirat shprehen në njësinë matëse të çastit të dokumentit (shihni
konstantet UNIT). Nëse nuk është dhënë &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
Nëse objekti &quot;emër&quot; i përket një grupi, zhvendoset tërë grupi.
</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Çaktivizon rivizatimin e faqes kur bool = False, përndryshe rivizatimi është
veprues. Ky ndryshim do të mbesë edhe pasi programthi të përfundojë, ndaj 
sigurohuni të thirret setRedraw(True) në një &quot;finally: clause&quot; te niveli sipëror 
i programthit tuaj.
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
        <translation>createRect(x, y, gjerësi, lartësi, [&quot;emër&quot;]) -&gt; string

Krijon një drejtkëndësh të ri në faqen e çastit dhe jep emrin e tij. Koordinatat
jepen në njësinë matëse të çastit për dokumentin (shihni konstantet UNIT). 
&quot;emër&quot; do të duhej të ishte një identifikues unik për objektin sepse
keni nevojë që ky emër të lidhet në të ardhmen me këtë objekt. Nëse nuk
jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të shfaqet NameExistsError nëse jepni shprehimisht një emër tashmë në përdorim.
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
        <translation>newPage(ku [,&quot;stampë&quot;])

Krijon një faqe të re. Nëse &quot;ku&quot; është -1 faqja e re shtohet te dokumenti, 
përndryshe faqja e re futet përpara &quot;ku&quot;. Numrat e faqeve numërohen
prej 1 e sipër, pa pyetur se çfarë numri tregon faqja e parë e dokumentit tuaj. 
Parametri opsional &quot;stampë&quot; përcakton emrin e faqes stampë për faqen e re.

Mund të shfaqë IndexError nëse numri i faqes zgjidhet është jashtë intervalit
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(tip, &quot;ngjyrë1&quot;, hije1, &quot;ngjyrë2&quot;, hije2, [&quot;emër&quot;])

Rregullon mbushjen e shkallëzuar të objektit &quot;emër&quot; sipas tipit. Përshkrimi i ngjyrave
është i njëjtë si për setFillColor() dhe setFillShade(). Shihni konstantet për tipe të
mundshëm (FILL_&lt;type&gt;).
</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;emër&quot;]) -&gt; float

Përgjigjet me madhësinë e gërmave për kornizën e teksti me &quot;emër&quot;. Nëse kjo 
kornizë teksti ka tekst të përzgjedhur sillet vlera e dhënë gërmës së parë te
përzgjedhja.
Nëse nuk tregohet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.
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
        <translation type="obsolete">register_macro_callable(emër, callable, përshpejt=&apos;&apos;)

Krijon një makro të quajtur &quot;emër&quot; me objekt të thirrshëm ekzistues &quot;callable&quot;.
I thirrshmi që jepet duhet të mos kërkojë ndonjë argument kur thirret (mund 
të marrë argumente opsionalë, por nuk do t&apos;i jepet ndonjë).
Nëse jepet një, vargu &quot;përshpejt&quot; do të përdoret për të caktuar një shkurtprerje
tastiere për makron.
Nëse i thirrshmi i dhënë është një klasë, nuk do të pranohet. Funksionet dhe 
metoda &quot;bound&quot; janë goxha të pranueshëm, siç janë edhe instanca klasash që
ofrojnë një __call__ method pa argumente. Nuk ka problem nëse regjistrohet
një i thirrshëm më shumë se një herë, as nëse regjistrohen metoda &quot;bound&quot;
të shumëfishta brenda një instance të vetme.</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;varg&quot;)

Shkruan &quot;vargun&quot; te shtyllë mesazhesh të Scribus-it (rresht gjendjeje). Teksti
duhet të jetë i koduar si UTF8 ose varg &apos;unicode&apos; (e këshillueshme).
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
        <translation>newDoc(madhësi, mënjana, orientim, firstPageNumber,
                   njësi, facingPages, firstSideLeft) -&gt; bool

Krijon një dokument të ri dhe përgjigjet me true nëse ka sukses. Parametrat kanë
domethëniet vijuese:

    madhësi = Një dyshe (gjerësi, lartësi) që përshkruan madhësinë e dokumentit. Mund të përdorni
    konstante të parapërcaktuara të emërtuara PAPER_&lt;tip_letre&gt; p.sh. PAPER_A4 etj.

    mënjana = Një katërshe (majtas, djathtas, krye, fund) që përshkruan mënjanat
    e dokumentit

    orientim = orientim i faqes - konstante PORTRAIT, LANDSCAPE

    firstPageNumer = është numri i faqes së parë te dokumenti i përdorur për
    numërtim faqesh. Ndërkohë që zakonisht do ta donit 1, është e dobishme të keni numra
    më të mëdhenj nëse po krijoni një dokument me disa pjesë.

    njësi: kjo vlerë përcakton njësitë matëse përdorur nga dokumenti. Për këtë përdorni
    një konstante të parapërcaktuar, një nga: UNIT_INCHES, UNIT_MILLIMETERS,
    UNIT_PICAS, UNIT_POINTS.

    facingPages = FACINGPAGES, NOFACINGPAGES

    firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

Vlerat për gjerësinë, lartësinë dhe mënjanat shprehen në njësitë e dhëna
për dokumentin. Konstantet PAPER_* shprehen në pikë. Nëse dokumenti juaj
nuk është në pikë, sigurohuni që i bëni llogaritë në pikë.

shembull: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
</translation>
    </message>
</context>
<context>
    <name>@default</name>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
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
        <source>G:</source>
        <translation type="obsolete">G:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="obsolete">K:</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="obsolete">M:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Jo</translation>
    </message>
    <message>
        <source>R:</source>
        <translation type="obsolete">R:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="obsolete">Y:</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation type="obsolete">&amp;&lt;&lt;</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation type="obsolete">&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Jo</translation>
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
        <translation type="obsolete">Tërë</translation>
    </message>
    <message>
        <source>Alt</source>
        <translation type="obsolete">Alt</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Kuti</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Pri</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation type="obsolete">Dre</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="obsolete">GUI</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Ulët</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">e Re</translation>
    </message>
    <message>
        <source>Old</source>
        <translation type="obsolete">I vjetër</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="obsolete">RGB</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">Për:</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation type="obsolete">X1:</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation type="obsolete">X2:</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation type="obsolete">Y1:</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation type="obsolete">Y2:</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Përdor</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Po</translation>
    </message>
    <message>
        <source>Zip</source>
        <translation type="obsolete">Zip</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="obsolete">min</translation>
    </message>
    <message>
        <source>sum</source>
        <translation type="obsolete">mbledhje</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">për:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation type="obsolete">Vendmbrritje</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete"> dpi</translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="obsolete"> pt </translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete">sek</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Shto</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation type="obsolete">&amp;X1:</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation type="obsolete">&amp;Y2:</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Po</translation>
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
        <source>A&amp;dd</source>
        <translation type="obsolete">Sht&amp;o</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="obsolete">CMYK</translation>
    </message>
    <message>
        <source>Alt+</source>
        <translation type="obsolete">Alt+</translation>
    </message>
    <message>
        <source>Back</source>
        <translation type="obsolete">Prapa</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopjo</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">P&amp;rij</translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="obsolete">Datë</translation>
    </message>
    <message>
        <source>Ctrl</source>
        <translation type="obsolete">Ctrl</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="obsolete">Cyan</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Kartelë</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Gërma</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation type="obsolete">Shko te</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Lartë</translation>
    </message>
    <message>
        <source>JPEG</source>
        <translation type="obsolete">JPEG</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation type="obsolete">Ikonë</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">O&amp;ff</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="obsolete">Majtas</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Rresht</translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="obsolete">Lidhje</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Kyç</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Emër</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Asnjë</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Hap</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Faqe</translation>
    </message>
    <message>
        <source>Path</source>
        <translation type="obsolete">Shteg</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Lër</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Ruaj</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="obsolete">Madhësi</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation type="obsolete">Anashkalo</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Po dal tani</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation type="obsolete">Rendit</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation type="obsolete">X&amp;2:</translation>
    </message>
    <message>
        <source>Star</source>
        <translation type="obsolete">Yllkë</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation type="obsolete">E ngushtë</translation>
    </message>
    <message>
        <source>Time</source>
        <translation type="obsolete">Kohë</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation type="obsolete">Y&amp;1:</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tip</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Zhbëj</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation type="obsolete">E gjerë</translation>
    </message>
    <message>
        <source>html</source>
        <translation type="obsolete">html</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Vendos Djathtas</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Mbyll Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation type="obsolete">Line out</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation type="obsolete">Shkon te Drejtori juaja Dokumentesh.
Kjo mund të rregullohet te Parapëlqimet.</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation type="obsolete">Urdhër &amp;Alternativ Shtypësi</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Hequrvije</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation type="obsolete">&amp;Merr Emra Fushe</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Vendos në Qendër</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Paraparje Shtypjeje</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation type="obsolete">Build-ID:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Ngjyrë M&amp;bushje:</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="obsolete">&amp;Gërma</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">&amp;Boshllëk</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Ndihmë</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Majtas</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="obsolete">&amp;Rresht</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Blloko</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation type="obsolete">&amp;Zhvendos</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation type="obsolete">&amp;Hap</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Faqe</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation type="obsolete">E &amp;kuqe:</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="obsolete">&amp;Ribëj</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Lër</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Madhësi</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation type="obsolete">&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Parje</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Stile...</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation type="obsolete">(TAB)</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation type="obsolete">&amp;Ngjesh Kartelë:</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation type="obsolete">Përdor Simbol Paraje</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">V&amp;eti Shënimesh...</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Pamje:</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation type="obsolete">Parashtro Formular</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Shpërnda/Drejto...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation type="obsolete">&amp;Përditëso Kornizë Teksti dhe Dil</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation type="obsolete">Kufi i </translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Përditëso Kornizë Teksti dhe Dil</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Drejto</translation>
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
        <source>Apply</source>
        <translation type="obsolete">Zbato</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="obsolete">Zi</translation>
    </message>
    <message>
        <source>Block</source>
        <translation type="obsolete">Blloko</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation type="obsolete">Kartolina</translation>
    </message>
    <message>
        <source>Check</source>
        <translation type="obsolete">Kontrollo</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Fshij</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Mbyll</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Ngjyrë</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation type="obsolete">Presje</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation type="obsolete">Kryq</translation>
    </message>
    <message>
        <source>Ctrl+</source>
        <translation type="obsolete">Ctrl+</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Çeke</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Holandeze:</translation>
    </message>
    <message>
        <source>Email</source>
        <translation type="obsolete">Email</translation>
    </message>
    <message>
        <source>Error</source>
        <translation type="obsolete">Gabim</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation type="obsolete">Në Fokus</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation type="obsolete">Fletëpalosje</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="obsolete">Gërma:</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation type="obsolete">Shko Tek</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Greke</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Grup</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation type="obsolete">Ikona</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">Të dhë&amp;na</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Pamje</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation type="obsolete">Zhvendos prapa</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation type="obsolete">Minimizo</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Irlandeze</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation type="obsolete">K&amp;ulme
të Rrumbullakët:</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Përditëso Pamje</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation type="obsolete">Shtresë</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Nivel</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">I poshtëm</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Menu</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Të ndryshme</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Emër:</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation type="obsolete">Vetëm Faqe Çift</translation>
    </message>
    <message>
        <source>Never</source>
        <translation type="obsolete">Kurrë</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finlandeze:</translation>
    </message>
    <message>
        <source>Nodes</source>
        <translation type="obsolete">Nyje</translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="obsolete">Faqe</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="obsolete">Faqe:</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Ngjit</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="obsolete">Shtigje</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Tjetër</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation type="obsolete">Thjesht</translation>
    </message>
    <message>
        <source>Print</source>
        <translation type="obsolete">Shtyp</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Ngri</translation>
    </message>
    <message>
        <source>Range</source>
        <translation type="obsolete">Interval</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation type="obsolete">Profil ngjyrash i prodhuar nga ju apo marrë prej prodhuesit.
Ky profil do të duhej të ishte i veçantë për monitorin tuaj dhe jo një profil i përgjithshëm (bie fjala sRGB).</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="obsolete">Gati</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Vini ngjyra prej një dokumenti ekzistues te seti i çastit</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation type="obsolete">Rimerr</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="obsolete">Djathtas</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">e Rrumbullakët</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Pikë (pts)</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="obsolete">Hije</translation>
    </message>
    <message>
        <source>Shift</source>
        <translation type="obsolete">Shift</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Brenda:</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation type="obsolete">Tabela</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Madhësi:</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">E vogël</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation type="obsolete">e Plotë</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Ndaj</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">më mirë</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Stil</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation type="obsolete">Si&amp;për:</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Mjete</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="obsolete">Tip:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Njësi</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Vlerë</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation type="obsolete">Përdorim</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Pastro tërë Tekstet</translation>
    </message>
    <message>
        <source>Width</source>
        <translation type="obsolete">Gjerësi</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="obsolete">&amp;Emër i të Ekzekutueshmit:</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">Stile &amp;Vije...</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation type="obsolete">Po gatis...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Përpuno Tekst...</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation type="obsolete">Qeth Shtegun Vertikalisht Poshtë</translation>
    </message>
    <message>
        <source>Align Text Left</source>
        <translation type="obsolete">Vendos Tekstin Majtas</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Shpjere Prapa</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation type="obsolete">&amp;Mundësi...</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation type="obsolete">Madhësi faqeje dokumenti, ose si madhësi standard ose si madhësi vetjake</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation type="obsolete">&amp;Ruaj te Kartelë...</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Çaktivizuar</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Programet vijues mungojnë:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Profil Për&amp;fundimesh:</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation type="obsolete">Mbaj ndryshim të X dhe Y njësoj</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation type="obsolete">Kuti Liste</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Përpuno tekstin me Përpunues Historie</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Italisht</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation type="obsolete">Shkruaj: %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Doni vërtet të humbasin tërë Ndryshimet tuaj?</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Tekst për tu vizatuar</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation type="obsolete">Përdor Simbol Paraje</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation type="obsolete">Rregullime Stili</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation type="obsolete">Pasqyro Shtegun Horizontalisht</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Importoni Faqe(t):</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">Shkurtprerje &amp;Tastiere...</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Ç&amp;grupo</translation>
    </message>
    <message>
        <source>Check &amp;During Typing</source>
        <translation type="obsolete">Kontrollo &amp;Gjatë Shtypjes</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="obsolete">Mundësi të Tjera</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="obsolete">Taste të Përcaktuar nga &amp;Përdoruesi</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">&amp;Veti Fushe</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Përdor Profil ICC</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">mungon!</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Veti Vizatimi Shumëkëndëshash</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Lejo &amp;Ndryshimin e Dokumentit</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation type="obsolete">Po importoj tekst</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="obsolete">Ruaj Tekst në Kartelë</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation type="obsolete">&amp;Fjalëkyça:</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Prodho Artikuj PDF, çka është e dobishme për lundrim artikujsh në një PDF.</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation type="obsolete">&amp;Përcaktoni Ngjyra Vetjake &gt;&gt;</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation type="obsolete">Dokument i Ri</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Trupëzo profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation type="obsolete">Profil ngjyrë për modelin tuaj të shtypësit nga prodhuesi.
Ky profil do të duhej të ishte i veçantë për shtypësin tuaj dhe jo një profil i përgjithshëm (bie fjala sRGB).</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Braziliane:</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation type="obsolete">Veti Teksti Shtegu</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Përdor kuti rrethuese në vend të formës së kornizës për rrjedhë teksti</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="obsolete">Qeth Shtegun Horizontalisht Majtas</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation type="obsolete">Përpunues Historie</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horizontalisht</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation type="obsolete">Format Numrash</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Jashtëvijëzo Tekst</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation type="obsolete">Ndrysho rregullime për pika majtas ose në fund </translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopjo #%1 nga</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation type="obsolete">Tekst dhe Figura</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation type="obsolete">N&amp;umër Kopjesh</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Po marr Profile ICC</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Rregullimet në Fuqi</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation type="obsolete">Horizontalisht</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation type="obsolete">Kontrollo Stil:</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Koordinata &amp;Absolute</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Zmadho ose zvogëlo</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Po gatis Shtojca</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Shto Grup</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation type="obsolete">Rendit sipas &amp;Madhësisë</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">Po zh&amp;vendos Rreshta:</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation type="obsolete">Rendit sipas &amp;Datës</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation type="obsolete">Rendit sipas &amp;Emrit</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation type="obsolete">Në Kartelë</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="obsolete">Programthe &amp;Scribus</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation type="obsolete">Pa Shtypje</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="obsolete">Pa Stil</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation type="obsolete">Pa Titull</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">&amp;Faqe Majtas fillimisht</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Vijë e Mençur</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation type="obsolete">Dosje e Re 1</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">Emri i Ngjyrës nuk është i vetëm</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Qartësi:</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">&amp;Përdor Kodim</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Version Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Largësi ndërmjet shtyllash</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="obsolete">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation type="obsolete">Kënd Rrotullimi</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation type="obsolete">Rri &amp;Sipër</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation type="obsolete">Vertikalisht</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">Kartela EPS (*.eps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Mailing list</source>
        <translation type="obsolete">Listë postimesh</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Shfaq &amp;Konsol</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Kornizë &amp;Pamjeje</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">M&amp;byll Sidoqoftë</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">Emri i Stilit nuk është i vetëm</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">Të &amp;dhëna Varg:</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation type="obsolete">Përdor Radhitjen DNM</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Para&amp;parje Shtypjeje</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Lejo ndryshimin e PDF-së. Nëse nuk shenjohet, parandalohet ndryshimi i PDF-së.</translation>
    </message>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Gërma që Mungojnë</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation type="obsolete">Eksporto si Pamje</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation type="obsolete">Rregullime Gërmash</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation type="obsolete">Formate të mundshëm eksporti</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Shto tek stampë</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation type="obsolete">Rregullime Administrimi Ngjyrash</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="obsolete">Sfond</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation type="obsolete">Zgjidhni Drejtori Eksportimi</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Ngjyrë Udhëzuesash &amp;Përdoruesi:</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation type="obsolete">Lajmërime</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation type="obsolete">Rrotullo Shtegun në Kah Antiorar</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation type="obsolete">Mjete të Jashtëm</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Doni vërtet të pastroni tërë Tekstin tuaj?</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation type="obsolete">&amp;Shfaq CMYK</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Kur zgjidhet përdoren Koordinata relative te Faqja,
përndryshe Koordinatat janë relative te Objekti.</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Importoni Faqe(t)</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation type="obsolete">Web Safe RGB</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Fut Special...</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation type="obsolete">Stil Paragrafi</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation type="obsolete">Të dhëna titujsh për pamje ISO9660 </translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">Programthe së &amp;Fundmi:</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation type="obsolete">Largësi teksti nga fundi i kornizës</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Zgjidhni fjalëkalim të përgjithshëm i cili aktivizon ose çaktivizon tërë
veçoritë sigurisë për PDF tuajat të eksportuara</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">Pa&amp;mje e Dukshme</translation>
    </message>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Përzgjidhni Gërma:</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation type="obsolete">Lëviz Faqerojtës</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Shfaq paraparje faqeje për secilën nga faqet e listuara më sipër. </translation>
    </message>
    <message>
        <source>Reverse Writing</source>
        <translation type="obsolete">Shkrim i ndërprerë...</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Anë të Sipërme</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation type="obsolete">Lidhje Web e Jashtme</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">Ndërro Ngjyrën e Sfondit</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation type="obsolete">drejtoria</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation type="obsolete">&amp;Tërë Fjalën</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation type="obsolete">S&amp;hfaq rishikime</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> e %1</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation type="obsolete">Përpunoni &amp;Rrethina Vije</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation type="obsolete">Parashtro te URL:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation type="obsolete">Majë e Sheshtë</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation type="obsolete">Rregullimet Shenjash</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation type="obsolete">&amp;Relacion:</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Aktivizoni veçori sigurie për PDF tuajat të eksportuara.
Nëse përzgjodhët Acrobat 4.0, PDF-ja do të mbrohet me kodim 40 bit.
Nëse përzgjodhët Acrobat 5.0, PDF-ja do të mbrohet me kodim 128 bit.
Disclaimer: Kodimi PDF nuk është aq i besueshëm sa kodimi GPG ose PGP dhe ka ca kufizime.</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="obsolete">URL për paraqitje</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation type="obsolete">Shkallëzim Radial</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Administro &amp;Udhëzuesa...</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation type="obsolete">Reklama</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation type="obsolete">Skemë:</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation type="obsolete">Stampa Vetjake</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">Caktoni rrezen e lakimit të kulmit</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation type="obsolete">T&amp;ip kartele:</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Eksporto Interval</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">Z&amp;gjidhni Faqe</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation type="obsolete">E&amp;mër kartele:</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="obsolete">&amp;Hiq numra rreshtash</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation type="obsolete">Vetëm me Ftesa</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Merr vetëm tekst</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation type="obsolete">&amp;Rreth</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Zbato</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Blloko</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation type="obsolete">Qartësi Pamjesh
Përdorni 72 dpi për Pamje të paramenduara për Ekranin</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Ngjyrë</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Mundësi Kartele</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Gërmat %1 nuk janë instaluar.</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation type="obsolete">Llogarit</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation type="obsolete">&amp;Kartelë:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Gërma:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Gërma</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Grup</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation type="obsolete">Hapësirë sipër paragrafit</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="obsolete">Anti-alias &amp;Grafikë</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation type="obsolete">&amp;Pamje</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Trupëzo në dokumetin tuaj faqerojtësit e krijuar.
Këta janë të dobishëm për të udhëtuar në dokumente të gjatë PDF.</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Ul</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Parasheh tërë faqet e dokumentit tuaj.</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation type="obsolete">Shkallëzim Horizontal</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Emër:</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Ngjit</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Mënjanë Djathtas</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation type="obsolete">&amp;Shtyp</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Ngri</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation type="obsolete">&amp;Interval</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">&amp;Djathtas</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">&amp;Hije</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Formë</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Madhësi:</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="obsolete">E &amp;vogël</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="obsolete">&amp;Mjete</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Përdorues:</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://scribus.net for updated docs and downloads.</source>
        <translation type="obsolete">Më ndjeni, nuk ka doracak të mundshëm! Ju lutem shihni: http://scribus.net për dokumente të përditësuar dhe shkarkime.</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Lejo Kop&amp;jim Tekstesh dhe Grafikash</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation type="obsolete">Kalendarë</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation type="obsolete">Gjatësi:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="obsolete">N&amp;gjesh Tekst dhe Grafikë Vektorialë</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Fshij...</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Zhvendosje:</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation type="obsolete">Fshij %1</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Fshij...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">Stile &amp;Paragrafi...</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation type="obsolete">Rirregulloni këtë Pikë Kontrolli</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Pasqyro Faqe(t) Vertikalisht</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Lartësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation type="obsolete">Ngjyrë sfondi:</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Bjer &amp;Përpara</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation type="obsolete">Rock &amp; Roll</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation type="obsolete">Ndalesë e Plotë</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation type="obsolete">Vlera nuk është vleftësuar</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Është &amp;Faqerojtës PDF</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="obsolete">&amp;Krijo dhe Ndrysho</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation type="obsolete">Tejdukshmëri me nuancë (hije)</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation type="obsolete">%1 - [%2]</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Vendos rrjetën pas objekteve të faqes</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galike:</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Stampa...</translation>
    </message>
    <message>
        <source>Line up</source>
        <translation type="obsolete">Ngjitu</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Përdorni një përgjegjës shtypjesh alternativ, si kprinter ose gtklp,
për të bërë të përdorshëm mundësi shtypjesh shtesë</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Krijon vetëm PostScript Level 2, mbani parasysh,
që kjo mund të krijojë kartela të stërmëdha</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation type="obsolete">S&apos;mund të vendos strukturë colormap</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Kroji Kartelë PDF</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Krijoni ngjyrë të re brenda setit të çastit</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="obsolete">Gërma për trupëzim</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Kyç &amp;Madhësi Objekti</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Ruaj &amp;Tekst...</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Dyfisho CD</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="obsolete">Kërko/Zëvendëso</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">Ripërmasim i &amp;Lirë</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation type="obsolete">&amp;Ngarko...</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Fut Vija</translation>
    </message>
    <message>
        <source>Choose the importer to use</source>
        <translation type="obsolete">Zgjidhni importuar për t&apos;u përdorur</translation>
    </message>
    <message>
        <source>Programming:</source>
        <translation type="obsolete">Programim:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation type="obsolete">B&amp;otues:</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Shfaq Paraparje Faqeje</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Fut Tabelë</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Fut Pamje</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation type="obsolete">Përpunoni rregullimet tab për kornizë tekstesh...</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Ngjyrë mbushjeje për korniza pamjesh</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Ngjyra...</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation type="obsolete">Largësi prej Kurbe:</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">Ngj&amp;yra...</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation type="obsolete">Përpunoni &amp;Rrethina Vije</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">&amp;Shqit Tekst prej Shtegu</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Krijon vetëm PostScript Level 1, mbani parasysh,
që kjo mund të krijojë kartela të stërmëdha</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Veti Vizatimi Formash</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="obsolete">Magenta</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="obsolete">Zgjidhni një set njgyrash për t&apos;u ngarkuar</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation type="obsolete">Faqe për Fletë</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation type="obsolete">më i madh</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Më i madhi</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation type="obsolete">Po formatoj</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation type="obsolete">Listë Kuti</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Stampë (Faqe Majtas):</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation type="obsolete">Kartela Vlerash Ndarë me Presje (CSV)</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation type="obsolete">Emër i stilit tuaj për paragrafin</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation type="obsolete">&amp;Pjesëmarrës:</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation type="obsolete">Pikë së cilës i referehon matjet ose këndet e rrotullimeve</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation type="obsolete">Shmangje vertikale e pamjes brenda kornizës</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation type="obsolete">Zhvendos Faqe:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation type="obsolete">Programth vetjak vleftësimi:</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Po kërkoj për Gërma</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation type="obsolete">Largësi ndërmjet shtyllash</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Fshij prej:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">Në Sfond:</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation type="obsolete">Sht&amp;o...</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metodë:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">Ripërmasim &amp;Horizontal:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">Funksion i panjohur</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Oook! Po thërrisni një objekt që nuk ekziston!</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation type="obsolete">Profil burim i pamjes</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Vargu i detyrueshëm për PDF/X-3 ose PDF do të dalë
përputhjes me PDF/X-3. Ju këshillojmë të përdorni titullin e dokumentit.</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Nga e Majta në të Djathtë</translation>
    </message>
    <message>
        <source>Delete Color</source>
        <translation type="obsolete">Fshij Ngjyrë</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation type="obsolete">Ripërmaso pamjen vertikalisht</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Nga e Djathta në të Majtë</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Elementë të përzgjedhur</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation type="obsolete">Rregullim Dokumenti</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Gabim programthi</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation type="obsolete">Përzgjidhni ngjyrë</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation type="obsolete">Profile Sistemi</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation type="obsolete">Fshij Nyje</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation type="obsolete">Fshij Faqe</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Programth vetjak vleftësimi:</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">Mundësi të Mët&amp;ejshme...</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Prej Dokumenti:</translation>
    </message>
    <message>
        <source>Edit Line Styles</source>
        <translation type="obsolete">Përpunoni Stile Vije</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation type="obsolete">Fshij Shtresë</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Zëvendësoje me</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation type="obsolete">më i vogël</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Më i vogli</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation type="obsolete">Tekst dhe Figura</translation>
    </message>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Rregullime Ndarësi</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation type="obsolete">Mungon</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation type="obsolete">Rrotullo Shtegun në Kah Orar </translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Të dhëna Dokumenti...</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation type="obsolete">Koordinata &amp;Absolute</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Ul Shtresë</translation>
    </message>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation type="obsolete">Importimi i Kartelës:
%1
dështoi!</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Profil parazgjedhje ngjyrash për pamje të importuara</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">D&amp;rejto</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation type="obsolete">Ngjyra Të plota</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Ngjyra të Plota</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation type="obsolete">&amp;Shto tek Ngjyra Vetjake</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Gabim në shkrim kartele(ash) përfundim.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Lejo shtypjen e PDF-së. Nëse nuk shenjohet, parandalohet shtypja.</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">&amp;Tip Vije:</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">Pa&amp;stro</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">Ngj&amp;yrë</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation type="obsolete">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">Rpërmasim &amp;Vertical:</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation type="obsolete">Metodë mbushjeje normale ose me shkallëzim</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Zhvendos...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Zhvendos...</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Stampë:</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Stil vijash</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Hiqni ngjyra të papërdorura prej seti ngjyrash të dokumentit të çastit</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation type="obsolete">Shkallëzim linear i lirë</translation>
    </message>
    <message>
        <source>Importer</source>
        <translation type="obsolete">Importues</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation type="obsolete">Prano</translation>
    </message>
    <message>
        <source>Action</source>
        <translation type="obsolete">Veprim</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">Dr&amp;ejto</translation>
    </message>
    <message>
        <source>Do you really want do delete this Style?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Stil?</translation>
    </message>
    <message>
        <source>Always</source>
        <translation type="obsolete">Përherë</translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="obsolete">Kënd:</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Interpretues PostScript</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation type="obsolete">Vendos Tekstin në Qendër</translation>
    </message>
    <message>
        <source>Author</source>
        <translation type="obsolete">Autor</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation type="obsolete">Bl&amp;u:</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation type="obsolete">Drejtim i faqeve të dokumentit</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">Kartela PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Border</source>
        <translation type="obsolete">Anë:</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Fshih Korniza</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation type="obsolete">Format Date</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="obsolete">Qendër</translation>
    </message>
    <message>
        <source>Button</source>
        <translation type="obsolete">Buton</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation type="obsolete">Rreth</translation>
    </message>
    <message>
        <source>Conve&amp;x Polygon</source>
        <translation type="obsolete">Mysë&amp;to Shumëkëndësh</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="obsolete">Ngjyrë:</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="obsolete">Ngjyra</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation type="obsolete">Vendos Tekstin Përligjshëm</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation type="obsolete">Veti Fushe</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Daneze</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation type="obsolete">Dat&amp;ë:</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation type="obsolete">me Vija</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Krijo</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Fshij</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Vetiake</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation type="obsolete">Format Kohore</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation type="obsolete">&amp;Funde:</translation>
    </message>
    <message>
        <source>&amp;Number of Hypenations allowed:</source>
        <translation type="obsolete">&amp;Numër i lejuar Ndarjesh me Vijë:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Çeke:</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Hapësirë pas Shtylle</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="obsolete">Së gjeri</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation type="obsolete">së gjeri</translation>
    </message>
    <message>
        <source>Editor</source>
        <translation type="obsolete">Përpunues</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Pa Efekt</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Ngjyrë vije formash</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">Ka paletë</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Mos përdor profile ICC të trupëzuar</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Zh&amp;vendosje:</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation type="obsolete">R&amp;ingarko</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation type="obsolete">Ngjarje:</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation type="obsolete">Vlera është</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Fshih Pamje</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Mundësi &amp;PDF</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Kartelë:</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation type="obsolete">Dosje e Re</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation type="obsolete">Vijë Poshtë</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Ngjyra Faqeje</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Ngjyrë mbushjeje për forma</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation type="obsolete">Konsol Programthesh</translation>
    </message>
    <message>
        <source>Format</source>
        <translation type="obsolete">Format</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">Hungareze</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Frënge</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Importo &amp;Faqe(t)...</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation type="obsolete">Gjerësi shtylle</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Gjermane</translation>
    </message>
    <message>
        <source>English:</source>
        <translation type="obsolete">Angleze:</translation>
    </message>
    <message>
        <source>Do you really want do delete this Script?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Programth?</translation>
    </message>
    <message>
        <source>Group </source>
        <translation type="obsolete">Grup</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="obsolete">&amp;Gjuhë:</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="obsolete">Udhëzuesa</translation>
    </message>
    <message>
        <source>Height</source>
        <translation type="obsolete">Lartësi</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Ngjyra Rrjete</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation type="obsolete">Fusha të Mundshme</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation type="obsolete">Emër i objekti të përzgjedhur</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation type="obsolete">Padukshëm</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation type="obsolete">Programth vetjak llogaritjeje:</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Gërma të Mundshme:</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation type="obsolete">Jepni këtu një listë fushash ndarë me presje</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Blloko/Çblloko</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Të ri &amp;prej Stampe...</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">Fshij ngjyrën e përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation type="obsolete">Zhvendos përpara</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation type="obsolete">Programthe Vetjakë:</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importo</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Brenda</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Përmbys</translation>
    </message>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Shfaq Emra Stampash</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="obsolete">Përdor një vijë të dytë bazuar fillimisht në formën e kornizës për rrjedhë teksti</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Numër kulmesh për shumëkëndëshat</translation>
    </message>
    <message>
        <source>No View</source>
        <translation type="obsolete">Pa Parje</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation type="obsolete">Etiketa</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">Ni&amp;vel</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="obsolete">SHtresa</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation type="obsolete">&amp;Përpunoni Stile...</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letër</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="obsolete">&amp;Përpuno Formë...</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Mesatare</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation type="obsolete">Pasqyro</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation type="obsolete">Kufizues vlere:</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation type="obsolete">Person ose organizëm përgjegjës për pasje pjesëmarrjeje në përmbajtjen e dokumentit</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="obsolete">Periudhë kohore ndërmjet ruajtjesh të vetvetishme</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Zgjidhni një fjalëkalim i cili t&apos;u lejojë përdoruesve të lexojnë PDF-të tuaja.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normale</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation type="obsolete">Numër më i madh Ndarjesh me Vizë që njekin njëra tjetrën.
Një vlerë 0 do të thotë ndarje me vizë të pakufizuara.</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Ringarko Tekst prej Kornize</translation>
    </message>
    <message>
        <source>Object</source>
        <translation type="obsolete">Objekt</translation>
    </message>
    <message>
        <source>Number</source>
        <translation type="obsolete">Numër</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation type="obsolete">Emër Gërmash</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="obsolete">Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Skemë Rrjete</translation>
    </message>
    <message>
        <source>Option</source>
        <translation type="obsolete">Mundësi</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation type="obsolete">Shtyp &amp;Tërë</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="obsolete">Nëse vepruese, Scribus ruan një kopje kopjeruajtjeje të kartelës suaj me zgjatimin .bak
çdo herë që mbaron periudha kohore</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Mënjanë Majtas</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation type="obsolete">Ti&amp;p:</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Shfaq Shtresa</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation type="obsolete">Pikë gërmash</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Numri i Shtyllave:</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Polake</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation type="obsolete">&amp;Ringarko Tekst prej Kornize</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Po ngarkoj:</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Kartela Postscript (*.ps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="obsolete">Hiq</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="obsolete">Riemërto</translation>
    </message>
    <message>
        <source>Result</source>
        <translation type="obsolete">Përfundim</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation type="obsolete">Gjerësi Vije:</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation type="obsolete">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation type="obsolete">Shembull</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Çkyç tërë</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation type="obsolete">Shkallë:</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation type="obsolete">Hi&amp;jezo</translation>
    </message>
    <message>
        <source>Search</source>
        <translation type="obsolete">Kërko</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">Madhë&amp;si:</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="obsolete">Përzgjidhni</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Shtyp Me Shkallë të &amp;Grisë</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Kyç tërë</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Hije:</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation type="obsolete">Formë</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="obsolete">Shift+</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation type="obsolete">Broshura</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation type="obsolete">Programth i Ri</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Ruaj si &amp;Pamje...</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Slovake</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation type="obsolete">Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation type="obsolete">E &amp;parenditur</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">St&amp;il</translation>
    </message>
    <message>
        <source>Square</source>
        <translation type="obsolete">Katror</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Tërë Formatet e Mbuluar</translation>
    </message>
    <message>
        <source>Status</source>
        <translation type="obsolete">Gjendje</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Nënset</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation type="obsolete">Stil:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">&amp;Dokument</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">Ngjyrë për Rrjetën e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation type="obsolete">Titull:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Poz:</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Krijo PostScript Level 3</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">Mos &amp;Ndrysho</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation type="obsolete">Fusha është formatuar si:</translation>
    </message>
    <message>
        <source>Enables automatic checking of your text while typing.</source>
        <translation type="obsolete">Aktivizon kontroll të vetvetishëm të tekstit ndërsa shtypni.</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation type="obsolete">Udhëzuesa Horizontalë</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">X-Poz:</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Çkyç</translation>
    </message>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;h3&gt;Rreth Qt-së&lt;/h3&gt;&lt;p&gt;Ky program përdor versionin %1 të Qt-së.&lt;/p&gt;&lt;p&gt;Qt është një toolkit C++ për zhvillim GUI &amp;amp; zbatime shumëplatformëshe.&lt;/p&gt;&lt;p&gt;Qt-ja ofron kalueshmëri prej një burimi të vetëm në MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, dhe tërë variantet e njohur komercialë Unix.&lt;br&gt;Qt është gjithaq i mundshëm për pajisje të trupëzuara.&lt;/p&gt;&lt;p&gt;Qt është një produkt Trolltech. Për më tepër të dhëna shihni &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation type="obsolete">Shfaq tejdukshmëri dhe objekte të tejdukshëm në dokumentin tuaj. Lyp Ghostscript 7.07 ose të mëvonshëm</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">Java Script</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Ekran / Web</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="obsolete">Qeth sa &amp;Mënjanat e Faqes</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation type="obsolete">Shih &amp;në:</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation type="obsolete">GtkRregullimi për vendndodhjen vertikale</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Emri &quot;%1&quot; s&apos;është unik.
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Welsh:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation type="obsolete">Fjala më e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Gjerësi:</translation>
    </message>
    <message>
        <source>Edit Styles</source>
        <translation type="obsolete">Përpunoni Stile</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="obsolete">Tipografi</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Shumë&amp;këndësh</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation type="obsolete">Titull CSV</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation type="obsolete">Ndihmëz</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">P&amp;rej:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Hapje për Rrjetën e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">Ngjyrë për Rrjetën e &amp;Madhe:</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="obsolete">Yellow</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation type="obsolete">Orientim</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Anët Djathtas</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation type="obsolete">Oshiloskop Turbullt %s</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation type="obsolete">Vetëm ikona</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Gërma për korniza të reja tekstesh</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation type="obsolete">është ndryshuar që prej ruatjes së fundit.</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation type="obsolete">&amp;Model Ngjyre</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation type="obsolete">Paraparje Përmbajtjeje Kartele</translation>
    </message>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Rregullime Administrimi Ngjyrash</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation type="obsolete">Gërma për tekstin apo objektin e përzgjedhur</translation>
    </message>
    <message>
        <source>Printer Options</source>
        <translation type="obsolete">Mundësi Shtypësi</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Faqerojtësa</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation type="obsolete">Paragraf i Çastit:</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation type="obsolete">Faqerojtësa</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation type="obsolete">Mos Eksporto Vlerë</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">Shkronja Fillimisht</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="obsolete">Përfundime Kërkimi për:</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Ky panel paraqet mundësi të ndryshme CUPS kur shtypet. 
Parametrat e saktë të mundshëm do të varen nga &quot;driver&quot;-i i shtypësit tuaj.
Mund të keni suport CUPS duke përzgjidhur Ndihmë &gt; Rreth.
Shih për përmendje të: C-C-T Kjo është e barazvlefshme me C=CUPS C=littlecms T=TIFF suport.
Mungesa e suportit për librarinë tregohet me një *</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation type="obsolete">Vetëm Fqe Tek</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation type="obsolete">Kur një Ikonë është tepër e vogël</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Ngjyrë &amp;Vije:</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Pamje (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Ndih&amp;mëza</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="obsolete">Emër i të &amp;Ekzekutueshmit:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation type="obsolete">Rreshti i parë është titull</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Oook! Po provoni të fshini një objekt që nuk ekziston!</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Zmadhimi më i vogël i lejuar</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Zmadhimi më i madh i lejuar</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Shumëvijë</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation type="obsolete">Ripërmaso pamjen jorizontalisht</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">Ngjyrë &amp;Teksti:</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="obsolete">Sete Ngjyrash</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation type="obsolete">Set ikonash për t&apos;u shfaqur</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation type="obsolete">&amp;Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation type="obsolete">Parashtro të Dhëna si HTML</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Kornizë Teksti</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">Ngjyrë &amp;Mbushëse:</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation type="obsolete">Fushë Tekstesh</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation type="obsolete">Kartela Teksti</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Mundëson një gërmë të stërmadhe për gërmën e parë të një paragrafi. E përdorur për efekte stilistikë</translation>
    </message>
    <message>
        <source>Edit Color</source>
        <translation type="obsolete">Përpunoni Ngjyrë</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation type="obsolete">&amp;Monitor:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation type="obsolete">M&amp;onitor:</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation type="obsolete">Shtylla &amp;Majtas</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation type="obsolete">T&amp;ip Vije:</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation type="obsolete">Shto Nyje</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation type="obsolete">Dokumente Tekst</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Përpunoni Përmbajtje Kornize</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="obsolete">&amp;Kyç Udhëzuesa</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation type="obsolete">Set ikonash për t&apos;u shfaqur</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Gërma...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Gërma...</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Anët Majtas</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation type="obsolete">Përpunoni Stil</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Kopjo fluturimthi</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Ndihmëza</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation type="obsolete">Dosje e Re %1</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">Kornizë &amp;Teksti</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Hap...</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Hap...</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation type="obsolete">Shmangje horizontale e pamjes brenda kornizës</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">&amp;Përpuno Formë</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Madhësi Gërmash:</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation type="obsolete">Fusha të Përzgjedhura</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="obsolete">Patejdukshmëri:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Mundësi</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation type="obsolete">Faqe</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">R&amp;reshti i Parë:</translation>
    </message>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Administro Shkurtprerje Tastiere</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">në Fund</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation type="obsolete">Po shkarkoj Stampa</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Gjerësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Shfaq Rrjetë</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Vizatoni Forma të ndryshme</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation type="obsolete">Vendosje Ikone...</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation type="obsolete">portret</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="obsolete">Portret</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation type="obsolete">Kopjo Kuti Teksti</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">PPI Fillestar: </translation>
    </message>
    <message>
        <source>custom</source>
        <translation type="obsolete">vetiak</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation type="obsolete">Largësi teksti nga kreu i kornizës</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Veti Vizatimi Vijash</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation type="obsolete">Doni vërtet të mbishkruhet Kartela:
%1 ?</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Ruaj Korniza të &amp;Lidhura Tekstesh si Artikuj PDF</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation type="obsolete">Pjesëmarrje prej:</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Rreth &amp;Qt-s</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation type="obsolete">Ngjitu</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Jashtë</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Slovene</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="obsolete">Fshi&amp;j</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Veti</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Eksporto interval faqesh si PDF</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation type="obsolete">Vijë Sipër</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation type="obsolete">prej 0</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Një përshkrim i përmbajtjes së dokumentit.
Roli i kësaj fushe është një përshkrim të shkurtër ose abstrakt i dokumentit. Gjatë eksportimit trupëzohet në PDF-në</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Jepni një listë objektesh të ndarë me presje ku
objekti mund të jetë * për tërë faqet, 1-5 për
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">Ndër&amp;mjet</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Nga Sipër-majtas Poshtë-djathtas</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="obsolete">Mjet Përpunimi Pamjesh</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="obsolete">Cakto shkallë zmadhimi parazgjedhje</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Zhbëj Fshirje Objekti</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Veti</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation type="obsolete">Një referencë jo e dykuptimtë te dokumenti brenda një konteksti të caktuar si p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation type="obsolete">Lëviz Pika Kontrolli në Mënyrë të Pavarur</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Pamjet e ripërmasuara vetvetiu ruajnë përpjestimet e veta fillestare</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Trupëzo profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation type="obsolete">Eksportoni vetëm faqen e çastit</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Këtu keni tërë Stampat tuaja, për të krijuar Faqe të re
tërhiqni një Stampë te parja e Faqes më poshtë.</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Trupëzo tërë Gërmat</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation type="obsolete">Merr...</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Pamje</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Degëzim Vertical</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Personi apo organizmi përgjegjës fillimisht për hartimin e përmbajtjes së dokumentit.
Kjo fushë mund të trupëzohet në dokumente Scribus për referencë, si dhe në metadatat e një PDF-je</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation type="obsolete">&amp;Përfshi Gërma</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Ekzekuto Programth...</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation type="obsolete">Të &amp;dhëna të Mëtejshme</translation>
    </message>
    <message>
        <source>Manage Tabulators</source>
        <translation type="obsolete">Administro shtojca</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation type="obsolete">Eksportoni një interval faqesh</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="obsolete">Ngjyrë për letrën</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation type="obsolete">Jep Fjalëkalimin</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Pikë e Thjeshtë</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Vendosje</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Ngri Shtresë</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Dyfisho</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Gradë rrotullimi për shumëkëndëshat</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation type="obsolete">&amp;Shfaq Gërma...</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation type="obsolete">Tip bashkimi vije</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Ndaj &amp;Shumëkëndësha</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation type="obsolete">Person ose organizëm përgjegjës për pasjen e dokumentit të mundshëm</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation type="obsolete">Lexim-shkrim</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation type="obsolete">Përdor ikona</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation type="obsolete">Zhvendos Nyje</translation>
    </message>
    <message>
        <source>Move Pages</source>
        <translation type="obsolete">Zhvendos Faqe</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation type="obsolete">Largësi ndërmjet shtyllash të krijuara vetvetiu</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Polake:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="obsolete">Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Shumëkëndësh</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation type="obsolete">Vetëm Lexim</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Fshih Mënjana</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation type="obsolete">Importues:</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation type="obsolete">Postera</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Ruaj  si</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation type="obsolete">Kontroll &amp;Jetim/E Ve</translation>
    </message>
    <message>
        <source>Manage Guides</source>
        <translation type="obsolete">Administroni Udhëzuesa</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation type="obsolete">Vetëm lexim</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation type="obsolete">Të dhëna rreth të drejtash të ruajtura rreth dhe mbi dokumentin, p.sh të drejta kopjimi, patenta ose shenja tregtare</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation type="obsolete">Shfaq Tej&amp;dukshmëri</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation type="obsolete">Gërma teksti të përzgjedhur</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">&amp;Paraparje Gërmash...</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="obsolete">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="obsolete">&amp;Paraparje</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Paraparje</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="obsolete">Set Ngjyrash i Çastit:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Shtyp:</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Shtypës</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation type="obsolete">Trashësi vije</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">Si&amp;për:</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation type="obsolete">Gjatësi e fjalës më të vogël për ta ndarë me vijë.</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">Shto tek stampë</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">Shn&amp;dërro si </translation>
    </message>
    <message>
        <source>product</source>
        <translation type="obsolete">produkt</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Scrapbooks (*.scs);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation type="obsolete">Veti Shënimesh</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation type="obsolete">Kur një Ikonë është tepër e madhe</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation type="obsolete">Të ri Prej Stampe</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Merr Pamje...</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">&amp;Merr Pamje...</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation type="obsolete">Shkallëzim Vertikal</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation type="obsolete">Pë&amp;rshkrim:</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopjo Këtu</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation type="obsolete">Kopjo Faqe</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation type="obsolete">T&amp;ip Pamjeje: </translation>
    </message>
    <message>
        <source>Polygon Properties</source>
        <translation type="obsolete">Veti Shumëkëndëshi</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation type="obsolete">&amp;Interval Shtypjeje</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation type="obsolete">Zëvendësim</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation type="obsolete">Rregulloni tejdukshmërinë për ngjyrën e përzgjedhur</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation type="obsolete">Qeth Shtegun Vertikalisht Sipër</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation type="obsolete">Faqe e Rregulluar</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation type="obsolete">Kompensim Pikash të Zeza është një metodë për përmirësimin e kontrastit në foto.
Këshillohet ta aktivizoni këtë nëse keni foto në dokumentin tuaj.</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Po importoj Faqe...</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;kstra</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation type="obsolete">Doni të ruani ndryshimet tuaj?</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Koeficient shkalle gërmash</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation type="obsolete">Ripërmaso &amp;Sa Madhësi Kornize</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Gjerësi vijash</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation type="obsolete">S&amp;til gërmash</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Sht&amp;yp Me Ngjyra Nëse Mundet</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Programi</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation type="obsolete">Mbyll këtë Kurbë Bezier</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Stil vije formash</translation>
    </message>
    <message>
        <source>Change the angles at which lines of the polygon join</source>
        <translation type="obsolete">Ndryshoni këndet nën të cilë vijat e shumëkëndëshit takohen</translation>
    </message>
    <message>
        <source>Copy or Move a File</source>
        <translation type="obsolete">Kopjo ose Zhvendos Kartelë</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukrainase:</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">&amp;Bashkangjit Tekst tek Shteg</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Scrapbooks (*.scs);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="obsolete">Madhësi parazgjedhje gërmash për menu dhe dritare</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Disa objekte janë të kyçur.</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Fut Numër Faqeje</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Jeni të sigurtë se doni të fshihen %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Dërgoje në të Papastër</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Numër i Faqes së Parë</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation type="obsolete">Po heq stampë</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation type="obsolete">Po përgatis stampë</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">para Faqes</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Ngjyrë objekti të përzgjedhur</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation type="obsolete">Përpuno tituj për CD-Tekst</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation type="obsolete">Vleftëso</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="obsolete">Gërma (pa hapësira):</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation type="obsolete">po përkthej stampë XML</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation type="obsolete">Stil për paragrafin e çastit</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Bjere Përpara</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="obsolete">Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Po ujdis Ngjyra</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation type="obsolete">Formularë PDF:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ndërro...</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="obsolete">&amp;Ndrysho...</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation type="obsolete">Vendos Tekstin Djathtas</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation type="obsolete">Mjete PDF</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation type="obsolete">Format Përqindjesh</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">Mjete P&amp;DF</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Merr Tekst...</translation>
    </message>
    <message>
        <source>CSV Importer Options</source>
        <translation type="obsolete">Mundësi Importuesi CSV</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Koeficient shkalle gërmash</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="obsolete">Gabim në shkrim kartele(ash) përfundim.</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation type="obsolete">&amp;Shtigje Shtesë</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation type="obsolete">Gërma</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Administro Udhëzuesa...</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrainase</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation type="obsolete">Majë Katrore</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;E papastër</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">E papastër</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Një rrugë braktisjeje e disa shkallëve të grisë të përbëra nga
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.
UCR prek më tepër pjesë pamjeje  që janë tone neutrale dhe/ose të errët
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje
dhe lipsen ca eksperimente dhe prova për çdo herë.
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Gërma</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baske:</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation type="obsolete">Përpunoni &amp;Rrethina Vije</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Mënjana:</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="obsolete">Zvogëlo ose zmadho shfaqjen e dokumentit </translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation type="obsolete">&amp;Numër Kopjesh:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Po lexoj nga e Papastër</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">&amp;Kryeradhë dhe Hapësira</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="obsolete">Ruaj setin e çastit për ngjyrat</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation type="obsolete">Gërma për përdorim me PDF 1.3:</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Veti Kornize Pamjeje</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation type="obsolete">Katalogë</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">&amp;Shfaq Kohëzgjatje:</translation>
    </message>
    <message>
        <source>Password</source>
        <translation type="obsolete">Fjalëkalim</translation>
    </message>
    <message>
        <source>Category</source>
        <translation type="obsolete">Kategori</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">&amp;Zbato Efekt te tërë Faqet</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Tekst Shtegu</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation type="obsolete">&amp;Ngarko prej Kartele...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Javascripts...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Trupëzo në:</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="obsolete">Profile &amp;ICC:</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation type="obsolete">Përshtat...</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Fshih &amp;Konsol</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation type="obsolete">Dukshmëri:</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation type="obsolete">Të dhëna CSV</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="obsolete">&amp;Zëvendëso</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Ruaj Tani</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Lejo &amp;Shtypjen e Dokumentit</translation>
    </message>
    <message>
        <source>&amp;Restore</source>
        <translation type="obsolete">&amp;Rikthe</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation type="obsolete">&amp;Autor:</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation type="obsolete">Autor:</translation>
    </message>
    <message>
        <source>average</source>
        <translation type="obsolete">mesatare</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation type="obsolete">Vijë Djathtas</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation type="obsolete">Kontroll i përfunduar</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">&amp;Sa Dritarja</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Sa Dritarja</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Pa&amp;stro Përmbajtje</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation type="obsolete">Eksportoni tërë faqet</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Shpjere &amp;Prapa</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation type="obsolete">Maksimizo</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Prodho bazë të dhënash</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Emër i dhënë dokumentit.
Kjo fushë mund të trupëzohet në dokumente Scribus për referencë, dhe edhe në metadatën e një PDF-je</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Ruaj si &amp;Stampë</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">Shpe&amp;Sti:</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation type="obsolete">Gjej Drejtori</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation type="obsolete">Veti</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Listë Kuti</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Shtyp...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Shtyp...</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="obsolete">Po shtyp</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation type="obsolete">Sht&amp;ypës:</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation type="obsolete">Vijë Majtas</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation type="obsolete">Shfaq kartela të &amp;fshehura</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation type="obsolete">&amp;Përfundo Përpunim</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Parazgjedhje Shkalle Zmadhimi</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="obsolete">Anti-alias &amp;Tekst</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="obsolete">Drejtori programthesh Scripter parazgjedhje</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation type="obsolete">&amp;Përkthime</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation type="obsolete">Importo të Dhëna</translation>
    </message>
    <message>
        <source>Insert Page</source>
        <translation type="obsolete">Fut Faqe</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Import i kryer</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation type="obsolete">Tip fundi vije</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation type="obsolete">Normalizo</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation type="obsolete">Shkallëzim Ndër Diagonal</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation type="obsolete">Ndrysho drejtori përfundimesh</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">Dyfisho CD</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Ngjyrë e Re:</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation type="obsolete">Shtylla Statike Ngjyrash</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation type="obsolete">Zë i Ri</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Përshkrim</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation type="obsolete">Shfaq Kurbë</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Shtresë e Re</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Kërko/Zëvendëso...</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="obsolete">Stil i Ri</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation type="obsolete">X-Shk&amp;allë:</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation type="obsolete">Programe</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Fshij përzgjedhjen(t) e çastit.</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Dërgo te Sh&amp;tresë</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">&amp;Gërma Parazgjedhje:</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Të dhëna</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Poshtë:</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="obsolete">Rreth Qt-s</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation type="obsolete">Përpunoni Veti Ngjyre Mbushëse</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Fshih &amp;Veti...</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation type="obsolete">Kërkesë Marrëveshjeje Kyçash</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation type="obsolete">Ul Volumin</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">&amp;Madhësi Parazgjedhje:</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Përdor Gërma </translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Merr Tekst.Pamje...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Merr Tekst/Pamje...</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">Ri&amp;përmasim</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Ripërmasim:</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pikas (p)</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Mos shfaq objekte jashtë mënjanave te faqe të shtypura ose kartela të eksportuara</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Lidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Ujvarë</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Katalane</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Ruaj Si</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Ruaj  si</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Zbato prej &amp;Faqeje:</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation type="obsolete">Y-Shkall&amp;ë:</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Zbato Stampë...</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Poshtëshkrim</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation type="obsolete">Faqe për Fletë</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">Përdor Rregullime të Përgjithshme &quot;Proxy&quot;</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation type="obsolete">Pro&amp;gramth</translation>
    </message>
    <message>
        <source>Spanish and Catalan:</source>
        <translation type="obsolete">Spanjolle e Katalane:</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation type="obsolete">Programth:</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation type="obsolete">Kyç ose çkyç objektin</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation type="obsolete">I pakapshëm</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Hap një Dokument</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Lejo shtim shënimesh dhe fushash te PDF-ja. 
Nëse nuk shenjohet, parandalohet përpunimi i shënimeve dhe fushave.</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Doni të fshini tërë Objektet edhe në këtë Shtresë?</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Sipërshkrim</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation type="obsolete">Përqindje</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Fut Kurba Bezier</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation type="obsolete">&amp;Dil pa Ruajtur</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Fut...</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Ky është një rregullim i cili si parazgjedhje nuk është veprues. Do të duhej të aktivizohej
kur kërkohet veçanërisht nga shtypësi juaj dhe ju kanë dhënë saktë hollësitë e nevojshme.
Përndryshe, PDF-ja juaj që eksportuat mund të mos shtypet si pritet dhe të jetë vërtet i kalueshëm nëpër sisteme.</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Po fus</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Ruse</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Po fus</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Fut...</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">Shtypja dështoi!</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Ruaj Dokumentin e çastit si PDF</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Stampa të Mundshme:</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Ndërro Ngjyrën e Tekstit</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Gërma:</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Numër shtyllash në një kornizë teksti</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation type="obsolete">Po shtyp Dokument...</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation type="obsolete">Karta Biznesi</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Vendos rrjetën para objekteve të faqes</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation type="obsolete">Dokumente PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Eksporto tërë faqet si PDF</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">Shfaq P&amp;araparje Faqesh</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation type="obsolete">Kërko për:</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation type="obsolete">Dhjetore:</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation type="obsolete">&amp;Pozicion:</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Faqja Majtas e Para</translation>
    </message>
    <message>
        <source>Position:</source>
        <translation type="obsolete">Pozicion:</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation type="obsolete">e fushave vijuese:</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation type="obsolete">Cilësia e pamjeve tuaja - 100% është më e mira, 1% cilësia më e ulët</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">&amp;Vijëzo Tekst</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Vijëzo Tekst</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation type="obsolete">Pasqyro Shtegun Vertikalisht</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Rrotulloni Element</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">PPI e Tanishme: </translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation type="obsolete">Dokument Scribus</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="obsolete">Për&amp;fundimi në Kartelë:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovake:</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation type="obsolete">Shkallëzim rrezor i lirë</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Përzgjidhni &amp;Tërë</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation type="obsolete">Përzgjidhni Gërma</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Lejo kopjim teksti apo grafike prej PDF-së. 
Nëse nuk shenjohet, nuk mund të kopjohet tekst apo grafikë.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation type="obsolete">Metodë shfaqjeje në ekran ngjyrash të cilat mund të mos shtypen si duhet.
Kjo lyp profile shumë të saktë dhe shërben vetëm si sinjalizim.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Zbato efektet e përzgjedhur te të tëra faqet.</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="obsolete">Ngarko Tekst prej Kartele</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation type="obsolete">Destinacion Shtypësi</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation type="obsolete">Importo të Dhëna prej:</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Korniza pamjesh lejojnë pamjet të ripërmasohen në çfarëdo madhësi</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation type="obsolete">Kodim:</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">/Mënyrë Vizualizimi/Pa</translation>
    </message>
    <message>
        <source>List View</source>
        <translation type="obsolete">Parje si Listë</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopje e</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation type="obsolete">Përzgjidhni një Kyç për këtë Veprim</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Spanjolle</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation type="obsolete">dhe më e vogël ose baraz me:</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Gjerësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Special</source>
        <translation type="obsolete">Special</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation type="obsolete">&amp;Burim:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation type="obsolete">St&amp;il:</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus-i vithiset për shkak të Sinjalit #%1</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Rregullime Miu</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>&amp;Fully Automatic</source>
        <translation type="obsolete">&amp;Plotësisht Automatike</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation type="obsolete">&amp;Shto...</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="obsolete">Fshi&amp;j</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Shfaq &amp;Mënjana</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Shtyp Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Merr Tekst...</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italiane:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Kënd:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Daneze:</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Vini</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation type="obsolete">Vetëm shkrim</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">P&amp;oshtë:</translation>
    </message>
    <message>
        <source>Scribus </source>
        <translation type="obsolete">Scribus </translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="obsolete">&amp;Programthe:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation type="obsolete">simlidhja</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Qendër</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">Administrimi &amp;Ngjyrash...</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation type="obsolete">&amp;Ngjyra</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation type="obsolete">Largësira</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Fshij</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Fjalëkalime</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation type="obsolete">Flamurka është shpërfillur për PDF 1.3</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Po shtyp...</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation type="obsolete">Tregon nivelin në të cilin gjendet objekti, 0 do të thotë që objekti është në fund fare</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Pamje:</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Javascripts (*.js);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Tekst sipas një Shtegu</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation type="obsolete">Dokument:</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation type="obsolete">Shumëvijëshe</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Drejtori profilesh ICC parazgjedhje</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Nënvijë</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Zhbëj Ndryshim Objekti</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="obsolete">&amp;Interval:</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="obsolete">&amp;Nënvijë</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="obsolete">&amp;Eksporto</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Veti Kornize Teksti</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="obsolete">Drejtori dokumentesh parazgjedhje</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation type="obsolete">&amp;Shpërfill të Mëdhaja/Vogla</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Suedeze</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="obsolete">ka një të tillë. Ta mbishkruaj? </translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation type="obsolete">Korniza Teksti &amp;Vetvetiu</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation type="obsolete">E &amp;gjelbër:</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Paraparje Gërmash</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Paraparje Gërmash</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="obsolete">Orientim parazgjedhje faqesh dokumenti</translation>
    </message>
    <message>
        <source>Other...</source>
        <translation type="obsolete">Tjetër...</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Tjetër...</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Ngjyrë e vijave udhëzuese që fusni</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Ruaj Dokumentin e çastit</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation type="obsolete">Diamant</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation type="obsolete">Drejtorira</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Po ngarkoj...</translation>
    </message>
    <message>
        <source>Do you really want do delete this Template?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Stampë?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;/Importo</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Pikë (pt)</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Fut</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Përmbys</translation>
    </message>
    <message>
        <source>Display</source>
        <translation type="obsolete">Shfaq</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation type="obsolete">E përpunueshme</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation type="obsolete">Simlidhje te Special</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation type="obsolete">Shtyp Fa&amp;qen e Çastit</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="obsolete">Qeth Shtegun Vertikalisht Majtas</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Zhbëj Zhvendosje Objekti</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation type="obsolete">Programth i &amp;Ri</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation type="obsolete">Lidhje e Jashtme</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Kartela Tekst (*.txt);;Tërë Kartelat(*)</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Hapje për Rrjetën e &amp;Madhe:</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Shtresa</translation>
    </message>
    <message>
        <source>Show Page Previews</source>
        <translation type="obsolete">Shfaq Paraparje Faqeje</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="obsolete">Vijë e Ndërprerë</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="obsolete">Vijë me Pika</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="obsolete">&amp;Rreshta:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Tekst i Lidhur</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Lituaneze:</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="obsolete">E &amp;mesme</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation type="obsolete">&amp;Dil Pa Përditësuar Kornizë Teksti</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Dil Pa Përditësuar Kornizë Teksti</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation type="obsolete">Përzgjidhni Drejtori</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Zgjidhni një Drejtori</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="obsolete">&amp;Pa Tast Prishkurt</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Automatike</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation type="obsolete">I &amp;lidhur</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation type="obsolete">Tërë Kartelat (*.*)</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation type="obsolete">Gjithsej:</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation type="obsolete">Bëje pamjen të hyjë brenda madhësisë së kornizës</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation type="obsolete">S&amp;kaje:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">Pr&amp;onar:</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Krijo një Dokument të ri</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopje e %1</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Portë Windows:</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation type="obsolete">P&amp;ërpjestimor</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation type="obsolete">Lexo: %1</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Zbato Stampë...</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Cilësi:</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">Ruajtje përfundimesh e suksesshme</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation type="obsolete">Natyra ose zhanri i përmbajtjes së dokumentit, psh. kategori, funksione, zhanre, etj</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="obsolete">&amp;Hiq</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="obsolete">&amp;Riemërto</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation type="obsolete">Dukje</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Djathtas:</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Ndihmë &amp;Online...</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="obsolete">&amp;Ujdis Madhësi Paraqitjeje</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="obsolete">&amp;Kërko</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation type="obsolete">Kutizë</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">&amp;Hije:</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">&amp;Ruaj Set Ngjyrash</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation type="obsolete">Përzgjidhni Tërë</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Përzgjidhni tërë</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation type="obsolete">Vlera nuk është llogaritur</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation type="obsolete">Kyç ose çkyç madhësinë e objektit</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">S&amp;iguri</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="obsolete">&amp;Temë:</translation>
    </message>
    <message>
        <source>Possible Hyphenation</source>
        <translation type="obsolete">Ndarje me Vizë e Mundshme</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation type="obsolete">Drejtori:</translation>
    </message>
    <message>
        <source>System Menu</source>
        <translation type="obsolete">Menu Sistemi</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation type="obsolete">Përpuno...</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Përpuno...</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation type="obsolete">&amp;Titull:</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="obsolete">&amp;X-Poz:</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="obsolete">Bëj tekst nga kornizat e poshtme të rrjedhin përreth formës së objektit</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Përdor Karakteristika &amp;Tejdukshmërie PDF 1.4 </translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation type="obsolete">Përdor përpjestimet e pamjes më mirë se ata të kornizës</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugeze</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="obsolete">&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Efekte</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Efekt:</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Efekte</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation type="obsolete">Të &amp;drejta</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Anë të Poshtme</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">&amp;Administro Pamje</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation type="obsolete">Hequr&amp;vije</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Kjo Radhë Tastesh është në përdorim</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Numri i Rreshtave:</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Administro Pamje</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation type="obsolete">Krijo vetvetiu korniza teksti kur shtohen faqe të reja</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation type="obsolete">Ngrije një nivel</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="obsolete">&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation type="obsolete">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Ngopje ngjyre:</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation type="obsolete">Stil vije e objektit të çastit</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">Drejt&amp;im:</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">Është Sh&amp;ënim PDF</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="obsolete">Sigurohuni që pamjet dhe gërmat që përdorni mund të përdoren lirisht. Nëse gërmat nuk janë të përbashkëta mos i koleksiononi gjatë ruajtjesh stampash.</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Zbato Profile &amp;ICC</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation type="obsolete">Zbrite një nivel</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Inç (inç)</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="obsolete">Orie&amp;ntim:</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Hapësira Vertikale</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Vini &amp;Tekst...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Çblloko </translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="obsolete">&amp;Dokumente:</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation type="obsolete">&amp;Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Ndryshimi në zmadhim për çdo veprim &quot;zoom&quot;</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation type="obsolete">Kyç i Çastit</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Një rrugë braktisjeje e disa shkallëve të grisë të përbëra nga
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.
UCR prek më tepër pjesë pamjeje  që janë tone neutrale dhe/ose të errët
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje
dhe lipsen ca eksperimente dhe prova për çdo herë.
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">Pa&amp;rapëlqime</translation>
    </message>
    <message>
        <source>Element</source>
        <translation type="obsolete">Element</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Hapësirë midis shtyllash në kornizë teksti</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation type="obsolete">Ngjyra &amp;Bazë</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Elips</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="obsolete">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Sfond:</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Përpunoni Pamje</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation type="obsolete">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">pas Faqes</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Shfaq &amp;Udhëzuesa</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="obsolete">Madhësi parazgjedhje e faqes, ose si madhësi standard ose si madhësi vetjake</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation type="obsolete">Majë e Rrumbullakët</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">&amp;Veprues</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Veprues</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Zgidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation type="obsolete">&amp;Aktivizo Administrim Ngjyrash</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Angleze:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="obsolete">E panjohur</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">Përp&amp;uthshmëri:</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">&amp;Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation type="obsolete">Rirregullo Formular</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="obsolete">Cakto &amp;Kyç</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation type="obsolete">Gjeometri</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="obsolete">Përditëso Kornizë Teksti</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">Paraqitja fizike ose numerike e dokumentit. Do të ishte me leverdi përmendja e tipit të medias dhe përmasat.
Gjithashtu të dobishme janë RFC2045, RFC2046 për tipe MIME</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Fshij ngjyrë:</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation type="obsolete">Ngjyra &amp;Vetjake</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation type="obsolete">Largësi teksti nga e majta e kornizës</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Gjerësi vije formash</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Fut Vijë me Dorë të Lirë</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Hapje:</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Zhvendos Këtu</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Zbato te Faqe e Ça&amp;stit</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Ripërm&amp;asim:</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Referencë Online</translation>
    </message>
    <message>
        <source>Required</source>
        <translation type="obsolete">E nevojshme</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Ngopje</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turke:</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation type="obsolete">Aktivizo ose çaktivizo shtypjen e objektit</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">Nuk u gjet gjë për ta importuar</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Shembull i gërmave tuaja</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikalisht</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">Zh&amp;vendosje</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation type="obsolete">Mos Kontrollo Drejtshkrimin</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation type="obsolete">Fshij Tërë</translation>
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
        <source>Global Font Settings</source>
        <translation type="obsolete">Rregullime të Përgjithshme Gërmash</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Ruse:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="obsolete">Vetëruajtje</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">J&amp;ashtë:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">&amp;Jashtë:</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">&amp;Rregullime</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Rregullime</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Ndërmjet</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Slovene:</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation type="obsolete">Zarfa</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="obsolete">Ruaj si Pamje</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Mos përdor profile ngjyrash që genden të trupëzuar në burimin e pamjes</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation type="obsolete">Përpunoni formën e kornizës...</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation type="obsolete">Mb&amp;ulim</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Tip i efektit të ekranit.</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation type="obsolete">Jo Përpjestimore</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation type="obsolete">Ruaj përpjestim</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation type="obsolete">Përpjestimore</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation type="obsolete">Krye Letrash</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="obsolete">Të dhëna Dokumenti:</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation type="obsolete">Parapëlqime</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Vithisje Scribus-i</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation type="obsolete">Njësi &amp;Parazgjedhje</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation type="obsolete">Gj&amp;uhë:</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="obsolete">&amp;Hiq numra rreshtash</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation type="obsolete">Zëvendëso &amp;Tërë</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Stampa...</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Po ruaj...</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Finlandeze</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation type="obsolete">Sht&amp;ypje:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation type="obsolete">I dukshëm</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="obsolete">Hidhu te:</translation>
    </message>
    <message>
        <source>Select Fields</source>
        <translation type="obsolete">Përzgjidhni Fusha</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation type="obsolete">Një referencë te një dokument i lidhur, mundësisht duke përdorur një identifikues formal si p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation type="obsolete">Hap një Shumëkëndësh ose Pret një Kurbë Bezier</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">&amp;Përfshi Faqerojtësa</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Kohëzgjatje Efek&amp;ti:</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">Zh&amp;vendoje</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation type="obsolete">Kanal alfa:</translation>
    </message>
    <message>
        <source>Save as Template</source>
        <translation type="obsolete">Ruaj si Stampë</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">Gjuha në të cilën përmbajtja e dokumentit është shkruajtur, zakonisht kod ISO-639 gjuhe
mundet edhe ndarë me vizë nga prapashtesë kodi ISO-3166 vendi, p.sh. en-GB, fr-CH</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Importoni Stampë</translation>
    </message>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Zbato Stampë</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation type="obsolete">Vlera duhet të jetë më e madhe ose e baraz:</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation type="obsolete">Datë e shoqëruar një ngjarjeje në jetën e dokumentit, në format VVVV-MM-DD, si në ISO 8601</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation type="obsolete">Ur&amp;dhër:</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation type="obsolete">Udhëzuesa Vertikalë</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">N&amp;dryshoni...</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Zbato te tërë Faqet &amp;çifte</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">&amp;Përshtatja Kornizën Pamjes</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation type="obsolete">Mi&amp;nimizo</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation type="obsolete">Numër shtyllash për t&apos;u krijuar në korniza teksti të krijuara vetvetiu</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation type="obsolete">Paraqitje PDF</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">Tema e përmbajtjes së dokumentit.
Kjo fushë është për fjalëkyça dokumenti që doni të trupëzohen në një PDF, për të ndihmuar kërkime dhe indexim kartelash PDF</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Shteg për te Kartelë Gërmash</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Faqe Majtas</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation type="obsolete">Format:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation type="obsolete">F&amp;ormat</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="obsolete">Veti...</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="obsolete">Ka një kartelë të tillë. Ta mbishkruaj? </translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Po lexoj Parapëlqimet</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation type="obsolete">Ruaj &amp;Dokument</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Përcakton përputhshmërinë me. Parazgjedhja është Acrobat 4.0 çka ju jep përputhshmërinë më të madhe.
Zgjidhni Acrobat 5.0 nese kartela juaj ka karakteristika PDF 1.4 si p.sh. tejdukshmëri ose nëse keni nevojë për kodim 128 bit.
PDF/X-3 vlen për eksportim PDF-sh për shtypje komerciale dhe është i përzgjedhshëm kur keni veprues administrim ngjyrash.</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="obsolete">Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Importo &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>Polygons will be convex rather than concave</source>
        <translation type="obsolete">Shumëkëndëshat do të jenë të mysët më tepër se të lugët</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">&amp;Hiq Të papërdorura</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Kujdes</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Hungareze:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Frënge:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bulgare:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="obsolete">Paraqit Zonë të &amp;Pashtypshme në Ngjyrë Mënjanash</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation type="obsolete">Revista</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Faqe Përkarshi</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Ri&amp;kthehu tek i Ruajtur</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation type="obsolete">nga %1</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Kohëzgjatja e xhirimit të efektit.
Një kohë më e shkurtër do ta përshpejtojë efektin, një më e gjatë do ta ngadalësojë.</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">&amp;Vini Tekst...</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation type="obsolete">Fut Faqerojtës</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation type="obsolete">Largësi teksti nga e djathta e kornizës</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Zgjidhni pamjet dhe zbukurim dritaresh parazgjedhje.
Scribus-i bën të vetën cilëndo temë të mundshme KDE apo Q</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="obsolete">Teksti &amp;Rrjedh Rreth Kornizës</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Norvegjeze:</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation type="obsolete">Hapësirë poshtë paragrafit</translation>
    </message>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="obsolete">Masko zonën jashtë mënjanash me ngjyrën e mënjanave</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Të përgjithshme</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">&amp;Të përgjithshme</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Gjermane:</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation type="obsolete">&amp;Eksporto te Drejtori:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation type="obsolete">A&amp;utorë</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation type="obsolete">Shtylla Dinamike Ngjyrash</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Aktivizo karakteristika tejdukshmërie brenda eksportimi PDF 1.4</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation type="obsolete">Një drejtori sipër</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">Po rregulloj Shkurtprerje</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="obsolete">Dokumente së &amp;Fundmi:</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation type="obsolete">&amp;Ruaj dhe Dil</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation type="obsolete">Paraparje Të dhënash Kartele</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Paragrafë:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation type="obsolete">Ma&amp;ksimizo</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation type="obsolete">Thekso</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Fut Shumëkëndësha</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation type="obsolete">Ul Volumin</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="obsolete">Fusni Fusha PDF</translation>
    </message>
    <message>
        <source>Icon Placement</source>
        <translation type="obsolete">Vendosje Ikone</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="obsolete">Gabim Fatal</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Vendos Majtas</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">&amp;Dritare</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="obsolete">Numër për faqen e parë të dokumentit</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="obsolete">Eksportim i suksesshëm</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation type="obsolete">Profil Futjesh:</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Shfaq Faqet &amp;Krah Njëra Tjetrës</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation type="obsolete">Nëse &quot;widget&quot;-i është &quot;widget&quot;-i parazgjedhje</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation type="obsolete">Skemë vije</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Përzgjidhni Gërma të Reja</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Mundësi të Mëtejshme</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Ruaj faqe si &amp;SVG...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Rreth Scribus-it</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Rreth Scribus-it</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">&amp;Kërko për Ndihmë</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation type="obsolete">Përzgjidhni një Kyç për këtë Veprim</translation>
    </message>
    <message>
        <source>Setup Printer</source>
        <translation type="obsolete">Rregulloni Shtypës:</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation type="obsolete">Kartela HTML</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation type="obsolete">Ngjyrë Mbushëse</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation type="obsolete">Pamje SVG (*.svg);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Tërë Faqet</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="obsolete">Tërë Faqet</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation type="obsolete">Vija Kutie</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation type="obsolete">X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation type="obsolete">GtkRregullimi për vendndodhjen horizontale</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="obsolete">Njësi matëse parazgjedhje për përpunime dokumentesh</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation type="obsolete">Mbush stil:</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation type="obsolete">Fusni Shënime PDF</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation type="obsolete">Kufizues fushe:</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Ngjyrë vijash</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Fjalë:</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Përcakton lidhjen e faqeve te PDF-ja. Lini parazgjedhjen - Majtas,
veç në e ditshi që keni nevojë ta ndryshoni.</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Përpuno Tekst...</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation type="obsolete">Shtrirja ose fokusi i përmbajtjes së dokumentit, mundësisht duke përfshirë vendndodhje, kohe dhe shtrirje juridike</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="obsolete">&amp;Tërë faqet</translation>
    </message>
    <message>
        <source>the file</source>
        <translation type="obsolete">kartela</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation type="obsolete">Lejoje pamjen të ketë tjetër madhësi në kornizë</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation type="obsolete">Shkallëzim Diagonal</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="obsolete">Lartësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">&amp;Drejtim:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Hije:</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Shfaq &amp;Veti...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation type="obsolete">&amp;Përditëso Kornizë Teksti</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation type="obsolete">Shembull:</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Udhëzuesa Shtylle</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation type="obsolete">Lëviz Pika Kontrolli Simetrikisht</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation type="obsolete">Mos Rrëshqit</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation type="obsolete">Caktoni rrezen e lakimit të kulmit</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation type="obsolete">Sim&amp;ulo Shtypës në Ekran</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">T&amp;ip Efekti:</translation>
    </message>
    <message>
        <source>If you uncheck this you will get a dialog
everytime a possible Hyphenation is found.</source>
        <translation type="obsolete">Nëse i vini shenjë kësaj do të kini një dialog
sa herë që gjendet një Ndarje me Vizë.</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation type="obsolete">Importo tekst pa e formatuar</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">Në Ballë</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation type="obsolete">Heqje Ngjyre &amp;Nën Të</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation type="obsolete">Përpuno tituj për CD-Tekst</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation type="obsolete">Ju duhet të paktën Ikona për Normal që të përdorni Ikona Për Butona</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="obsolete">&amp;Lartësi:</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation type="obsolete">Krijoni Faqe(t)</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">Kurbë &amp;Bezier</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">&amp;Ndërthur Shumëkëndësha</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">Të dhë&amp;na Dokumenti...</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Nga Poshtë Sipër</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Pasqyro Faqe(t) Horizontalisht</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">&amp;Ruaj si P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation type="obsolete">&amp;Sfond...</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Lejo Shtim Shëni&amp;mesh dhe Fushash</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">Shtypje &amp;Normale</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation type="obsolete">Tekst dhe Figura</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">Hap të &amp;Fundmin</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">&amp;Gjerësi Vije:</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Shpërnda/&amp;Drejto...</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation type="obsolete">Shtigje Shtesë</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation type="obsolete">Zgjidhni formën e kornizës...</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation type="obsolete">Largësi e Tekstit</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Ruaj &amp;Përpjestim</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Stampë e Re</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation type="obsolete">Krijo Dosje të Re</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Ruaj Faqe si $EPS...</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation type="obsolete">Efekte Gërma</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation type="obsolete">Simlidhje te Kartelë</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Shembull Shumëkëndëshi</translation>
    </message>
    <message>
        <source>Documentation:</source>
        <translation type="obsolete">Dokumentim:</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Çkyç &amp;Madhësi Objekti</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">&amp;Fut Shembull Teksti</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation type="obsolete">Ndryshim Përzgjedhjeje</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Shto Shtresë të re</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation type="obsolete">Identi&amp;fikues:</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Përpunoni ngjyrën e përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Shfaq &amp;Korniza</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation type="obsolete">Simlidhje te Drejtori</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">&amp;Veçime Shtypjeje</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation type="obsolete">Gërma të &amp;Mundshme</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation type="obsolete">Gërma të Mundshme</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation type="obsolete">Numër shtyllash në kornizë teksti</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetra (mm)</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation type="obsolete">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="obsolete">Antialias &amp;Grafikash</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Faqe Djathtas</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="obsolete">Vijë e Plotë</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation type="obsolete">Degëzim Vertical</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">&amp;Dil Sidoqoftë</translation>
    </message>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Përpuno Stampa</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Fut Kornizë Teksti</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">S&apos;shkruaj dot Kartelën: 
%1</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Shfaq &amp;Pamje</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Zbato te tërë Faqet &amp;teke</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">&amp;Kërko për Ndihmë</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">&amp;Mos Ndrysho</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation type="obsolete">Më tepër Hollësi</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation type="obsolete">Po instaloj Stampa</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Kryej matje</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="obsolete">Tekst i etiketës së kornizës</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation type="obsolete">Një referencë te një dokument prej të cilit dokumenti i pranishëm ka rrjedhur, p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Faqe Dokumenti</translation>
    </message>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Grafikë</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation type="obsolete">Parje në Hollësi</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation type="obsolete">Kontroll &amp;Jetim/E Ve</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Lituaneze</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Doracak për Scribus</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation type="obsolete">Zëvendëso me:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="obsolete">Zgjidhni Emër</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Stampë (Faqe Djathtas):</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="obsolete">Po, tërë</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Rregullime Pamjeje</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation type="obsolete">Faqe e çastit</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Sistemo Faqe</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation type="obsolete">Krijuar me</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="obsolete">Antialias &amp;Tekstesh</translation>
    </message>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Rreth Scribus-it %1%2</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Ngjyrë gërmash</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation type="obsolete">Më pak Hollësi</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">&amp;Drejtim</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Nga Sipër Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Fut Special</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Importo &amp;SVG...</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation type="obsolete">Përpuno JavaScript</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation type="obsolete">Përpunoni Veti Ngjyre Vije</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Bëni një kopje të ngjyrës së përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">Bëj set ngjyrash të çastit set parazgjedhje të çastit</translation>
    </message>
    <message>
        <source>getColor(&quot;name&quot;) -&gt; tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>getColor(&quot;emër&quot;) -&gt; &quot;tuple&quot;

Përgjigjet me një &quot;tuple&quot; (C, M, Y, K) që përmban përbërësit në katër ngjyra të
ngjyrës &quot;emër&quot; prej dokumentit të çastit. Nëse nuk ka dokument të hapur, sillet
vlera e ngjyrës me atë emër prej ngjyrave të dokumentit parazgjedhje.

Mund të sjellë NotFoundError nëse nuk gjendet emri i ngjyrës.
Mund të sjellë ValueError nëse është dhënë emër i pavlefshëm ngjyre.
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
        <translation>changeColor(&quot;emër&quot;, c, m, y, k)

Ndryshon ngjyrën &quot;emër&quot; në vlerën e treguar CMYK. Vlera e ngjyrës përcaktohet
nga katër përbërs c = Cyan, m = Magenta, y = Yellow and k = Black.
Përbërësit e ngjyrës do të duhej të ishin brenda intervalit nga 0 te 255.

Mund të sjellë NotFoundError nëse emri i ngjyrës nuk gjendet.
Mund të sjellë ValueError nëse është dhënë emër i pavlefshëm ngjyre.</translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">defineColor(&quot;emër&quot;, c, m, y, k)

Përcakton një ngjyrë të re &quot;emër&quot;. Vlera e ngjyrës përcaktohet nga katër përbërës:
c = Cyan, m = Magenta, y = Yellow and k = Black. Përbërësit e ngjyrës do të duhej 
të ishin brenda intervalit nga 0 te 255.

Mund të sjellë ValueError nëse është dhënë emër i pavlefshëm ngjyre.</translation>
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
        <translation>deleteColor(&quot;emër&quot;, &quot;zëvendëso&quot;)

Fshin ngjyrën &quot;emër&quot;. Çdo hasje e asaj ngjyre zëvendësohet nga
ngjyra &quot;zëvendëso&quot;. Nëse nuk jepet një, &quot;zëvendëso&quot; merr si vlerë ngjyrën e tejdukshme
&quot;Asnjë&quot;.

deleteColor funksionon në ngjyra dokumenti parazgjedhje nëse nuk ka dokument të hapur.
Në atë rast, &quot;zëvendëso&quot;, nëse është caktuar, nuk ka efekt.

Mund të sjellë NotFoundError nëse nuk gjendet emri i ngjyrës.
Mund të sjellë ValueError nëse është dhënë emër i pavlefshëm ngjyre.</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;emër&quot;, &quot;zëvendëso&quot;)

Çdo hasje e ngjyrës &quot;emër&quot; zëvendësohet me ngjyrën &quot;zëvendëso&quot;.

Mund të sjellë NotFoundError nëse nuk gjendet emri i ngjyrës.
Mund të sjellë ValueError nëse është dhënë emër i pavlefshëm ngjyre.</translation>
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
        <translation>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; varg me emër kartele

Shfaq një kuti dialogu Hap Kartelë me caption &quot;caption&quot;. Kartelat filtrohen me
vargun e filtrit &quot;filtër&quot;. Mund të jepet edhe një emër kartele ose shteg kartele,
lëreni bosh këtë varg kur doni të mos e përdorni.  Vlera True për haspreview 
aktivizon një kuti të vogël paraparjesh te kutiza për Përzgjedhje Kartelash.  Kur
parametri issave është caktuar True dialogu vepron si një dialog &quot;Ruaj Si&quot; 
përndryshe vepron si &quot;Dialog Hapje Kartelash&quot;. Parazgjedhja për të dy parametrat
opsionalë është False.

Filtri, nëse tregohet, merr formën &apos;koment (*.tip *.tip2 ...)&apos;.
Për shembull &apos;Pamje (*.png *.xpm *.jpg)&apos;.

Këshillohuni me dokumentimin e Qt-së mbi QFileDialog për hollësi  mbi filtra.

Shembull: fileDialog(&apos;Open input&apos;, &apos;CSV files (*.csv)&apos;)
Shembull: fileDialog(&apos;Save report&apos;, defaultname=&apos;report.txt&apos;, issave=True)
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
        <translation>messageBox(&quot;titull&quot;, &quot;mesazh&quot;,
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
    button2=BUTTON_NONE, button3=BUTTON_NONE) -&gt; integer

Shfaq një kuti mesazhesh me titullin &quot;titullin&quot;, mesazhin &quot;mesazh&quot;, dhe
një ikonë &quot;ikonë&quot; dhe deri në tre butona. Si parazgjedhje nuk përdoret ikonë 
dhe shfaqet një buton i vetëm, OK. Lipsen vetëm argumentat titull dhe mesazh,
edhe pse vendosja e një ikone dhe butonit(ave) këshillohet me forcë. 
Teksti i mesazhit mund të përmbajë shënime të thjeshtë tip HTML.

Përgjigjet me numrin e butonit që shtypi përdoruesi. Numrat e butonave fillojnë 
nga 1.

Për parametrat e ikonës dhe butonit ka konstante të paracaktuara të mundshme
me emra të njëjtë si në Dokumentimin e Qt. Këto janë konstantet BUTTON_* dhe
ICON_* përcaktuar te moduli. Ka po ashtu dy konstante ekstra që
pranojnë binary-ORed me konstante butonash:
    BUTTONOPT_DEFAULT   Shtypja e Enter-it shtyp këtë buton.
    BUTTONOPT_ESCAPE    Shtypja e Esc-ut shtyp këtë buton.

Shembuj përdorimi:
result = messageBox(&apos;Script failed&apos;,
                    &apos;This script only works when you have a text frame selected.&apos;,
                    ICON_ERROR)
result = messageBox(&apos;Monkeys!&apos;, &apos;Something went ook! &lt;i&gt;Was it a monkey?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Konstante të përcaktuara butonash dhe ikonash:
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
        <translation>valueDialog(titull, mesazh [,defaultvalue]) -&gt; string

Shfaq dialogun e zakonshëm &apos;Kërko për varg&apos; dhe sjell vlerën e vargut
Parametra: titull dritareje, tekst në dritaren dhe vlerë opsionale &apos;parazgjedhje&apos;.

Shembull: valueDialog(&apos;titull&apos;, &apos;tekst në dritaren&apos;, &apos;opsionale&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Mbyll dokumentin e çastit pa pyetur për ruajtje apo jo.

Mund të shfaqë NoDocOpenError nëse nuk ka dokument që të mbyllet</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Përgjigjet me true nëse ka një dokument të hapur.
</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;)

Hap dokumentin &quot;emër&quot;.

Mund të sjellë ScribusError nëse dokumenti nuk hapet dot.
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

Ruan dokumentin e çastit me emrin e çastit, përgjigjet me true nëse ka sukses.
Nëse dokumenti nuk është ruajtur ende, kjo mund të sjellë një dialog ruajtje.

Nëse ruajtja dështon, hëpërhë nuk ka rrugë se si të merret vesh.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;name&quot;)

Ruan dokumentin e çastit nën një emër të ri &quot;emër&quot; (i cili mund të jepet me shteg 
të plotë ose relativ).

Mund të sjellë ScribusError nëse ruajtja dështon.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;autor&quot;, &quot;të dhëna&quot;, &quot;përshkrim&quot;) -&gt; bool

Cakton të dhëna dokumenti. &quot;Autor&quot;, &quot;Të dhëna&quot;, &quot;Përshkrim&quot; janë
vargje.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Cakton mënjanat e dokumentit, mënjanat Majtas(lr), Djathtas(rr), Sipër(tr) 
dhe Poshtë(br) jepen në njësi matëse të përdorur për dokumentin - shihni 
konstantet UNIT_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type)

Ndryshon njësinë matëse të dokumentit. Vlera të mundshme për &quot;njësinë&quot; përcaktohen
si konstante UNIT_&lt;tip&gt;.

Mund të sjellë ValueError nëse jepet një njësi e pavlefshme.
</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (konstante Scribus për njësi)

Përgjigjet me njësitë marëse të dokumentit. Vlera e sjellë do të jetë ajo e
konstanteve UNIT_*:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;emër kartele&quot;)

Ngarkon në dokumentin e çastit stile paragrafësh prej dokumentit Scribus 
te &quot;emër kartele&quot;.
</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(facingPages, firstPageLeft)

Cakton tipin e dokumentit. Për të pasur faqe për karshi vëreni parametrin e parë si
FACINGPAGES, për të mos përdorur facingPages përdorni NOFACINGPAGES.  Nëse doni
që faqja e parë të jetë faqe majtas vëreni parametrin e dytë si FIRSTPAGELEFT, për 
faqe djathtas përdorni FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;emër&quot;]) -&gt; string

Përgjigjet me emrin e ngjyrës për vijën e objektit &quot;emër&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;emër&quot;]) -&gt; integer

Përgjigjet me gjerësinë e vijës për objektin &quot;emër&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;emër&quot;]) -&gt; integer

Përgjigjet me vlerën e hijes së ngjyrës për vijën e objektit &quot;emër&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;emër&quot;]) -&gt; integer (shihni konstante)

Përgjigjet me stil bashkimi për vijën e objektit &quot;emër&quot;. Nëse nuk jepet &quot;emër&quot; 
përdoret objekti i përzgjedhur për çastin.  Tipet e bashkimit janë:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;emër&quot;]) -&gt; integer (shihni konstante)

Përgjigjet me stil kulmi për vijën e objektit &quot;emër&quot;. Nëse nuk jepet &quot;emër&quot;
përdoret objekti i përzgjedhur për çastin. Tipet e kulmeve janë:
CAP_FLAT, CAP_ROUND, CAP_SQUARE</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;emër&quot;]) -&gt; integer (shihni konstante)

Përgjigjet me stil vije për objektin &quot;emër&quot;. Nëse nuk jepet &quot;emër&quot; përdoret 
objekti i përzgjedhur për çastin.  Konstante tipi vijash janë:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;emër&quot;]) -&gt; integer

Përgjigjet me vlerën e hijes së ngjyrës së mbushjes për objektin &quot;emër&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">getCornerRadius([&quot;emër&quot;]) -&gt; integer

Përgjigjet me rrezen e kulmit për objektin &quot;emër&quot;. Rrezja shprehet
në pikë. Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;emër&quot;]) -&gt; (x,y)

Përgjigjet me një çift (x, y) që përmban vlera ripërmasimi për kornizën e pamjes
&quot;emër&quot;.  Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;emër&quot;]) -&gt; string

Përgjigjet me emrin e kartelës së pamjes në kornizën e pamjes.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="obsolete">getPosition([&quot;emër&quot;]) -&gt; (x,y)

Përgjigjet me një çift (x, y) për pozicionin e objektit &quot;emër&quot;.
Nëse nuk tregohet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.
Pozicioni shprehet në njësinë matëse aktuale të dokumentit
- për sqarime shihni UNIT_&lt;tip&gt;.
</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;emër&quot;]) -&gt; (width,height)

Përgjigjet me një çift (gjerësi, lartësi) për madhësinë e objektit &quot;emër&quot;.
Nëse nuk tregohet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin. 
Madhësia shprehet në njësinë matëse aktuale të dokumentit - për sqarime shihni UNIT_&lt;tip&gt;.</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;emër&quot;]) -&gt; integer

Përgjigjet me rrotullimin e objektit &quot;emër&quot;. Vlera shprehet në gradë,
dhe kahu orar merr vlera pozitive. Nëse nuk jepet &quot;emër&quot; përdoret 
objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Përgjigjet me një listë që përmban emrat e tërë objekteve në faqen e çastit.
</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;emër&quot;])

Zhvendos objektin &quot;emër&quot; te një vendndodhje e re. Koordinatat shprehen në 
njësinë matëse të çastit për dokumentin (shihni konstante UNIT).  Nëse nuk 
është dhënë &quot;emër&quot; përdoret objekti i përzgjedhur për çastin. Nëse objekti 
&quot;emër&quot; i përket një grupi, zhvendoset tërë grupi.
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
        <translation type="unfinished">rotateObject(rot [, &quot;emër&quot;])

Rrotullon objektin &quot;emër&quot; në mënyrë relative me gradë &quot;rot&quot;. Objekti rrotullohet
rotated by the vertex that is currently selected as the rotation point - by
default, the top left vertext at zero rotation. Vlerat pozitive do të thonë
rrotullim kundërorar kur përdoret pikë parazgjedhje rrotullimi. Nëse nuk është dhënë
&quot;emër&quot; përdoret përbërësi i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">rotateObjectAbs(rot [, &quot;emër&quot;])

Cakton rrotullimin e objektit &quot;emër&quot; sa &quot;rot&quot;. Vlerat pozitive
nënkuptojnë rrotullim në kah antiorar. Nëse nuk jepet &quot;emër&quot; përdoret
objekti i përzgjedhur për çastin. 
</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(width, height [, &quot;name&quot;])

Ripërmason objektin &quot;emër&quot; në gjerësinë dhe lartësinë e dhënë. Nëse nuk 
jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; string

Përgjigjet me emrin e objektit të përzgjedhur. &quot;nr&quot;, nëse jepet, tregon numrin
e objektit të përzgjedhur, p.sh. 0 do të thotë objekti i parë, 1 do të thotë
i dyti i përzgjedhur e me radhë.
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Përgjigjet me numrin e objekteve të përzgjedhur.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;emër&quot;)

Përzgjedh objektin me &quot;emrin&quot; e dhënë.
</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Çpërzgjedh tërë objektet në tërë dokumentin.
</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(list)

Grupon bashkë objektet e përmendur në &quot;listë&quot;. &quot;Lista&quot; duhet të përmbajë
emrat e objekteve që do të grupohen. Nëse nuk jepet &quot;emër&quot; përdoret objektet e 
përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;emër&quot;)

Shpërbën grupin te i cili bën pjesë objekti &quot;emër&quot;. Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
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
        <translation>scaleGroup(factor [,&quot;emër&quot;])

Ripërmason grupin të cilit i përket objekti me &quot;emër&quot;. Vlera më të mëdha se 1 e zgjerojnë
grupin, vlera më të vogla se 1 e bëjnë grupin më të vogël p.sh. një vlerë 0.5
e ripërmason grupin në 50 % të madhësisë fillestare, një vlerë 1.5 e ripërmason grupin
sa 150 % e vlerës së tij fillestare.  Vlera për &quot;faktorin&quot; duhet të jetë më e madhe se
0. Nëse jepet &quot;emër&quot; do të përdoret ai i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse jepet një koeficient i pavlefshëm ripërmasimi.
</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;emër kartele&quot; [, &quot;emër&quot;])

Ngarkon pamjen &quot;pamje&quot; te korniza e pamjes &quot;emër&quot;. Nëse nuk jepet 
&quot;emër&quot; përdoret objekti i përzgjedhur për çastin.

Mund të sjellë WrongFrameTypeError nëse korniza objektiv nuk është një kornizë pamjeje
</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;emër&quot;])

Cakton koeficientët për ripërmasim pamjesh në kornizën e pamjeve &quot;emër&quot;.
Nëse nuk jepet emër &quot;emër&quot; përdoret objekti i përzgjedhur për çastin. Vlera 1
nënkupton 100 %.

Mund të sjellë WrongFrameTypeError nëse korniza objektiv nuk është kornizë pamjesh
</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;emër&quot;]) -&gt; bool

Kyç objektin &quot;emër&quot; nëse është i çkyçur ose e çkyç në qoftë i kyçur.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin. Përgjigjet me &quot;true&quot;
nëse gjen kyçje.
</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;emër&quot;]) -&gt; bool

Përgjigjet me &quot;true&quot; nëse objekti &quot;emër&quot; është i kyçur.  Nëse nuk jepet &quot;emër&quot; 
përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Cakton ripërmasimin për kornizë në kornizën e përzgjedhur ose të treguar të 
pamjeve sa `scaletoframe&apos;. Nëse tregohet `proportional&apos;, caktoni ripërmasim me koeficient të fiksuar si `proportional&apos;.
Si `scaletoframe&apos; ashtu edhe `proportional&apos; janë vlera buleane.

Mund të sjellë WrongFrameTypeError.
</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Përgjigjet me një listë me emrat e tërë gërmave të mundshme.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; listë vlerash

Përgjigjet me më tepër të dhëna gërmash. Është një listë vlerash me:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</translation>
    </message>
    <message>
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</source>
        <translation type="obsolete">rendeFont(&quot;name&quot;, &quot;emër kartele&quot;, &quot;shembull&quot;, madhësi) -&gt; bool

Krijon një paraparje pamjeje të gërmave &quot;emër&quot; me tekstin e dhënë &quot;shembull&quot; dhe madhësinë.
Pamjet ruhen te &quot;emër kartele&quot;. Përgjigjet me &quot;true&quot; në rast suksesi.

Mund të sjellë NotFoundError nëse nuk gjenden gërmat e treguara.
Mund të sjellë ValueError nëse jepet një shembull ose një emër kartele bosh.
</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list

Përgjigjet me një listë me emrat e tërë shtresave të përcaktuara.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;emër&quot;)

Vë si shtresë vepruese shtresën e emërtuar &quot;emër&quot;.

Mund të sjellë NotFoundError nëse nuk gjendet shtresa.
Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

Përgjigjet me emrin e shtresë vepruese për çastin.
</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;shtresë&quot; [, &quot;emër&quot;])

Dërgon objektin &quot;emër&quot; te shtresa &quot;shtresë&quot;. Shtresa duhet të ekzistojë.
Nëse nuk gjendet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.

Mundet të sjellë NotFoundError nëse nuk gjendet shtresa.
Mundet të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;shtresë&quot;, e dukshme)

Cakton nëse shtresa &quot;shtresë&quot; të jetë apo jo e dukshme. Në qoftë e dukshmja &quot;false&quot;
shtresa është e padukshme.

Mund të sjellë NotFoundError nëse shtresa nuk gjendet.
Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;shtresë&quot;, e shtypshme)

Cakton shtresën &quot;shtresë&quot; si të shtypshme ose jo. Nëse e shtypshme është
vendosur si &quot;false&quot; shtresa nuk do të jetë e shtypshme.

Mund të sjellë NotFoundError nëse shtresa nuk gjendet.
Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the Layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">isLayerPrintable(&quot;shtresë&quot;) -&gt; bool

Përgjigjet nëse Shtresa &quot;shtresë&quot; është e dukshme ose jo, vlera True do të thotë 
që shtresa &quot;shtresë&quot; është e dukshme, vlera False do të thotë 
që shtresa &quot;shtresë&quot; është e padukshme.

Mund të sjellë NotFoundError nëse shtresa nuk gjendet.
Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">isLayerPrintable(&quot;shtresë&quot;) -&gt; bool

Përgjigjet nëse Shtresa &quot;shtresë&quot; është e shtypshme ose jo, vlera True do të thotë 
që shtresa &quot;shtresë&quot; është e shtypshme, vlera False do të thotë që shtypja e
shtresës &quot;shtresë&quot; është e çaktivizuar.

Mund të sjellë NotFoundError nëse shtresa nuk gjendet.
Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;shtresë&quot;)

Fshin shtresën me emrin &quot;shtresë&quot;. Nuk bën gjë nëse shtresa nuk 
ekziston ose nëse është shtresa e vetme në dokument.

Mund të sjellë NotFoundError nëse shtresa nuk gjendet.
Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(shtresë)

Krijon shtresë të re me emrin &quot;shtresë&quot;.

Mund të sjellë ValueError nëse emri i shtresës është i papranueshëm.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

Përgjigjet me një varg me vlerën -lang.
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
        <translation>createEllipse(x, y, gjerësi, lartësi, [&quot;emër&quot;]) -&gt; string

Krijon një elips të ri në faqen e çastit dhe jep edhe emrin e tij.
Koordinatat jepen në njësinë matëse të çastit për dokumentin
(shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një identifikues i vetëm
për objektin sepse ju duhet ky emër për përdorim të mëvonshëm të 
këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
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
        <translation>createImage(x, y, gjerësi, lartësi, [&quot;emër&quot;]) -&gt; string

Krijon një pamje të re në faqen e çastit dhe jep edhe emrin e saj. Koordinatat 
jepen në njësinë matëse të çastit për dokumentin. &quot;emri&quot; duhet të jetë 
një identifikues i vetëm për objektin sepse ju duhet ky emër për përdorim 
të mëvonshëm të këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që është 
tashmë i përdorur.
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
        <translation>createText(x, y, gjerësi, lartësi, [&quot;emër&quot;]) -&gt; string

Krijon një kornizë të re teskti në faqen e çastit dhe jep edhe emrin e tij. 
Koordinatat jepen në njësinë matëse të çastit për dokumentin
(shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një identifikues i vetëm
për objektin sepse ju duhet ky emër për përdorim të mëvonshëm të 
këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
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
        <translation>createLine(x1, y1, x2, y2, [&quot;emër&quot;]) -&gt; string

Krijon një vijë të re prej pikës(x1, y1) te pika(x2, y2) dhe jep edhe emrin e saj. 
Koordinatat jepen në njësinë matëse të çastit për dokumentin
(shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një identifikues i vetëm
për objektin sepse ju duhet ky emër për përdorim të mëvonshëm të 
këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
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
        <translation>createPolyLine(listë, [&quot;emër&quot;]) -&gt; string

Krijon një vijë të re të thyer dhe jep edhe emrin e saj. . Pikat për vijën e thyer
paraqiten në listën &quot;listë&quot; në rendin vijues: [x1, y1, x2, y2...xn. yn].
Koordinatat jepen në njësinë matëse të çastit për dokumentin
(shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një identifikues i vetëm
për objektin sepse ju duhet ky emër për përdorim të mëvonshëm të 
këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
Mund të sjellë ValueError nëse jepet një numër i pamjaftueshëm pikash ose
nëse numri i vlerave nuk përkon pa mbetje me pikat.



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
        <translation>createPolygon(listë, [&quot;emër&quot;]) -&gt; string

Krijon një shumëkëndësh të ri dhe jep edhe emrin e tij. Pikat për shumëkëndëshin
paraqiten në listën &quot;listë&quot; në rendin vijues: [x1, y1, x2, y2...xn. yn]. Lipsen të paktën
tre pika. Nuk ka nevojë të rijepet pika e parë për të mbyllur shumëkëndëshin. 
Shumëkëndëshi mbyllet automatikisht duke lidhur pikën e parë me të fundit.  
Koordinatat jepen në njësinë matëse të çastit për dokumentin
(shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një identifikues i vetëm
për objektin sepse ju duhet ky emër për përdorim të mëvonshëm të 
këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
Mund të sjellë ValueError nëse jepet një numër i pamjaftueshëm pikash ose
nëse numri i vlerave nuk përkon pa mbetje me pikat.
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
        <translation>createBezierLine(list, [&quot;emër&quot;]) -&gt; string

Krijon një lakore të re Bezier dhe jep edhe emrin e saj. Pikat për lakoren Bezier
paraqiten në listën &quot;listë&quot; në rendin vijues:
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]
Në listën e pikave, x dhe y nënkuptojnë koordinatat x dhe y të pikës dhe kx
e ky nënkupton pikat e kontrollit të lakores.  Koordinatat jepen në njësinë matëse 
të çastit për dokumentin (shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një 
identifikues i vetëm për objektin sepse ju duhet ky emër për përdorim të 
mëvonshëm të këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
Mund të sjellë ValueError nëse jepet një numër i pamjaftueshëm pikash ose
nëse numri i vlerave nuk përkon pa mbetje me pikat.
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
        <translation>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;emër&quot;]) -&gt; string

Krijon një pathText të ri duke përzjerë dy objektet &quot;textbox&quot; dhe &quot;beziercurve&quot; 
dhe jep edhe emrin e tij. Koordinatat jepen në njësinë matëse të çastit për 
dokumentin (shihni konstantet UNIT). &quot;emri&quot; duhet të jetë një identifikues 
i vetëm për objektin sepse ju duhet ky emër për përdorim të mëvonshëm 
të këtij objekti. Nëse nuk jepet &quot;emër&quot; Scribus-i do të krijojë një për ju.

Mund të sjellë NameExistsError nëse jepni shprehimisht një emër që 
është tashmë i përdorur.
Mund të sjellë NotFoundError nëse nuk gjenden një ose të dy objektet bazë të emërtuar.
</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;emër&quot;])

Fshin objektin me emrin &quot;emër&quot;. Nëse nuk jepet &quot;emër&quot; fshihet objekti i 
përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;emër&quot; [, gjendje])

Aktivizon/çaktivizon karakteristikën &quot;Teksti Rrjedh Përreth Kornize&quot; për 
objektin &quot;emër&quot;. Thirru me emër vargu parametrash dhe &quot;gjendje&quot; buleane 
opsionale. Nëse nuk jepet &quot;gjendje&quot;, këmbehet gjendja për rrjedhje teksti.
</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;emër&quot;]) -&gt; bool

Sheh nëse një objekt me emrin e treguar gjendet vërtet në dokument.
Parametri opsional është emri i objektit. Kur nuk tregohet emër objekti,
përgjigjet me True nëse ka diçka të përzgjedhur.
</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;stil&quot; [, &quot;emër&quot;])

Zbaton &quot;stilin&quot; e përmendur te objekti i quajtur &quot;emër&quot;. Nëse nuk jepet emër
objekti, zbatohet mbi objektin e përzgjedhur.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Përgjigjet me një listë emrash të tërë stileve të paragrafëve në dokumentin e përzgjedhur.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Përgjigjet me numrin e faqes ku punohet për çastin. Numrat e faqeve numërohen prej 1
e sipër, pa pyetur se cili është numri i faqes së parë në dokumentin tuaj.
</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Rivizaton tërë faqet.
</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;emër&quot;)

Ruan faqen e çastit si një kartelë EPS me emrin &quot;emër&quot;.

Mund të sjellë ScribusError nëse dështon ruajtja.
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

Fshin faqen e dhënë. Nuk bën gjë nëse dokumenti ka vetëm një faqe.
Numrat e faqeve numërohen nga 1 e sipër, pa pyetur se cili është 
numri i shfaqur për faqen e parë.

Mund të sjellë IndexError nëse numri i faqes është jashtë intervali
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

Zhvendoset te faqja &quot;nr&quot; (pra, bën të çastit faqen &quot;nr&quot;). Vini re që
gotoPage nuk ndryshon (hëpërhë) faqen të cilën parësi i përdoruesit 
po shfaq, thjesht cakton faqen mbi të cilën do të zbatohen urdhrat e programthit.

Mund të sjellë IndexError nëse numri i faqes është jashtë intervali.
</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; integer

Përgjigjet me numrin e faqeve të dokumentit.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

Përgjigjet me një listë që përmban pozicionet e udhëzuesve horizontalë. Vlerat jepen
njësitë e çastit për dokumentin - shihni konstante UNIT_&lt;tip&gt;.
</translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(list)

Cakton udhëzuesat horizontalë. Parametri i dhënies duhet të jetë një listë pozicionesh 
udhëzuesish matur kundrejt njësisë së çastit të dokumentit - shih konstantet UNIT_&lt;type&gt;.

Shembull: setHGuides(getHGuides() + [200.0, 210.0] # shto udhëzues të rinj pa ndonjë humbje
         setHGuides([90,250]) # zëvendëso tërësisht udhëzuesa të çastit
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Shihni getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Shihni setHGuides.
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Përgjigjet me një dyshe me përmasat e faqes matur në njësitë e çastit për dokumentin.
Shihni konstantet UNIT_&lt;type&gt; dhe getPageMargins()
</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; list

Përgjigjet me një listë vargjesh objektesh të faqes së çastit. Vargu është:
(emër, objectType, rend) P.sh. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
do të thotë që objekti me emrin &apos;Text1&apos; është një kornizë teksti (type 4) dhe është 
i pari në faqen...
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="obsolete">getPageMargins()

Përgjigjet me mënjanat e faqes (majtas, djathtas, sipër, krye) në njësitë e çastit.
Shihni konstantet UNIT_&lt;tip&gt; dhe getPageSize().
</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;ngjyrë&quot;, [&quot;emër&quot;])

Cakton si ngjyrën e mbushjes për objektin &quot;emër&quot; ngjyrën &quot;ngjyrë&quot;. &quot;ngjyra&quot;
është emri i njërës prej ngjyrave të përcaktuara. Nëse nuk jepet &quot;ngjyrë&quot;
përdoret ajo e objektit të përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;ngjyrë&quot;, [&quot;emër&quot;])

Cakton si ngjyrën e vijës për objektin &quot;emër&quot; ngjyrën &quot;ngjyrë&quot;.  Nëse nuk jepet 
&quot;ngjyrë&quot; përdoret ajo e objektit të përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(gjerësi, [&quot;emër&quot;])

Cakton gjerësi vije për objektin &quot;emër&quot; sa &quot;gjerësi&quot;. &quot;gjerësia&quot; duhet të jetë 
brenda segmentit nga 0.0 deri 12.0, dhe e matur në pikë. Nëse nuk jepet &quot;ngjyrë&quot;
përdoret ajo e objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse gjerësia e vijës është jashtë kufijsh.
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
        <translation>setLineShade(hije, [&quot;emër&quot;])

Cakton si hijëzimin e ngjyrës së vijës së objektit &quot;emër&quot; &quot;hijen&quot;.
&quot;hija&quot; duhet të jetë një numër i plotë brenda intervalit 0 (më e ndritshmja)
deri 100 (intensitet i plotë ngjyre). Nëse nuk jepet &quot;emër&quot; përdoret ai 
i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse hija e vijës është jashtë kufijve.
</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;emër&quot;])

Cakton si stil bashkimi të vijës për objektin &quot;emër&quot; stilin &quot;bashkim&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin. Ka
konstante të parapërcaktuara për bashkim - JOIN_&lt;tip&gt;.
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endtype, [&quot;emër&quot;])

Cakton si stil maje të vijës për objektin &quot;emër&quot; stilin &quot;majë&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin. Ka
konstante të parapërcaktuar për &quot;majë&quot; - CAP_&lt;tip&gt;.
</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(stil, [&quot;emër&quot;])

Cakton si stil të vijës së objektit &quot;emër&quot; stilin &quot;stil&quot;. Nëse nuk jepet
&quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin. Ka konstante 
të parapërcaktuara për &quot;stil&quot; - LINE_&lt;stil&gt;.
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
        <translation>setFillShade(hije, [&quot;emër&quot;])

Cakton si hijëzimin e ngjyrës së mbushjes së objektit &quot;emër&quot; &quot;hijen&quot;.
&quot;hija&quot; duhet të jetë një numër i plotë brenda intervalit 0 (më e ndritshmja)
deri 100 (intensitet i plotë ngjyre). Nëse nuk jepet &quot;emër&quot; përdoret ai 
i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse hija e mbushjes është jashtë kufijve.
</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(rreze, [&quot;emër&quot;])

Cakton rrezen e kulmit të objektit &quot;emër&quot;. Rrezja matet në pikë. 
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse rrezja e kulmit është negative.
</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;emër&quot;])

Cakton si stil vije për objektin &quot;emër&quot; stil e emërtuar &quot;namedStyle&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse stili i vijës nuk ekziston.
</translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;emër&quot;]) -&gt; string

Përgjigjet me emrin e gërmave të kornizës së tekstit me &quot;emër&quot;. 
Nëse korniza e tekstit ka ca tekst të përzgjedhur sillet vlera 
e caktuar për gërmën e parë të përzgjedhjes. Nëse nuk jepet &quot;emër&quot; 
përdoret ai i objektit të përzgjedhur të çastit.
</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;emër&quot;]) -&gt; integer

Përgjigjet me gjatësinë e tekstit të kornizës së teksit me &quot;emër&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;emër&quot;]) -&gt; string

Përgjigjet me tekstin e kornizës së tekstit &quot;emër&quot;. Nëse kjo kornizë teksti 
ka ca tekst të përzgjedhur, sillet teksti i përzgjedhur. Sillet tërë teksti 
i kornizës, jo thjesht teksti i dukshëm për çastin. Nëse nuk jepet &quot;ngjyrë&quot;
përdoret ajo e objektit të përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>getAllText([&quot;emër&quot;]) -&gt; string

Përgjigjet me tekstin e kornizës së tekstit &quot;emër&quot; dhe tërë kornizave të tekstit 
të cilat janë të lidhura te kjo kornizë. Nëse kjo kornizë teksti ka ca tekst 
të përzgjedhur, sillet teksti i përzgjedhur. Nëse nuk jepet &quot;emër&quot; përdoret 
ai i objektit të përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;emër&quot;]) -&gt; float

Përgjigjet me hapësirë vije (&quot;leading&quot;) e kornizës së tekstit &quot;emër&quot; shprehur në
pikë. Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;emër&quot;]) -&gt; float

Përgjigjet me madhësinë e hapësirës së shtyllës së kornizës së tekstit &quot;emër&quot; 
shprehur në pikë.  Nëse nuk jepet &quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;emër&quot;]) -&gt; integer

Përgjigjet me numrin e shtyllave të kornizës së tekstit &quot;emër&quot;. Nëse nuk jepet 
&quot;emër&quot; përdoret objekti i përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;tekst&quot;, [&quot;emër&quot;])

Rregullon tekstin e kornizës së tekstit &quot;emër&quot; si tekst të llojit &quot;tekst&quot;. Teksti 
duhet të jetë i koduar në UTF8 - përdorni p.sh. unikod (tekst, &apos;iso-8859-2&apos;). 
Shihni FAQ për më tepër hollësi. Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit 
të përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame. Text
must be UTF encoded (see setText() as reference) The first character has an
index of 0. &quot;name&quot; If &quot;name&quot; is not given the currently selected Item is
used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="obsolete">insertText(&quot;tekst&quot;, pos, [&quot;emër&quot;])

Fut tekstin &quot;tekst&quot; te pozicioni &quot;pos&quot; brenda kornizës së tekstit. Teksti
duhet të jetë i koduar në UTF (për referencë shihni setText()) Gërma e parë ka
tregues 0. nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.

Mund të sjellë IndexError për futje jashtë kufijve.
</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation>setFont(&quot;gërma&quot;, [&quot;emër&quot;])

Rregullon gërmat e kornizës së tekstit &quot;emër&quot; si &quot;gërmat&quot;. Nëse ka ca tekst 
të përzgjedhur, vetëm teksti i përzgjedhur ndryshohet.  Nëse nuk jepet &quot;emër&quot; 
përdoret ai i objektit të përzgjedhur të çastit.

Mund të sjellë ValueError nëse gërmat në fjalë nuk gjenden dot.
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
        <translation>setFontSize(size, [&quot;emër&quot;])

Rregullon madhësinë e gërmave të kornizës së tekstit &quot;emër&quot; sa &quot;madhësi&quot;. 
&quot;size&quot; trajtohet si vlerë në pikë. Nëse ka ca tekst të përzgjedhur, vetëm 
teksti i përzgjedhur ndryshohet. &quot;madhësi&quot; duhet të jetë brenda intervalit 
1 deri 512. Nëse nuk jepet &quot;emër&quot; përdoret ai i emrit të objektit të përzgjedhur.

Mund të sjellë ValueError në rast madhësie gërmash që është 
jashtë kufijsh.
</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(size, [&quot;emër&quot;])

Cakton si hapësirë vije (&quot;leading&quot;) të kornizës së tekstit &quot;emër&quot;
&quot;madhësinë&quot;. &quot;madhësia&quot; është një vlerë në pikë. Nëse nuk jepet 
&quot;emër&quot; përdoret ai i objektit të përzgjedhur të çastit.

Mund të sjellë ValueError nëse hapësirë vijash është jashtë kufijsh.
</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(size, [&quot;name&quot;])

Cakton boshllëk shtylle të kornizës së tekstit &quot;emër&quot; sa vlera &quot;madhësi&quot;. 
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse boshllëku i shtyllës është jashtë kufijsh (duhet të jetë pozitiv).
</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(nr, [&quot;emër&quot;])

Cakton numrin e shtyllave të kornizës së tekstit &quot;emër&quot; sa numri i plotë &quot;nr&quot;.
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.

Mund të sjellë ValueError nëse numri i shtyllave nuk është të paktën një.
</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(drejtim, [&quot;emër&quot;])

Rregullon drejtimin e tekstit të kornizës së tekstit &quot;emër&quot; sipas drejtimit të treguar.
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin. &quot;drejtimi&quot; 
do të duhej të ishte një nga konstantet ALIGN_ përcaktuar në këtë modul - shihni dir(scribus).

Mund të sjellë ValueError në rast konstanteje të pavlefshme drejtimi.
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
        <translation type="obsolete">selectText(fillim, numërim, [&quot;emër&quot;])

Përzgjedh shenja &quot;numërimi&quot; të tekstit në kornizën e tekstit &quot;emër&quot; duke filluar 
prej shenjës &quot;fillim&quot;. Numërimi i shenjave nis nga 0. Nëse &quot;numërimi&quot; është zero, 
do të pastrohet çfarëdo përzgjedhje teksti. Nëse &quot;numërimi&quot; është -1, do të 
përzgjidhet tërë teksti në kornizën. Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit
të përzgjedhur për çastin.

Mund të sjellë IndexError nëse përzgjedhja është jashtë kufijve të tekstit.
</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;emër&quot;])

Fshin çfarëdo teksti në kornizën e tekstit &quot;emër&quot;. Nëse ka ca tekst të përzgjedhur,
do të fshihet vetëm teksti i përzgjedhur. Nëse nuk jepet &quot;emër&quot; pëdore ai i objektit 
të përzgjedhur për çastin.
</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;ngjyrë&quot;, [&quot;emër&quot;])

Cakton si ngjyrë teksti për kornizën e tekstit &quot;emër&quot; ngjyrën &quot;ngjyrë&quot;. Nëse ka
ca tekst të përzgjedhur ndryshohet vetëm teksti i përzgjedhur. 
Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit të përzgjedhur për çastin.
</translation>
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
        <translation>setTextShade(hije, [&quot;emër&quot;])

Cakton si hije të ngjyrës së tekstit të objektit &quot;emër&quot; &quot;hijen&quot;. Nëse ka 
ndoca tekst të përzgjedhur ndryshohet vetëm teksti i përzgjedhur. 
&quot;hija&quot; duhet të jetë një vlerë e plotë brenda segmentit nga 0 (më e hapta) 
te 100 (intensitet i plotë ngjyre). Nëse nuk jepet &quot;emër&quot; përdoret ai i objektit
të përzgjedhur.
</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;ngaemër&quot;, &quot;teemër&quot;)

Lidh dy korniza tekstesh. Korniza e emërtuar &quot;ngaemër&quot; lidhet te
korniza e emërtuar &quot;teemër&quot;. Korniza objektiv duhet të jetë një kornizë
teksti bosh dhe nuk duhet të ketë lidhje nisen apo përfundojnë në të 
prej kornizash të tjera.

Mund të sjellë ScribusException nëse dhunohen rregulla lidhjeje.
</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;emër&quot;)

Hiq objektin e treguar (me emër) prej kornize flow/linkage teksti. Nëse
korniza qe në mes të një zinxhiri, do të lidhen kornizat paraardhëse dhe
pasuese, p.sh. &apos;a-&gt;b-&gt;c&apos; bëhet &apos;a-&gt;c&apos; kur ju unlinkTextFrames(b)&apos;

Mund të sjellë ScribusException nëse dhunohen rregulla lidhjeje.
</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;emër&quot;])

Shndërron kornizën e tekstit &quot;emër&quot; në vija të jashtme. Nëse nuk jepet &quot;emër&quot;
përdoret ai i objektit të përzgjedhur për çastin.</translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
</source>
        <translation type="obsolete">register_macro_code(name, sourcetext, accel=&apos;&apos;)

Krijo një makro të quajtur &quot;emër&quot; duke çmuar kodin burim &quot;sourcetext&quot;.
&quot;sourcetext&quot; duhet të pajtohet me të njëjtët rregulla si makrot e krijuara në GUI.
Në u dhëntë, vargu &quot;accel&quot; do të përdoret për të rregualluar një shkurtprerje
tastiere për makron.
</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Heq rregullimet e mëparshmëm për shtyllë përparimi te Scribus. Thirret para
përdorimit rishtas të shtyllës së përparimit. Shihni progressSet.
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Cakton vlerën më të madhe për hapat e shtyllës së përparimit sa vlera e dhënë.
Shihni progressSet.
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

Cakton venndodhjen e shtyllës së përparimit sa &quot;nr&quot;, një vlerë relative ndaj
asaj të rregulluar më parë për progressTotal. Shtylla e përparimit përdor
konceptin e hapave; i jepni numrin gjithsej të hapave dhe atë të hapave të
plotësuar deri tani dhe do t&apos;ju shfaqë përqindjen e hapave që janë plotësuar. 
Mund të caktoni numrin gjithsej të hapave me progressTotal(). Numri i çastit
për hapat caktohet nga progressSet(). Shtylla e përparimit mund të riniset nga
fillimi me progressReset(). [bazuar në të dhëna marrë nga dokumente prej 
të Trolltech-ut mbi Qt-në]
</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[E PAMBULUAR!] Kjo mund t&apos;i prishë gjërat, ndaj larg prej kësaj hëpërhë.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Aktivizon/çaktivizon ikonë ruajtjesh te shtyllë ikonash të Scribus-it dhe pjesën te menuja Ruaj. 
Është e dobishme për thirrjen e një procedure kur po ndryshoni dokumentin, ngaqë Scribus-i
nuk do të vërë re automatikisht ndryshimin e dokumentit përmes përdorimit të një programthi.
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Rreth Scribus-it %1%2</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Version Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation>Build-ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Rreth</translation>
    </message>
    <message>
        <source>Programming:</source>
        <translation type="obsolete">Programim:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Pjesëmarrje prej:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Portë Windows:</translation>
    </message>
    <message>
        <source>Documentation:</source>
        <translation type="obsolete">Dokumentim:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Autorë</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Gjermane:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Frënge:</translation>
    </message>
    <message>
        <source>Spanish and Catalan:</source>
        <translation type="obsolete">Spanjolle e Katalane:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italiane:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Hungareze:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ukrainase:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Bullgare:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Galike:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turke:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Lituaneze:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Polake:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Çeke:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Slovake:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Daneze:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Norvegjeze:</translation>
    </message>
    <message>
        <source>English:</source>
        <translation type="obsolete">Angleze:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Welsh:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Ruse:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Braziliane:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finlandeze:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Slovene:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Baske:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Përkthime</translation>
    </message>
    <message>
        <source>Homepage and online reference</source>
        <translation type="obsolete">Faqja hyrëse dhe referencë online</translation>
    </message>
    <message>
        <source>Mailing list</source>
        <translation type="obsolete">Listë postimesh</translation>
    </message>
    <message>
        <source>Bugs and feature requests</source>
        <translation type="obsolete">Bug-e dhe kërkesa për gjëra të reja</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;I lidhur</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Ky panel tregon versionin, datë e krijimit dhe(new line)
librari suporti të përpiluara brenda Scribus-it(new line)
C-C-T do të thotë C=CUPS C=littlecms T=TIFF suport.(new line)
Librari suport që mungojnë tregohen me a *</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Reth Scribus-it %1</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Ekipi Zhvillues:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Dokumentacioni Zyrtar:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Dokumentacion Tjetër:</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Përkthime dhe Përkthyes Zyrtarë:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Katalane:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Anglishte (Britanike):</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Koreane:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Serbe:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Spanjolle:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Suedeze:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Kontribues Përkthimesh të Dikurshëm:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Faqja Hyrëse</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referencë Online</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Bug-e dhe Kërkesa për Anë të Reja</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Listë Postimesh</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Ky panel tregon versionin, datën e krijimit dhe
 suport librarish përpiluar në Scribus
C-C-T është e barazvlefshme me suport C=littlecms C=CUPS T=TIFF.
Mungesë suporti librarish tregohet me një *</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation>%1 %2 %3</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation>Portë Windows:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.
Missing library support is indicated by a *</source>
        <translation>Ky panel shfaq versionin, datë krijimi dhe
suport të përpiluar në librari në Scribus
C-C-T-F është e njëvlefshme me C=littlecms C=CUPS T=suport TIFF F=suport Fontconfig.
Mungesa e suportit në librari tregohet me a *</translation>
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
        <translation>&amp;Hap...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Ruaj</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Ri&amp;kthehu tek i Ruajtur</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Merr Tekst...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Vini &amp;Tekst...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Merr Pamje...</translation>
    </message>
    <message>
        <source>Import Page(s)...</source>
        <translation>Importo Faqe...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Ruaj &amp;Tekst...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Ruaj Faqen si &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Ruaje si P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>Të &amp;dhëna Dokumenti...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>&amp;Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Shtyp...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Lër</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ribëj</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Pri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>Pa&amp;stro Përmbajtje</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Përzgjidh &amp;Tërë</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>&amp;Çpërzgjidh Tërë</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Kërko/Zëvendëso...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>Ngj&amp;yra...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Stile &amp;Paragrafi...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>Stile &amp;Vije...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Stampa...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Javascripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>P&amp;arapëlqime...</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Tjetër...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Majtas</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Qendër</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Djathtas</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blloko</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Detyrueshëm</translation>
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
        <translation>&amp;Nënvijë</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Hequrvije</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>Si&amp;përshkrim</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>Po&amp;shtëshkrim</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Jashtëvijëzuar</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Përmbys</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabelëzuesa...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;yfisho</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Dyfishim i &amp;Shumëfishtë</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Grupo</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>&amp;Çgrupo</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Është &amp;Kyçur</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Madhë&amp;sia është Kyçur</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Shpjere &amp;Prapa</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Bjere &amp;Përpara</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Ul</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Ngri</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Shpërnda/&amp;Drejto...</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Shpjere në të &amp;Papastër</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Atribute...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Pa&amp;mje e Dukshme</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Është &amp;Faqerojtës PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Është Sh&amp;ënim PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Veti Shënimesh</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Veti Fushe</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Përpunoni Formë...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Bashkangjit Tekst tek Shteg</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Shqit Tekst prej Shtegu</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Ndërthur Shumëkëndësha</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Ndaj &amp;Shumëkëndësha</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Kurbë &amp;Bezier</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>Kornizë &amp;Pamjesh</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Jashtëvija</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Shumëkëndësh</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>Kornizë &amp;Teksti</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Shenjë...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Tekst Shembull</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Fut...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Fshij...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopjo...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Zhvendos...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Zbato Stampë...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Administro &amp;Udhëzuesa...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>&amp;Sa dritarja</translation>
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
        <translation>Fi&amp;gurëza</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Shfaq &amp;Mënjana</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Shfaq &amp;Korniza</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Shfaq &amp;Pamje</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Shfaq &amp;Udhëzuesa</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Shfaq Zinxhir &amp;Tekstesh</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Veti</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;E papastër</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Shtresa</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>Paletë &amp;Faqeje</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Faqerojtësa</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Matje</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>&amp;Historik Veprimesh</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Mjete</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Mjete P&amp;DF</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Përzgjidhni Objekt</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>T&amp;abelë</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Formë</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Vijë</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Vijë me &amp;Dorë të Lirë</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Fut Kornizë Teksti</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Fut Kornizë Pamje</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Fut Tabelë</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Fut Formë</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Fut Shumëkëndësh</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Fut Vijë</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Fut Kurbë Bezier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Fut Vijë me Dorë të Lirë</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rrotullo Objekt</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Zmadho ose zvogëlo</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Zmadho</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Zvogëlo</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Përpunoni Përmbajtje Kornize</translation>
    </message>
    <message>
        <source>Text...</source>
        <translation>Tekst...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Lidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Zgidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Kopjo Veti Objekti</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Përpunoni tekstin me Përpunues Rrëfimesh</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Administro Pamje</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Vijëzo Tekst</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Prodho Tabelë Përmbajtjeje</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Rreth Scribus-it</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Rreth &amp;Qt-s</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Ndih&amp;mëza</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Numër &amp;Faqeje</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Të drejta kopjimi</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Shenjë Tregtare e Regjistruar</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Shenjë Tregtare</translation>
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
        <translation>Apostrof</translation>
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
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Mundësi të Mëtejshme</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Pasqyro Faqe(t) &amp;Horizontalisht</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Pasqyro Faqe(t) &amp;Vertikalisht</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Zbato Gjatë &amp;Heqje Ngjyre</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Zbato Profile &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Krijo PostScript Level 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Krijon vetëm PostScript Level 2, mbani parasysh,(new line)
që kjo mund të krijojë kartela të stërmëdha</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Krijon vetëm PostScript Level 1, mbani parasysh,(new line)
që kjo mund të krijojë kartela të stërmëdha</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Një rrugë braktisjeje e disa shkallëve të grisë që janë të përbëra nga(new line)
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.(new line)
UCR prek më tepër pjesë pamjeje që janë tone neutralë dhe/ose të errët(new line)
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje(new line)
dhe lipsen ca eksperimente dhe prova për çdo herë.
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Drejto</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontalisht</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Anët Majtas</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Të mesit</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Anët Djathtas</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Ndërmjet:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;Mos Ndrysho</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>&amp;Drejto</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>Z&amp;hvendoje</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Shpërnda &amp;Njësoj</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikalisht</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Anë të Sipërme</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Anë të Poshtme</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>Ndë&amp;rmjet:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>Mos Ndr&amp;ysho</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Dr&amp;ejto</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Zh&amp;vendosje</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Shpërnda Një&amp;soj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Zbato</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Vendos Tekstin Majtas</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Vendos Tekstin Djathtas</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Vendos Tekstin në Qendër</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Vendos Tekstin Përligjshëm</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Vendos Tekstin Detyrimisht Përligjshëm</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Veti Fushe</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tip:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Buton</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Fushë Tekstesh</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kutizë</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Listë Kuti</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Kuti Liste</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Veti</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Emër:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Ndihmëz:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Gërma për përdorim me PDF 1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Anë</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Ngjyrë:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Gjerësi:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>E ngushtë</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>E gjerë</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>E plotë</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Me vija</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Nënvijë</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Tjetër</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Vetëm Lexim</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>E nevojshme</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Mos Eksporto Vlerë</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Dukshmëri:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>I dukshëm</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>I padukshëm</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Pa Shtypje</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Pa Parje</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Dukje</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Tekst për Button Down</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Tekst për Roll Over</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ikona</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Përdor ikona</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Hiq</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Roll Over</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Vendosje Ikone...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Thekso</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Përmbys</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Push</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Shumëvijëshe</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Fjalëkalim</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Kufi i </translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Gërma</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Mos Rrëshqit</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Mos Kontrollo Drejtshkrimin</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Kontrollo Stil:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Kontrollo</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kryq</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Rreth</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Yllkë</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Katror</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Nëse Parazgjedhja është Shenjuar</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>E përpunueshme</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Mundësi</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Shko Tek</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Parashtro Formular</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Rirregullo Formular</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importo të Dhëna</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Ngjarje:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Miu Up</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Miu Down</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Miu Enter</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Miu Exit</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Në Fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Në Turbull</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Programth:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Përpunoni...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Parashtro te URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Parashtro të Dhëna si HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importo të Dhëna prej:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Vendmbërritje</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Në Kartelë:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Ndërro...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Faqe:</translation>
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
        <translation>X-Poz:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Veprim</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Fusha është formatuar si:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Thjesht</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Numër</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Përqindje</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datë</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Kohë</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vetjake</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Format Numrash</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Dhjetore:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Përdor Simbol Paraje</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Parangjit Simbol Paraje</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Po formatoj</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Format Përqindjesh</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Format Date</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Format Kohor</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Programthe Vetjakë</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Taste:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Vlera nuk është vleftësuar</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Vlera duhet të jetë më e madhe ose e baraz:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>dhe më e vogël ose baraz me:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Programth vetjak vleftësimi:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Vleftëso</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Vlera nuk është llogaritur</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Vlera është</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>mbledhje</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation>mesatare</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>më i vogël</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>më i madh</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>e fushave vijuese:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Merr...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Programth vetjak llogaritjeje:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Llogarit</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Flamurka është shpërfillur për PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Jepni këtu një listë fushash ndarë me presje</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Ju duhet të paktën Ikona për Normal që të përdorni Ikona Për Butona</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Pamje (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Shembull:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Ndryshim Përzgjedhjeje</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Kartela PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Veti Shënimesh</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="obsolete">Tip:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lidhje</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Lidhje e Jashtme</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Lidhje Web e Jashtme</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Vendmbërritje</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ndërro...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="obsolete">Faqe:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Poz:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">X-Poz:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Dokumente PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tip:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Faqe:</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Zbato Stampë</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Stampë:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Zbato te Faqe e Ça&amp;stit</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Zbato te tërë Faqet &amp;çifte</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Zbato te tërë Faqet &amp;teke</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Zbato prej &amp;Faqeje:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>Për:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Apply to &amp;current page</source>
        <translation>Zbato te faqe e &amp;çastit</translation>
    </message>
    <message>
        <source>Apply to all &amp;even pages</source>
        <translation>Zbato te tërë faqet &amp;tek</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd pages</source>
        <translation>Zbato te tërë faqet &amp;çift</translation>
    </message>
    <message>
        <source>Apply to &amp;all pages:</source>
        <translation>Zbato te &amp;tërë faqet:</translation>
    </message>
</context>
<context>
    <name>ApplyTemplate</name>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Apply Template</source>
        <translation>Zbato Stampë</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Stampë:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Zbato Te</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Faqe e &amp;çastit</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>Faqe &amp;Çift</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>Faqe &amp;Tek</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Tërë faqet</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;Brenda intervalit</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Zbato stampën e përzgjedhur te faqe teke, çifte ose tërë faqet brenda intervalit vijues&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>to</source>
        <translation>te</translation>
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
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>E papastër</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Ngarko...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Ruaj</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>E &amp;vogël</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>E &amp;mesme</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>E m&amp;adhe</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Paraparje</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Scrapbooks (*.scs);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Riemërto</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Fshij</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Emri &quot;%1&quot; s&apos;është unik.(new line)
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Emër:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Zë i Ri</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Faqerojtësa</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Lëviz Faqerojtës</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Fut Faqerojtës</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Faqerojtësa</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Vendosje Ikone</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Skemë:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Vetëm marrje</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Vetëm ikona</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Marrje nën Ikonë</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Marrje mbi Ikonë</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Marrje djathtas Ikonës</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Marrje majtas Ikonës</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Marrje përmbi Ikonë</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Shkallë:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Përherë</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Kur një Ikonë është tepër e vogël</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Kur një Ikonë është tepër e madhe</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Kurrë</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Si ta Ripërmasoj:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Përpjestimore</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Jo Përpjestimore</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ikonë</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Rimerr</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Rregullime Administrimi Ngjyrash</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>&amp;Aktivizo Administrim Ngjyrash</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Profile Sistemi</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Pamje:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Ngjyra Të &amp;plota:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Sht&amp;ypës:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Kolorimetrik Relativ</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Kolorimetrik Absolut</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Sht&amp;ypje:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ulo Shtypës në Ekran</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Përdor Kompensim &amp;Blackpoint</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Profil parazgjedhje ngjyrash për pamje të importuara</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Profil parazgjedhje ngjyrash për ngjyra të plota te faqja</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Profil ngjyrash i prodhuar nga ju apo marrë prej prodhuesit.(new line)
Ky profil do të duhej të ishte i veçantë për monitorin tuaj dhe jo një profil i përgjithshëm (bie fjala sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Profil ngjyrash për modelin tuaj të shtypësit nga prodhuesi.(new line)
Ky profil do të duhej të ishte i veçantë për shtypësin tuaj dhe jo një profil i përgjithshëm (bie fjala sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Aktivizo &apos;soft proofing&apos; rreth se si do të shtypeshin ngjyrat e dokumentit,(new line)
bazuar në profilin e zgjedhur për shtypësin.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Metodë shfaqjeje në ekran ngjyrash të cilat mund të mos shtypen si duhet.(new line)
Kjo lyp profile shumë të saktë dhe shërben vetëm si sinjalizim.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Kompensim Pikash të Zeza është një metodë për përmirësimin e kontrastit në foto.(new line)
Këshillohet ta aktivizoni këtë nëse keni foto në dokumentin tuaj.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Përpunoni Ngjyrë</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Emër:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Model Ngjyre</translation>
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
        <translation>RGB E përshtatshme për Web</translation>
    </message>
    <message>
        <source>New</source>
        <translation>E re</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>E vjetër</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Tabelë HSV ngjyrash</translation>
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
        <translation>Shtylla Dinamike Ngjyrash</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Shtylla Statike Ngjyrash</translation>
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
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Emri i Ngjyrës nuk është i vetëm</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Nuk mund të krijoni një ngjyrë të quajtur &quot;%1&quot;.(new line)
Është emër i ruajtur për ngjyrë të tejdukshme</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
        <translation>Mund të shihni një figurëz nëse shtypni
dhe mbani të shtypur butonin e djathtë të miut

Tasti Insert fut një Shenjë te Përzgjedhja nën të
dhe tasti Delete heq atë të futur së fundmi</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Përzgjidhni Gërma:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Gërma:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Klasë Gërmash:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Fut</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Fut gërma në tekst atje ku gjendet kursori</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Fshij përzgjedhje të çastit.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Mbyll këtë dialog dhe kthehu te përpunimi i tekstit.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Grup i Plotë Gërmash</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Latine Bazë</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Shtesë Latin-1</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latine Të zgjeruara-A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latine Të Zgjeruara-B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Shenja të Përgjithshme Pikësimi</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Sipër- dhe Poshtëshkrime</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Simbole Monetarë</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Simbole të Ngjashëm me Gërma</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Forma Numrash</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Shigjeta</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Operatorë Matematikë</translation>
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
        <translation>Forma Gjeometrike</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Simbole të Ndryshëm</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbats</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Lidhëse</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Speciale</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Greke</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Greke të Zgjeruara</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Cirilike</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Cirilike Shtesë</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabike</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabike të Zgjeruara A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabike të Zgjeruara B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebraike</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Current Profile:</source>
        <translation>Profil i Çastit:</translation>
    </message>
    <message>
        <source>Items</source>
        <translation>Objekte</translation>
    </message>
    <message>
        <source>Problems</source>
        <translation>Probleme</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>&amp;Shpërfill Gabime</translation>
    </message>
    <message>
        <source>Glyphs missing</source>
        <translation>Shenja që mungojnë</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objekti nuk është në ndonjë Faqe</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Pamje që Mungon</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Pamja ka DPI-Value më të vogël se %1 DPI</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Objekti ka tejdukshmëri</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekti është Shënim ose Fushë PDF-je</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objekti është PDF e vendosur</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Nuk u gjetën probleme</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Faqe</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Objekte të Lirë</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>U gjetën probleme</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Zgjidhni Stile</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Stile të Mundshëm</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Njëngjyrëshe</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Anologe</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Përplotësuese</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Treshe</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Katërshe (Dopjo Përplotësuese)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Color Wheel</source>
        <translation>Rrethi i Ngjyrave</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Ngjyrë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
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
        <translation>Përzgjidhni Metodë:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Kënd (0 - 90 gradë):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>&amp;Përzieni Ngjyra</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>&amp;Zëvendëso Ngjyra</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Përzje ngjyrat e krijuara me ngjyrat e dokumentit</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Zëvendëso ngjyrat e krijuara te ngjyrat e dokumentit</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Lëri ngjyrat të paprekura</translation>
    </message>
    <message>
        <source>Difference between selected value and counted ones. See documentation for more info</source>
        <translation>Ndryshimi ndërmjet vlerash të përzgjedhura dhe atyre të tashmë në lojë. Për më tepër të dhëna shihni dokumentimin</translation>
    </message>
    <message>
        <source>Click the wheel to get base color</source>
        <translation>Klikoni rrethin për të marrë ngjyrën bazë</translation>
    </message>
    <message>
        <source>Here you have the sample color schema</source>
        <translation>Këtu ju tregohet skema e ngjyrës shembull</translation>
    </message>
    <message>
        <source>Select one of the method to create color schema. See documentation for more info</source>
        <translation>Përzgjidhni një nga metodat për të krijuar skemë ngjyre. Shihni dokumentimin për më tepër të dhëna</translation>
    </message>
    <message>
        <source>Here you have the color of your chosen color schema</source>
        <translation>Këtu ju tregohet ngjyra e skemës së zgjedhur për ngjyrën</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Po përzie ngjyra</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Gabim:</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Po hap përgjegjësin e ngjyrave.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Përzierje Ngjyrash</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Hije:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Patejdukshmëri:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Shkallëzim Horizontal</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Shkallëzim Vertikal</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Shkallëzim Diagonal</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Shkallëzim Ndër Diagonal</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Shkallëzim Radial</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Shkallëzim linear i lirë</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Shkallëzim rrezor i lirë</translation>
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
        <translation>Përpunoni Veti Ngjyre Vije</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Përpunoni Veti Ngjyre Mbushëse</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Ngopje ngjyre</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Metodë mbushjeje normale ose me shkallëzim</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Rregulloni tejdukshmërinë për ngjyrën e përzgjedhur</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Ngjyrë objekti të përzgjedhur</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Zhvendos Vektor</translation>
    </message>
    <message>
        <source>Move the start of the Gradient Vector with the left Mouse Button pressed and
nove the end of the Gradient Vector with the right Mouse Button pressed</source>
        <translation>Zhvendosni fillimin e Vektorit Gradient me Butonin e majtë të Miut të shtypur dhe
zhvendosni fundin e Vektorit Gradient me Butonin e djathtë të Miut të shtypur</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Mundësi Importuesi CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Kufizues fushe:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Kufizues vlere:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Rreshti i parë është titull</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Mundësi Shtypësi</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Faqe e Rregulluar</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Tërë Faqet</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Vetëm Faqe Çift</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Vetëm Faqe Tek</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Pasqyro</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Jo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Po</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientim</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Së gjeri</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Shtypje N-Up</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Faqe për Fletë</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Faqe për Fletë</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Mundësi</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vlerë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Ky panel paraqet mundësi të ndryshme CUPS kur shtypet. (new line)
Parametrat e saktë të mundshëm do të varen nga &quot;driver&quot;-i i shtypësit tuaj.(new line)
Mund të keni suport CUPS duke përzgjidhur Ndihmë &gt; Rreth.(new line)
Shih për përmendje të: C-C-T Kjo vlen për suport C=CUPS C=littlecms T=TIFF.(new line)
Mungesa e suportit për librarinë tregohet me një *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Ngjesh Kartelë</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Përfshi Gërma</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodim:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Shkon te Drejtoria juaj e Dokumenteve.(new line)
Kjo mund të rregullohet te Parapëlqimet.</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Fshij Ngjyrë</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Fshij ngjyrë:</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Zëvendësoje me:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Fshij Ngjyrë:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Zëvendëso Me:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Fshij Faqe</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Fshij prej:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>për:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Fshij Prej:</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Gërma që Mungojnë</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Gërmat %1 nuk janë instaluar.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Përdor</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">më mirë</translation>
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
        <translation>Të dhëna Dokumenti</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titull:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Fjalëkyça:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Përshkrim:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Botues:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Pjesë&amp;marrës:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Datë:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ip:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormat:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identifi&amp;kues:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>B&amp;urim:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Gjuhë:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Marrëdhënie:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Mbu&amp;lim:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Të &amp;drejta:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Të &amp;dhëna të Mëtejshme</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Personi apo organizmi përgjegjës para të gjithësh për hartimin e përmbajtjes së dokumentit.(new line)
Kjo fushë mund të trupëzohet në dokumente Scribus për referencë, si dhe në metadatat e një PDF-je</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Emër dhënë dokumentit.(new line)
Kjo fushë mund të trupëzohet në dokumente Scribus për referencë, dhe edhe në metadatën e një PDF-je</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Një përshkrim i përmbajtjes së dokumentit.(new line)
Roli i kësaj fushe është një përshkrim të shkurtër ose abstrakt i dokumentit. Gjatë eksportimit trupëzohet në PDF-në</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Tema e përmbajtjes së dokumentit.(new line)
Kjo fushë është për fjalëkyça dokumenti që doni të trupëzohen në një PDF, për të ndihmuar kërkime dhe indexim kartelash PDF</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Person ose organizëm përgjegjës për bërjen e dokumentit të passhëm</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Person ose organizëm përgjegjës për pasje pjesëmarrjeje në përmbajtjen e dokumentit</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Datë shoqëruar një ngjarjeje në jetën e dokumentit, në format VVVV-MM-DD, si në ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Natyra ose zhanri i përmbajtjes së dokumentit, psh. kategori, funksione, zhanre, etj</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Paraqitja fizike ose numerike e dokumentit. Do të ishte me leverdi përmendja e tipit të medias dhe përmasat.(new line)
Gjithashtu të dobishme janë RFC2045, RFC2046 për tipe MIME</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Një referencë jo e dykuptimtë te dokumenti brenda një konteksti të caktuar si p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Një referencë te një dokument prej të cilit dokumenti i pranishëm ka rrjedhur, p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Gjuha në të cilën përmbajtja e dokumentit është shkruajtur, zakonisht kod ISO-639 gjuhe(new line)
mundet edhe ndarë me vizë nga prapashtesë kodi ISO-3166 vendi, p.sh. en-GB, fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Një referencë te një dokument i afërm, mundësisht duke përdorur një identifikues formal si p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Shtrirja ose fokusi i përmbajtjes së dokumentit, mundësisht duke përfshirë shtrirje juridike,hapësinore dhe kohore</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Të dhëna rreth të drejtash të ruajtura rreth dhe mbi dokumentin, p.sh të drejta kopjimi, patenta ose shenja tregtare</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Lidhet Me</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Është Prind I</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Është Pjellë E</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Korniza Tekstesh</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Korniza Pamjesh</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Buleane</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>I plotçËëÇ</translation>
    </message>
    <message>
        <source>String</source>
        <translation>Varg</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Veti Objekti Dokumenti</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tip</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vlerë</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parametër</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Marrëdhënie</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Marrëdhënie Me</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Vetështo Te</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Shto</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshi</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Pa&amp;stro</translation>
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
        <translation>Rregulloni Shtypës</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Destinacion Shtypësi</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Kartelë</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Mundësi...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Kartelë:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Urdhër &amp;Alternativ Shtypësi</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Përdorni një përgjegjës shtypjesh alternativ, si kprinter ose gtklp,(new line)
për të bërë të përdorshme mundësi shtypjesh shtesë</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Urdhër:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Interval</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Shtyp &amp;Tërë</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Shtyp Fa&amp;qen e Çastit</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>&amp;Interval Shtypjeje</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Jepni një listë objektesh të ndarë me presje ku(new line)
objekti mund të jetë * për tërë faqet, 1-5 për(new line)
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>&amp;Numër Kopjesh:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Mundësi</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Shtypje &amp;Normale</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>&amp;Veçime Shtypjeje</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tërë</translation>
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
        <translation>Yellow</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Të zezë</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Sht&amp;yp Me Ngjyra Nëse Mundet</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Shtyp Me Shkallë të &amp;Grisë</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Mundësi të Mët&amp;ejshme...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Shtyp</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Ruaj  si</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Kartela Postscript (*.ps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Postscript Files (*.ps);;All Files (*)</source>
        <translation>Kartela Postscript (*.ps);;Tërë Kartelat(*)</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Importimi i Kartelës:(new line)
%1(new line)
dështoi!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Gabim Fatal</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="obsolete">Po përpunoj Makro: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Përgjegjës Makrosh</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="obsolete">Ka një kartelë &apos;%1&apos; tashmë.
Jeni të sigurtë se doni të mbishkruhet?
</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation type="obsolete">E keni përpunuar tashmë këtë makro.
Jeni të sigurtë se doni të hidhni tej tërë ndryshimet tuaj?
</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation type="obsolete">Vijon një gjurmim i plotë:

%1
</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation type="obsolete">Dështoi përpunimi i makros, ndaj nuk mund 
të ruhet në formën e tanishme. Gabimi qe:
%1
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Scribus - Makro e Re</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Kjo është kutia e dialogut Përpunoni Makro / Makro e Re. Këtu mund të ndryshoni kodin burim për makrot. Përpunoni kodin burim për makron te zona për përpunim teksti nën etiketën &quot;Kod Burim&quot; dhe klikoni OK për të ruajtur ndryshimet te makroja.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation type="obsolete">Kod Burim:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="obsolete">Po përpunoj Makro:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="obsolete">Emri i makros që po përpunohet.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation type="obsolete">&lt;/qt&gt;Ky përbërës shfaq emrin e makros që jeni duke përpunuar për çastin.&lt;qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Hidh tej tërë ndryshimetDiscard all changes dhe dil.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Dilni nga dialogu i përpunimit, duke hedhur tej tërë ndryshimet që bëtë. Nëse doni të dilni pa ruajtur makron por nuk doni të humbni ndryshimet tuaj, ruajini ndryshimet me &quot;Ruaj Burim Si...&quot;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Ok</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Ruaj ndryshimet dhe dil.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Ruaj ndryshimet te makroja dhe dil. Nëse ka ndonjë problem me makron, do të shfaqet një mesazh dhe dialogu i përpunimit nuk do të mbyllet.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Kjo zonë teksti përmban kodin burim të makros. Nëse po krijoni një makro të re nuk do të ketë gjë brenda saj, dhe nëse po përpunoni një makro ekzistuese, këtu do të tregohet kodi burim që përcaktonte makron.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation type="obsolete">&amp;Ngarko Burim ...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Zëvendëso kodin e çastit me kod prej një kartele.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Ngarko prej një &quot;kartele&quot; kod të ri burim te zona e përpunimit . Çfarëdo kod burim në zonën e përpunimit zëvendësohet. Burimi i ngarkuar duhet të jetë një funksion makroje Scribus. Nëse ngarkoni një tjetër programth, do t&apos;ju duhet ta përpunoni para ruajtjes së tij që të mund të funksionojë si një programues makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="obsolete">&amp;Ruaj Burim Si...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Ruaj te një kartelë kodin burim që po përpunohet.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation type="obsolete">Ruaj në një kartelë kodin burim - tekstin - e makros. Mund ta përpunoni kodin e ruajtur dhe ta ngarkoni sërish me &quot;Ngarko Burim...&quot;.</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Përpunoni Stil</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Emër:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Gërmë</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>&amp;Gërma:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>Madhë&amp;si:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Efekt:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Drejtim:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="unfinished">Shkronja Fillimisht</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Rreshta:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Ngjyrë M&amp;bushje:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Hapësira Vertikale</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>Ndër&amp;vijë:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>Si&amp;për:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>P&amp;oshtë:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabelëzues dhe Kryeradhë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Emër i stilit tuaj për paragrafin</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Gërma për tekstin apo objektin e përzgjedhur</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Ngjyrë mbushjeje teksti</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Ndërro Ngjyrën e Tekstit</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Mundëson një gërmë të stërmadhe për gërmën e parë të një paragrafi. E përdorur për efekte stilistikë</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Hapësirë sipër paragrafit</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Hapësirë poshtë paragrafit</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Ndërvijë</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Emri i Stilit nuk është i vetëm</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Align to Baseline &amp;Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style:</source>
        <translation>Paraparje e Stilit të Paragrafit:</translation>
    </message>
    <message>
        <source>Sample text of this paragraph style</source>
        <translation>Tekst shembull për këtë stil paragrafi</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Përpunues</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Hap...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Ruaj dhe Dil</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Dil pa Ruajtur</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ribëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>P&amp;ri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Merr Emra Fushe</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation>Javascripts (*.js);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Zgjidhni Drejtori Eksportimi</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Eksporto si Pamje</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Eksporto te Drejtori:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Mundësi</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>T&amp;ip Pamjeje: </translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Cilësi:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Qartësi:</translation>
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
        <translation>Interval</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Faqe e &amp;çastit</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Tërë faqet</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Interval</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Eksporto një interval faqesh</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Jepni një listë objektesh të ndarë me presje ku(new line)
objekti mund të jetë * për tërë faqet, 1-5 për(new line)
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Eksportoni tërë faqet</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Eksporto vetëm faqen e çastit</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Qartësi e Pamjeve(new line)
Përdorni 72 dpi për Pamje të paramenduara për Ekranin</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Cilësia e pamjeve tuaja - 100% është më e mira, 1% cilësia më e ulët</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Formate të mundshëm eksporti</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Drejtoria e përfundimeve - vendi ku të ruhen pamjet tuaja.(new line)
Emri i kartelës eksport do të jetë &apos;emërdokumenti-numërfaqeje.tipkartele &apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Ndrysho drejtori përfundimesh</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Madhësi:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Madhësi e pamjeve. 100% për të mos patur ndryshime, 200% për dy herë më të madhe etj.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Vetë të Zgjeruara Pamjeje</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Errëso</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Zbardh</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Hue</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Ngopje</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Ngjyrë</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Ndriçim</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Shumëfisho</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Ekran</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished"></translation>
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
        <translation>Ndryshim</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Përjashtim</translation>
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
        <source>Blend Mode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Patejdukshmëri:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Sfond</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Shtresa</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Mos përdor ndonjë Shteg</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Shtigje</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Madhësi:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titull:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Pa Titull</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>E panjohur</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Dokument Scribus</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Qartësi:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation></translation>
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
        <translation>Shkallë të Grisë</translation>
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
        <translation>Ngjyra</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Hiq Të papërdorura</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Sete Ngjyrash</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Set Ngjyrash i Çastit:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>&amp;Ruaj Set Ngjyrash</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Zgjidhni një set njgyrash për t&apos;u ngarkuar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Ruaj setin e çastit për ngjyrat</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Hiqni ngjyra të papërdorura prej seti ngjyrash të dokumentit të çastit</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Vini te seti i çastit ngjyra prej një dokumenti ekzistues</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Krijoni ngjyrë të re brenda setit të çastit</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Përpunoni ngjyrën e përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Bëni një kopje të ngjyrës së përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Fshij ngjyrën e përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Bëj set ngjyrash të çastit set parazgjedhje ngjyrash</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Emër:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Zgjidhni Emër</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopje e %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Ngjyrë e Re</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Rregullime të Përgjithshme Gërmash</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Gërma të Mundshme</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Shtigje Shtesë</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Po</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Emër Gërmash</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Përdor Gërma </translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Trupëzo në:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Nënset</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tip</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Shteg për te Kartelë Gërmash</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Gërma të &amp;Mundshme</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Zëvendësim</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>Sht&amp;o...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Hiq</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Shtigje Shtesë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Zgjidhni një Drejtori</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Emër Gërmash</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Përdor Gërma </translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Trupëzo në:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Nënset</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Shteg për te Kartelë Gërmash</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Paraparje Gërmash</translation>
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
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Vër gërma të përzgjedhura te Stil, menu Gërmash</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Lër paraparje</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Emër Gërmash</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation>Dok</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation>Tip</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Nënset</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation>Hyrje</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation>Madhësi Gërmash:</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation>Paraparje Gërmash</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation>Kërkim i Shpejtë:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Kërko</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Vër gërma të përzgjedhura te Stil, menu Gërmash</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Lër paraparje</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation>Shtypja këtu e tekstit ofron kërkim të shpejtë nëpër emrat e gërmave. P.sh. &apos;bold&apos; shfaq tërë gërmat me Bold te emri. Kërkimi nuk ndikohet nga gërma të mëdha apo të vogla.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Fillo kërkimin</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Madhësi gërmash të përzgjedhura</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Përdorues</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>Sistem</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>This Document contains some Fonts that are not installed on your System,
please choose a suitable replacement for them.</source>
        <translation>Ky Dokument përmban ca Gërma që nuk janë të instaluara në Sistemin tuaj,
ju lutemi zgjidhni një zëvendësim të përshtatshëm për to.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Gërma Origjinale</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Gërma Zëvendësuese</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Bëji këta zëvendësime të përhershëm</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Pozicion:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Këtu mund të shtoni, ndryshoni ose hiqni Ndalesa Ngjyre.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Administroni Udhëzuesa</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Udhëzuesa Horizontalë</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Shto</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Udhëzuesa Vertikalë</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Poz:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Shto</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>Fshi&amp;j</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Kyç Udhëzuesa</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://scribus.net for updated docs and downloads.</source>
        <translation type="obsolete">Më ndjeni, nuk ka doracak të mundshëm! Ju lutem shihni: http://scribus.net për dokumente të përditësuar dhe shkarkime.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Përmbajtje</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lidhje</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Ndihmë Onlinë për Scribus-in</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Më ndjeni, nuk ka doracak të passhëm! Ju lutem shihni: http://docs.scribus.net për(new line)
dokumente të përditësuar dhe www.scribus.net për shkarkime.</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Përmbajtje</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Kërko</translation>
    </message>
    <message>
        <source>S&amp;earch</source>
        <translation>Kër&amp;ko</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Shto</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshi</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>&amp;Fshij Tërë</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Faqerojtësa</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Shtyp...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>&amp;Dil</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Kërkimi nuk ndikohet nga gërma të mëdha apo të vogla</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>e panjohur</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Gjej</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Term Kërkimi:</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Faqerojtës i Ri</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Titull Faqerojtësi të Ri:</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Ndarje me Vizë e Mundshme</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Prano</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Anashkalo</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Rregullime Ndarësi</translation>
    </message>
    <message>
        <source>&amp;Fully Automatic</source>
        <translation type="obsolete">&amp;Plotësisht Automatike</translation>
    </message>
    <message>
        <source>Check &amp;During Typing</source>
        <translation type="obsolete">Kontrollo &amp;Gjatë Shtypjes</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Gjuhë:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Fjala më e &amp;Vogël:</translation>
    </message>
    <message>
        <source>&amp;Number of Hypenations allowed:</source>
        <translation type="obsolete">&amp;Numër i lejuar Ndarjesh me Vijë:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>If you uncheck this you will get a dialog
everytime a possible Hyphenation is found.</source>
        <translation type="obsolete">Nëse i vini shenjë kësaj do të kini një dialog(new line)
sa herë që gjendet një Ndarje me Vizë.</translation>
    </message>
    <message>
        <source>Enables automatic checking of your text while typing.</source>
        <translation type="obsolete">Aktivizon kontroll të vetvetishëm të tekstit ndërsa shtypni.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Gjatësi e fjalës më të vogël për ta ndarë me vijë.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Numër më i madh Ndarjesh me Vizë që ndjekin njëra tjetrën.(new line)
Një vlerë 0 do të thotë ndarje me vizë të pakufizuara.</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Këshillime &amp;Ndarjeje</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Ndaj Tekst Automatikisht &amp;Gjatë Shtypjes</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Ndarje të &amp;Njëpasnjëshme të &amp;Lejuara:</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Kur përdorni Ekstra, mundësi Ndarjeje Teksti shfaqet një kuti dialogu që tregon tërë ndarësat e mundshëm për çdo fjalë.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Aktivizon ndarje automatike të tekstit tuaj në shtypje e sipër.</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Fut Faqe</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Po fus</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Faqe</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>para Faqes</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>pas Faqes</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>në Fund</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Po fus</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Stampë (Faqe Majtas):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Stampë:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Stampë (Faqe Djathtas):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Fut</translation>
    </message>
    <message>
        <source>Template (&amp;Left Page):</source>
        <translation>Stampë (Faqe &amp;Majtas):</translation>
    </message>
    <message>
        <source>Template (&amp;Right Page):</source>
        <translation>Stampë (Faqe &amp;Djathtas):</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Fut Tabelë</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Numri i Rreshtave:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Numri i Shtyllave:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Numër rreshtash:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Numër shtyllash:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Përpuno JavaScript</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Përpuno...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Shto...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>Programth i &amp;Ri:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Programth i Ri</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you really want do delete this Script?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Programth?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Po</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Doni vërtet të fshini këtë Programth?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Administro Shkurtprerje Tastiere</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Veprim</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Kyç i Çastit</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Përzgjidhni një Kyç për këtë Veprim</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Pa Tast</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Taste të Përcaktuar nga &amp;Përdoruesi</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Caktoni &amp;Tast</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
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
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Kjo Radhë Tastesh është në përdorim</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Grupe të Ngarkueshëm Shkurtprerjesh</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Ngarko</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importo...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>&amp;Eksporto...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Rimerr</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Grupe shkurtprerjesh tastierash të gatshëm për t´u ngarkuar</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Ngarko grupin e përzgjedhur të shkurtprerjeve</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Importo një grup shkurtprerjesh në formësimin e çastit</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Eksporto shkurtprerjet e çastit në një kartelë të importueshme</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Ringarko shkurtprerjet Scribus parazgjedhje</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>Kartela Gupe Tastesh XML (*.ksxml)</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Shtresa</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Shto Shtresë të re</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Fshij Shtresë</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Ngri Shtresë</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Ul Shtresë</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Shtresë e Re</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Doni të fshini tërë Objektet edhe në këtë Shtresë?</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Përpunoni Stile Vije</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopje e %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Stil i Ri</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you really want do delete this Style?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Stil?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Po</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Doni vërtet të fshini këtë Stil?</translation>
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
        <translation type="obsolete">inç</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekti i dhënë nuk mund të thirret</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="obsolete">Administro Makro</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="obsolete">Bie një dritare grafike për krijim, fshirje, përpunim, ruajtje dhe ngarkim makrosh.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="obsolete">Krijoni, përpunoni dhe fshini makro</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="obsolete">&amp;Makro</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Përgjegjës Makrosh</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="obsolete">I pazoti të hap kartelën e kërkuar: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="obsolete">Scribus - Përpunoni Makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Emri i makros që kërkuat është zënë tashmë nga një tjetër makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Krijimi i makros dështoi. Përgjegjësi i makros nuk qe në gjendje të rregullojë makron.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">Makroja &apos;%1&apos; ka raportuar një gabim të vockël.
Gabimi është: %2
Vijon një gjurmues i plotë:

%3
</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">Makroja &apos;%1&apos; nuk arriti të ekzekutohet saktë.
Gabimi është: %2
Vijon një gjurmues i plotë:

%3
</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Përgjegjës Makrosh</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation type="obsolete">Riemërtimi i makros dështoi sepse emri është tashmë në përdorim.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="obsolete">Scribus - Administroni Makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Kjo dritare përfaqëson Përgjegjësin e Makrove të Scribus-it. Këtu mund të krijoni makro, përpunoni makro, etj. Tërë ndryshimet bëhen duke përdorur butonat në anën e djathtë të dritares.&lt;/p&gt;
&lt;p&gt;Tërë ndryshimet e bërë në këtë dialog hyjnë në fuqi në çast - nuk mund të anuloni veprimet që bëni këtu.
Tabela në qendër të dialogut liston cilat makro janë për çastin të ngarkuara dhe disa të dhëna rreth tyre. Përdorni &quot;Ç&apos;është kjo&quot; prej tabele për më tepër të dhëna.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">E &amp;re</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="obsolete">Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Krijoni një makro të re.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Krijoni një makro të re duke filluar nga emri i makros mandej me hapjen e kutizës së dialogut të përpunimit të makrove.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Ok</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Close this dialog.</source>
        <translation type="obsolete">Mbyll këtë dialog.</translation>
    </message>
    <message>
        <source>Return to Scribus.</source>
        <translation type="obsolete">Kthehu te Scribus.</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="obsolete">Makro</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Përpunoni</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation type="obsolete">Përshpejt</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Përshkrim</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Kjo tabelë liston makrot që janë për çastin të përcaktuara.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Emër:&lt;/b&gt; Emri i makros, si tregohet në shtyllën e menusë dhe në vende të tjerë nëpër Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Përpunoni:&lt;/b&gt; Nëse makroja mund të përpunohet, në këtë shtyllë shfaqet &quot;Po&quot;. Zakonisht nëse një makro nuk mund të përpunohet ka pas qenë krijuar duke përdorur urdhrin register_macro në një programth.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Përshpejt:&lt;/b&gt; Radha e shtypjes së tasteve për menunë, në pastë, shoqëruar kësaj makroje. Për shembull, CTRL-F8 do të thotë që mund të shtypni Control-F8 kur jeni brenda Scribus-it për të xhiruar makron.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Përshkrim:&lt;/b&gt; Një varg special në krye të përkufizimit që e përshkruan, nëse makroja përmban një &quot;docstring&quot;,  dhe që shfaqet këtu. Nëse docstring-u është shumë i gjatë, tregohet vetëm fillimi - përdorni &quot;Ç&apos;është Kjo&quot; te zëri për makron te menuja Makro për të parë përshkrimin e plotë.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="obsolete">Ri&amp;emërto</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="obsolete">Alt+M</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="obsolete">Riemërtoni makron e përzgjedhur.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Riemërtoni makron e përzgjedhur. Do t&apos;ju kërkohet emri i ri.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Përpuno...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="obsolete">Alt+E</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Përpunoni burimin e makros së përzgjedhur, nëse burimi është i mundshëm.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Përpunoni makron e përzgjedhur. &lt;/p&gt;
&lt;p&gt;Nëse ky buton është gri, ose nuk është përzgjedhur makro, ose përgjegjësi i makrove nuk e ka kodin burim për makron që keni përzgjedhur (e në këtë rast do të shfaqet një &lt;tt&gt;Jo&lt;/tt&gt; te shtylla &lt;tt&gt;Përpunoni &lt;/tt&gt;e makros).&lt;/p&gt;
&lt;p&gt;Nëse Scribus-i nuk nuk e ka kodin burim, makroja mbase qe krijuar nga një programth.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Fshi</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Fshi makron e përzgjedhur për çastin.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Fshi makron e përzgjedhur. Kjo është e menjëhershme, dhe nuk ka rrugëdalje nëse fshihet një herë makroja. Nëse makroja është krijuar nga një programth që niset gjatë nisjes së kompjuterit, do të rishfaqet herës pasuese që ngarkoni ngarkoni Scribus-in.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation type="obsolete">&amp;Cakto Accel</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Rregulloni shkurtprerje tastiere për makron e përzgjedhur.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Caktoni tast shkurtprejrjeje tastiere (përshpejtues) për makron e përzgjedhur. Do t&apos;ju kërkohet shkurtprerja e re te kutia e dialogut.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="obsolete">&amp;Eksportoni</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="obsolete">Alt+X</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="obsolete">Eksporto makro si një kartelë.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Eksporto makro në një kartelë të jashtme. Kartela do të jetë një programth Python që përmban urdhra programuesi për të rikrijuar makron. Mund të xhirohet përmes &lt;tt&gt;Ngarko programth zgjerimi&lt;/tt&gt; prej menusë &lt;tt&gt;Programthe&lt;/tt&gt;, ose prej butonit import te përgjegjësi i makrove.&lt;/p&gt;
&lt;p&gt;Nëse doni një version të këndshëm, të lexueshëm nga syri i njeriut për makrot tuaja, përzgjidhni makron që doni, shtypni butonin &lt;tt&gt;Përpunoni&lt;/tt&gt;, dhe përdorni butonin &lt;tt&gt;Ruaj burim&lt;/tt&gt; te dialogu &lt;tt&gt;Përpunoni Makro&lt;/tt&gt;. Nuk do të jeni në gjendje të ngarkoni atë version me &lt;tt&gt;Ngarko programth zgjerimi&lt;/tt&gt; - në vend të kësaj, krijoni një makro të re me butonin &lt;tt&gt; E re&lt;/tt&gt; dhe përdorni &lt;tt&gt;Ngarko burim&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="obsolete">Fshi &amp;Tërë</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="obsolete">Fshi tërë makrot.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Fshi tërë makrot e regjistruara. Kjo është e menjëhershme, dhe nuk ka rrugë për të ripasur makrot e fshira. Çfarëdo makro e krijuar nga programthi i fillimit do të rishfaqet herës pasuese që ngarkoni Scribus-in.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importo</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation type="obsolete">Importo makro prej një kartele.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Ngarkon makro prej një kartele të jashtmee.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Kopje të Shumëfishtë</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Numër Kopjesh:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation type="unfinished">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation type="unfinished">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Largësira</translation>
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
        <translation>Kënd:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Gjatësi:</translation>
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
        <translation type="obsolete">inç</translation>
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
        <translation type="obsolete">Gabim programthi</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Nëse po xhironit një programth zyrtar ju lutemi raportojeni te &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Këtë mesazh e keni edhe në clipboard. Përdorni Ctrl+V për ta hedhur te gjurmues bug-esh.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Shfaq &amp;Konsol</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Fshih &amp;Konsol</translation>
    </message>
    <message>
        <source>Scribus - Script Plugin</source>
        <translation type="obsolete">Scribus - Shtojcë Programthesh</translation>
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
        <translation type="obsolete">Funksioni &apos;Ngarko Programth&apos; për shtojcën e programtheve është e çaktivizuar për çastin.
Nëse doni thjesht të xhironi një programth normal, mbase doni të përdorni programin
&apos;Ekzekuto Programth...&apos; më mirë.

Nëse në fakt doni të ngarkoni një programth Python zgjerimi ose makro, 
lipset të shkoni te Rregullime Programuesi te menuja Programthe dhe
aktivizoni atje zgjerime programuesi.

Ju lutem së pari lexoni dokumentimin mbi programthe zgjerimi.
</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation>Importoni Stampë</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importoni Faqe(t)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Prej Dokumenti:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Ndërro...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Importoni Faqe(t):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Jepni një listë objektesh të ndarë me presje ku(new line)
objekti mund të jetë * për tërë faqet, 1-5 për(new line)
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>prej 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Krijoni Faqe(t)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">para Faqes</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">pas Faqes</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">në Fund</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importo</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>nga %1</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>&amp;Prej Dokumenti:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>N&amp;drysho...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>&amp;Importoni Faqe(t):</translation>
    </message>
    <message>
        <source>&amp;Import Template</source>
        <translation>&amp;Importoni Stampë</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens importWhereData
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Jepni një listë objektesh importWhereData të ndarë(new line)
me presje ku objekti mund të jetë * për tërë faqet,(new line)
1-5 për një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Para Faqeje</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Pas Faqeje</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>në Fund</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importo</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Gërma që Mungojnë</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Gërmat %1 nuk janë instaluar.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Përdor</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>më mirë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Zhvendos Faqe</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Kopjo Faqe</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Zhvendos Faqe:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>te:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">para Faqes</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">pas Faqes</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">në Fund</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Zhvendos Faqe</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Para Faqeje</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Pas Faqeje</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Në Fund</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Veti</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Formë</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Pamje</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Vijë</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Ngjyra</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Gjeometri</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
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
        <translation>&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivel</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Formë:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Përpunoni Formë...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>K&amp;ulme(new line)
të Rrumbullakët:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Largësi e Tekstit</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Boshllëk:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Sipër:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Djathtas:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabelëzues...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Veti Teksti Shtegu</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Shfaq Kurbë</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation type="unfinished">Degëzim Vertical</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Largësi prej Kurbe:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Teksti &amp;Rrjedh Rreth Kornizës</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation type="unfinished">Kopjo Kuti Teksti</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Përdorni Rrethina Vije</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Madhësi Gërmash:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Hije:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Ndërvijë Vetjake</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>N&amp;dërvijë:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>St&amp;il:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>Gj&amp;uhë:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Ripërmasim i &amp;Lirë</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>X-Shk&amp;allë:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Y-Shk&amp;allë:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Ripërmaso &amp;Sa Madhësi Kornize</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;ërpjestimor</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Profil Futjesh:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Kolorimetrik Relativ</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Kolorimetrik Absolut</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Pikë Majtas</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Pika Skajore</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>T&amp;ip Vije:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Gjerësi Vije:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation type="unfinished">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation type="unfinished">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation type="unfinished">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>S&amp;kaje:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Majë e Sheshtë</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Majë Katrore</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Majë e Rrumbullakët</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Funde:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Pa Stil</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Vija Kutie</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Vijë Sipër</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Vijë Majtas</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Vijë Djathtas</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Vijë Poshtë</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Emër i objekti të përzgjedhur</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation type="unfinished">GtkRregullimi për vendndodhjen horizontale</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation type="unfinished">GtkRregullimi për vendndodhjen vertikale</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Gjerësi</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Lartësi</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Pikë së cilës i referohen matjet ose këndet e rrotullimeve</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation type="unfinished">Përzgjidhni një Kyç për këtë Veprim</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Kthe Horizontalisht</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Kthe Vertikalisht</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Ngrije një nivel</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Zbrite një nivel</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Zhvendos përpara</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Zhvendos prapa</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Tregon nivelin në të cilin gjendet objekti, 0 do të thotë që objekti është në fund fare</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Kyç ose çkyç objektin</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Kyç ose çkyç madhësinë e objektit</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Aktivizo ose çaktivizo shtypjen e objektit</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Bëj tekst nga kornizat e poshtme të rrjedhë përreth formës së objektit</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Përdor kuti rrethuese në vend të formës së kornizës për rrjedhë teksti</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Përdor një vijë të dytë bazuar fillimisht në formën e kornizës për rrjedhë teksti</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Gërma për tekstin apo objektin e përzgjedhur</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Ndryshim gjerësie shenjash</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Ndërro Ngjyrën e Tekstit</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Ngjyrë mbushjeje teksti</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Ngopje ngjyre teksti mbushjeje</translation>
    </message>
    <message>
        <source>Reverse Writing</source>
        <translation type="obsolete">Shkrim Së Prapthi</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Ndërvijë</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stil për paragrafin e çastit</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Gjuhë ndarjeje me vijë e kornizës</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Ndrysho rregullime për pika majtas ose në fund </translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Skemë vije</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Trashësi vije</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tip bashkimi vije</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tip fundi vije</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Stil vije e objektit të çastit</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Zgjidhni formën e kornizës...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Përpunoni formën e kornizës...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Caktoni rrezen e lakimit të kulmit</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Numër shtyllash në kornizë teksti</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Kalon midis gjerësie Shtylle apo Boshllëku</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Largësi ndërmjet shtyllash</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Largësi teksti nga kreu i kornizës</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Largësi teksti nga fundi i kornizës</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Largësi teksti nga e majta e kornizës</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Largësi teksti nga e djathta e kornizës</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Përpunoni rregullimet tab për kornizë tekstesh...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Lejoje pamjen të ketë tjetër madhësi në kornizë</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Shmangje horizontale e pamjes brenda kornizës</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Shmangje vertikale e pamjes brenda kornizës</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Ripërmaso pamjen jorizontalisht</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Ripërmaso pamjen vertikalisht</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Mbaj ndryshim të X dhe Y njësoj</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Ruaj përpjestim</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Bëje pamjen të hyjë brenda madhësisë së kornizës</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Përdor përpjestimet e pamjes, më mirë se ata të kornizës</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Profil burim i pamjes</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation type="unfinished">Të dhëna titujsh për pamje ISO9660 </translation>
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
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Gjerësi shtylle</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Emri &quot;%1&quot; s&apos;është unik.(new line)
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Shkrim nga e Majt në të Djathtë</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation>Gj&amp;urmim:</translation>
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
        <source>Manual Tracking</source>
        <translation>Gjurmim Dorazi</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Përpunoni Stil</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Majë e Sheshtë</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Majë Katrore</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Majë e Rrumbullakët</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation type="unfinished">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation type="unfinished">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation type="unfinished">Hyni në Kanal...</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Gjerësi Vije:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Vijë e Plotë</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Vijë e Ndërprerë</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Vijë me Pika</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Vijë Pikë Vijë</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Vijë Vijë Pikë Pikë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Emri &quot;%1&quot; s&apos;është unik.(new line)
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>MusterPages</name>
    <message>
        <source>Edit Templates</source>
        <translation>Përpunoni Stampa</translation>
    </message>
    <message>
        <source>Duplicates the selected master page</source>
        <translation>Dyfishon faqen kryesore të përzgjedhur</translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation>Fshin faqen kryesore të përzgjedhur</translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation>Shton një faqe të re kryesore</translation>
    </message>
    <message>
        <source>Loads master page(s) from another document</source>
        <translation>Ngarkon faqe kryesore prej tjetër dokumenti</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation>Doni vërtet të fshini këtë Stampë?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Po</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Emër:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Stampë e Re</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopje e %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Emër:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopjim i #%1 nga</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Përpunoni Stampa</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;I ri</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Dyfisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want do delete this Template?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Stampë?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Po</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Emër:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Stampë e Re</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopje e %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Emër:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Kopjo #%1 nga</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normale</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Stampë?</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Dokument i Ri</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Madhësi Faqeje</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Madhësi:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Letër</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloid</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Vetjake</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntim:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Së gjeri</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Faqe Majtas Fillimisht</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Djathtas:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Poshtë:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Mundësi</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Numër i Faqes së &amp;Parë:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Njësi &amp;Parazgjedhje:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Pikë (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetra (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Inç (inç)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pikas (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Korniza Teksti &amp;Vetvetiu</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Udhëzuesa Shtylle</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Boshllëk:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Madhësi faqeje dokumenti, si madhësi standard ose si madhësi vetjake</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Drejtim i faqeve të dokumentit</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Gjerësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Lartësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.(new line)
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.(new line)
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Numër për faqen e parë të dokumentit</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Njësi matëse parazgjedhje për përpunime dokumentesh</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Krijo vetvetiu korniza teksti kur shtohen faqe të reja</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Numër shtyllash për t&apos;u krijuar në korniza teksti të krijuara vetvetiu</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Largësi ndërmjet shtyllash të krijuara vetvetiu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Brenda:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>&amp;Jashtë:</translation>
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
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation>Faqe Majtas</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Faqe Djathtas</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nyje</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Koordinata &amp;Absolute</translation>
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
        <translation>Përpunoni  Vijë &amp;Rrethine</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Rirregulloni Vijë Rrethine</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Përfundo Përpunim</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Zhvendos Nyje</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Zhvendos Pika Kontrolli</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Shto Nyje</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Fshi Nyje</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Lëviz Pika Kontrolli në Mënyrë të Pavarur</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Lëviz Pika Kontrolli Simetrikisht</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Rirregulloni Pika Kontrolli</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Rirregulloni këtë Pikë Kontrolli</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Hap një Shumëkëndësh ose Pret një Kurbë Bezier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Mbyll këtë Kurbë Bezier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Pasqyro Shtegun Horizontalisht</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Pasqyro Shtegun Vertikalisht</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Qeth Shtegun Horizontalisht Djathtas</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Qeth Shtegun Horizontalisht Majtas</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Qeth Shtegun Vertikalisht Sipër</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Qeth Shtegun Vertikalisht Poshtë</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rrotullo Shtegun në Kah Antiorar</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rrotullo Shtegun në Kah Orar </translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Zvogëloni Madhësinë e Shtegut me % e treguar</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Zmadhoni Madhësinë e Shtegut me % e treguar</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Kënd Rrotullimi</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation></translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Aktivizo Mënyrë Përpunimi Vije Rrethine</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Rikthe Vijë Rrethine te Forma Origjinale e Kornizës</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Kur zgjidhet përdoren Koordinata relative te Faqja,(new line)
përndryshe Koordinatat janë relative te Objekti.</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Mundësi Importuesi për OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation>Përditëso stile paragrafësh</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation>Nëse një stil paragrafi me të njëjtin numër si paragrafi i dokumentit
OpenOffice.org ekziston tashmë, stili në Scribus do të duhej të përpunohej 
për përputhje me atë të importuar, ose të lihet i paprekur</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation>Paketo stile pargrafësh</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation>Grupo stile paragrafesh sipas atributesh.
Më pak stile paragrafesh por kontrolli i tyre mund të jetë i vështirë.
Do të duhej të përdorej nëse dihet që teksti nuk duhet përpunuar
pas importimi.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Përdor emra dokumentesh si parashtesë për stile paragrafësh </translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation>Nëse do të duhej që importuesi të shtonte emrin e dokumentit
në ballë të emrit të stilit të paragrafit në Scribus</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Mos pyet sërish</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation>Nëse do të duhej që importuesi të përdorë përherë vlerën
e vendosur për çastin kur importon dokument OpenOffice.org dhe
të mos kërkojë më ripohimin tuaj sërish</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Konsol Programthesh</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Kroji Kartelë PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Për&amp;fundimi në Kartelë:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Ndryshoni...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Eksporto Interval</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Tërë Faqet</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">&amp;Zgjidhni Faqe</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Mundësi Kartele</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">Përp&amp;uthshmëri:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Mënjanë Majtas</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Mënjanë Djathtas</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Prodho &amp;Figurëza</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Ruaj Korniza të &amp;Lidhura Tekstesh si Artikuj PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">&amp;Përfshi Faqerojtësa</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete"> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Qartësi:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation type="obsolete">N&amp;gjesh Tekst dhe Grafikë Vektorialë</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Rregullime Pamjeje</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Automatike</translation>
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
        <translation type="obsolete">Asnjë</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Metodë:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Cilësi:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Më i madhi</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Lartë</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Mesatare</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Ulët</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Më i vogli</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">&amp;thjeshto Pamje në:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">&amp;Të përgjithshme</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Trupëzo tërë Gërmat</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Po trupëzoj</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Gërma të Mundshme:</translation>
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
        <translation type="obsolete">Gërma për trupëzim:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Gërma</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="obsolete">Aktivizo Efekte P&amp;araqitjeje</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Faqe</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">Shfaq P&amp;araparje Faqesh</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Efekte</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">&amp;Shfaq Kohëzgjatje:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Kohëzgjatje Efek&amp;ti:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">T&amp;ip Efekti:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">Po zh&amp;vendos Rreshta:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">P&amp;rej:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">Drejt&amp;im:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete">sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Pa Efekt</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Kuti</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Ndaj</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horizontalisht</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikalisht</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Brenda</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Jashtë</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Nga e Majta në të Djathtë</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Nga Sipër Poshtë</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Nga Poshtë Sipër</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Nga e Djathta në të Majtë</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Nga Sipër-majtas Poshtë-djathtas</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">&amp;Zbato Efekt te tërë Faqet</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">&amp;Përdor Kodim</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Fjalëkalime</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Përdorues:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">Pr&amp;onar:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Rregullime</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Lejo &amp;Shtypjen e Dokumentit</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Lejo &amp;Ndryshimin e Dokumentit</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Lejo Kop&amp;jim Tekstesh dhe Grafikash</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Lejo Shtim Shëni&amp;mesh dhe Fushash</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">S&amp;iguri</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Të përgjithshme</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">Përfundim i Para&amp;menduar Për:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Ekran / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Shtypës</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">Për&amp;dor Rregullime Vetjake Vizatimi</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Rregullime Vizatimi</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">Shpe&amp;shti:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Kënd:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Pikë e Thjeshtë</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Vijë</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Rreth</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Elips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Ngjyra të Plota:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Përdor Profil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Profil:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Kolorimetrik Relativ</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Kolorimetrik Absolut</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Pamje:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Mos përdor profile ICC të trupëzuar</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">Ngj&amp;yrë</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">Ruajtje përfundimesh e suksesshme</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">Të &amp;dhëna Vargu:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Profil Për&amp;fundimesh:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Listë Kuti</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Eksporto tërë faqet si PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Eksporto interval faqesh si PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Jepni një listë objektesh të ndarë me presje ku(new line)
objekti mund të jetë * për tërë faqet, 1-5 për(new line)
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Përcakton përputhshmërinë me. Parazgjedhja është Acrobat 4.0 çka ju jep përputhshmërinë më të madhe.(new line)
Zgjidhni Acrobat 5.0 nese kartela juaj ka karakteristika PDF 1.4 si p.sh. tejdukshmëri ose nëse keni nevojë për kodim 128 bit.(new line)
PDF/X-3 vlen për eksportim PDF-sh për shtypje komerciale dhe është i përzgjedhshëm kur keni veprues administrim ngjyrash.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Përcakton lidhjen e faqeve te PDF-ja. Lini parazgjedhjen - Majtas,(new line)
veç në e ditshi që keni nevojë ta ndryshoni.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Prodhon figurëza për çdo faqe të PDF-së.(new line)
Disa parësa mund të përdorin figurëza për lundrim.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Prodho Artikuj PDF, çka është e dobishme për lundrim artikujsh në një PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Trupëzo në dokumetin tuaj faqerojtësit e krijuar.(new line)
Këta janë të dobishëm për të udhëtuar në dokumente të gjatë PDF.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Eksporto qartësi teksti dhe grafikësh vektorialë.(new line)
Kjo nuk prek qartësinë e pamjeve bitmap, si fotot p.sh.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Ngjeshje teksti dhe grafike.(new line)
Nëse nuk keni arsye, lëreni të shenjuar. Kjo zvogëlon madhësinë PDF.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Version ngjeshjeje për pamje.(new line)
Automatikja i lejon Scribus-it të zgjedhë metodën më të mirë.(new line)
ZIP është i mirë për pamje me ngjyra të plota.(new line)
JPEG është më i mirë pëër krijim kartelash PDF më të vogla të cilat mund të kenë plot foto (me pakëz humbje në pamje, si mundësi).(new line)
Lëreni të caktuar te automatike, veç në paçi nevojë për mundësira speciale ngjeshjeje.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Shkallë ngjeshjeje: Më e ulta (25%), E ulët (50%), E mesme (75%), E lartë (85%), Më e larta (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Thjeshtoni pamjet tuaja bitmap deri te vlera DPI e përzgjedhur.(new line)
Po e latë të pashenjuar dotë vizatohen me qartësinë origjinale.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">DPI (Pika për Inç) për eksport pamjesh.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Trupëzo gërma te PDF-ja. Trupëzimi i gërmave(new line)
do të ruajë skemën dhe dukjen e dokumentit tuaj.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Mundëson efekte paraqitjeje kur Acrobat Reader-i përdoret në mënyrën tërë ekrani. </translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Shfaq paraparje faqeje për secilën nga faqet e listuara më sipër. </translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Koha gjatë së cilës tregohet faqja para se të nisë paraqitja në faqen e përzgjedhur.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Kohëzgjatja e xhirimit të efektit.(new line)
Një kohë më e shkurtër do ta përshpejtojë efektin, një më e gjatë do ta ngadalësojë.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Tip i efektit në ekran.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Zbato efektin e përzgjedhur te të tëra faqet.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Aktivizoni veçori sigurie për PDF tuajat të eksportuara.(new line)
Nëse përzgjodhët Acrobat 4.0, PDF-ja do të mbrohet me kodim 40 bitësh.(new line)
Nëse përzgjodhët Acrobat 5.0, PDF-ja do të mbrohet me kodim 128 bitësh.(new line)
Kujdes: Kodimi PDF nuk është aq i besueshëm sa kodimi GPG ose PGP dhe ka ca kufizime.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Zgjidhni fjalëkalim të përgjithshëm i cili aktivizon ose çaktivizon tërë(new line)
veçoritë e sigurisë për PDF tuajat të eksportuara</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Zgjidhni një fjalëkalim i cili t&apos;u lejojë përdoruesve të lexojnë PDF-të tuaja.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Lejo shtypjen e PDF-së. Nëse nuk shenjohet, parandalohet shtypja.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Lejo ndryshimin e PDF-së. Nëse nuk shenjohet, parandalohet ndryshimi i PDF-së.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Lejo kopjim teksti apo grafike prej PDF-së. (new line)
Nëse nuk shenjohet, nuk mund të kopjohet tekst apo grafikë.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Lejo shtim shënimesh dhe fushash te PDF-ja. (new line)
Nëse nuk shenjohet, parandalohet përpunimi i shënimeve dhe fushave.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Model ngjyre për përfundimin e PDF-së tuaj.(new line)
Zgjidhni Ekran/Web për PDF që përdoren për shfaqje në ekran dhe për shtypje në shtypësa inkjet të zakonshëm.(new line)
Zgjidhni Shtypës kur shtypet vërtet me një shtypës 4 ngjyrësh CMYK.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Ky është një rregullim i cili si parazgjedhje nuk është veprues. Do të duhej të aktivizohej(new line)
kur kërkohet veçanërisht nga shtypësi juaj dhe ju kanë dhënë saktë hollësitë e nevojshme.(new line)
Përndryshe, PDF-ja që eksportuat mund të mos shtypet si duhet dhe as të jetë vërtet i kalueshëm nëpër sisteme.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Trupëzo një profil ngjyre për ngjyra të plota</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Profil ngjyrash për ngjyra të plota</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Trupëzo profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Mos përdor profile ngjyrash që genden të trupëzuar në burimin e pamjes</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Profil përfundimesh për shtypjen. Nëse mundet, merrni ca udhëzime prej shtypësit tuaj rreth përzgjedhjes së profilit.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Vargu i detyrueshëm për PDF/X-3 ose PDF do të dalë jo në(new line)
përputhje me PDF/X-3. Ju këshillojmë të përdorni titullin e dokumentit.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Ruaj  si</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Kartela PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Ngjesh Tekst dhe Grafikë &amp;Vektorialë</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">&amp;Mundëso Efekte Paraqitjeje</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Paraqitje</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Pasqyro Faqe(t) horizontalisht</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Pasqyro Faqe(t) vertikalisht</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation>Ruaje si PDF</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Paraparje Shtypjeje</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Anti-alias &amp;Tekst</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Anti-alias &amp;Grafikë</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Shfaq Tej&amp;dukshmëri</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>Heqje Ngjyre &amp;Nën Të</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Shfaq CMYK</translation>
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
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Ofron në parës një pamje më të pëlqyeshme për pjesë teksti, në kurriz të(new line)
një ngadalësimi të pakët të pararjeve. Kjo prek vetëm gërma Type 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Ofron në paraparje një pamje më të pëlqyeshme për True Type Fonts, Open Type Fonts,(new line)
EPS, PDF dhe grafikë vektoriale, në kurriz të një ngadalësimi të pakët të pararjeve</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Shfaq tejdukshmëri dhe objekte të tejdukshëm në dokumentin tuaj. Lyp Ghostscript 7.07 ose të mëvonshëm</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Jep një paraparje shtypjeje duke përdorur një simulim bojërash të zakonshme CMYK, në vend të ngjyrave RGB</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Një rrugë braktisjeje e disa shkallëve të grisë të përbëra nga(new line)
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.(new line)
UCR prek më tepër pjesë pamjeje  që janë tone neutrale dhe/ose të errët(new line)
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje(new line)
dhe lipsen ca eksperimente dhe prova për çdo herë.(new line)
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tërë</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Ofron në paraparje një pamje më të pëlqyeshme për Gërma TrueType, Gërma OpenType, EPS, dhe(new line)
grafikë vektorialë, në kurriz të një ngadalësimi të pakët të pararjeve</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Një rrugë braktisjeje e disa shkallëve të grisë që janë të përbëra nga(new line)
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.(new line)
UCR prek më tepër pjesë pamjeje që janë tone neutralë dhe/ose të errët(new line)
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje(new line)
dhe lipsen ca eksperimente dhe prova për çdo herë.(new line)
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="unfinished">U zbuluan ca Gabime.
Shqyrtoni përdorimin e Preflight Checker për ndreqjen e tyre</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation>Ndërpre</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation>Shpërfill</translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Kopjo Këtu</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Zhvendos Këtu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Ngjit</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Shfaq &amp;Mënjana</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Shfaq &amp;Korniza</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Shfaq &amp;Pamje</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Shfaq &amp;Udhëzuesa</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Pamje</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Kartelë:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">PPI Fillestar: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">PPI i Tanishëm: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Tekst i Lidhur</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Kornizë Teksti</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Tekst sipas një Shtegu</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Paragrafë:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Fjalë:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Gërma:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Shtyp:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Veprues</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Çaktivizuar</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">Të dhë&amp;na</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">&amp;Merr Pamje...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">Pa&amp;mje e Dukshme</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">&amp;Përditëso Pamje</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Përpunoni Pamje</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">&amp;Përshtatja Kornizën Pamjes</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Merr Tekst...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">&amp;Vini Tekst...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Përpunoni Tekst...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">Është &amp;Faqerojtës PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">Është Sh&amp;ënim PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">V&amp;eti Shënimesh</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">&amp;Veti Fushe</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Mundësi &amp;PDF</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Përpunoni Tekst...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Blloko</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Çblloko </translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Kyç &amp;Madhësi Objekti</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Çkyç &amp;Madhësi Objekti</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Dërgoje në të Pa&amp;pastër</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Dërgo te Sh&amp;tresë</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">&amp;Fut Shembull Teksti</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Grupo</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Ç&amp;grupo</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">Ni&amp;vel</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Shpjere &amp;Prapa</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Bjere &amp;Përpara</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Ul</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Ngri</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Kornizë &amp;Pamjeje</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Shumë&amp;këndësh</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Jashtëvija</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">Kornizë &amp;Teksti</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">Kurbë &amp;Bezier</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">Shn&amp;dërro si </translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">P&amp;ri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Fshi</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Pa&amp;stro Përmbajtje</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Shfaq &amp;Veti...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Fshih &amp;Veti...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Asnjë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Doni vërtet të pastroni tërë Tekstin tuaj?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Programi</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">mungon!</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopje e</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Pamje</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Vijë</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Shumëkëndësh</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Shumëvijë</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Tekst Shtegu</translation>
    </message>
    <message>
        <source>Do you really want to replace all your text
in the frame named %1 with sample text?</source>
        <translation>Doni vërtet të zëvendësohet tërë teksti juaj
te korniza me emër %1 me tekstin shembull?</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Lidhet Me</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Është Prind I</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Është Pjellë E</translation>
    </message>
    <message>
        <source>Page Item Attributes</source>
        <translation>Veti Objekti Faqeje</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tip</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Vlerë</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parametër</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Marrëdhënie</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Marrëdhënie Me</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Shto</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Pa&amp;stro</translation>
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
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Faqe</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation> e %1</translation>
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
        <translation>Përfundim</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Përfundime Kërkimi për:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Paraparje</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Përzgjidhni</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation>Grafikë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Shteg</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Faqe</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Shtyp</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Gjendje</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Shko te</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Po</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Mungon</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Kërko</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Can&apos;t find Plug-in</source>
        <translation>S&apos;mund të gjej Shtojcë</translation>
    </message>
    <message>
        <source>Persistent</source>
        <comment>plugin manager</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation>Importo</translation>
    </message>
    <message>
        <source>Standard</source>
        <comment>plugin manager</comment>
        <translation>Standard</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation>E panjohur</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Veti Shumëkëndëshi</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">&amp;Kulme:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Conve&amp;x Polygon</source>
        <translation type="obsolete">Mysë&amp;to Shumëkëndësh</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Numër kulmesh për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Gradë rrotullimi për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Polygons will be convex rather than concave</source>
        <translation type="obsolete">Shumëkëndëshat do të jenë të mysët më tepër se të lugët</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Shembull Shumëkëndëshi</translation>
    </message>
    <message>
        <source>Change the angles at which lines of the polygon join</source>
        <translation type="obsolete">Ndryshoni këndet nën të cilë vijat e shumëkëndëshit takohen</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Zbato &amp;Faktor</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Zbato Faktor I lugët/I mysët për të ndryshuar formën e Shumëkëndëshave</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Një vlerë negative do ta bëjë shumëkëndëshin të lugët (ose si yll),
një vlerë pozitive do ta bëjë at të mysët</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>K&amp;ulme:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Zbato &amp;Faktor</translation>
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
        <translation>Numër kulmesh për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Gradë rrotullimi për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Zbato Faktor I lugët/I mysët për të ndryshuar formën e Shumëkëndëshave</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Shembull Shumëkëndëshi</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Një vlerë negative do ta bëjë shumëkëndëshin të lugët (ose si yll),
një vlerë pozitive do ta bëjë atë të mysët</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Parapëlqime</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Të përgjithshme</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Udhëzuesa</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Mjete</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>E papastër</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Shfaq</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Mjete të Jashtëm</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Të ndrysh.</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Temë:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Madhësi Gërmash:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Njësi</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Pikë (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milimetra (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Inç (inç)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Pikas (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Rregullime Miu</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Kërcim &amp;Miu:</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete">px</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">&amp;Merr Rreze:</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Menu</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>Dokumente së &amp;Fundmi:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Shtigje</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Ndrysho...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>Profile &amp;ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Programthe:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>N&amp;drysho...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vetjake</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Madhësi:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Së gjeri</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntim:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Faqe Majtas fillimisht</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Djathtas:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Majtas:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Vetëruaj</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>&amp;Veprues</translation>
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
        <source>Grid Layout</source>
        <translation type="obsolete">Skemë Rrjete</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Hapje për Rrjetën e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Hapje për Rrjetën e &amp;Madhe:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Largësi ndërmjet shtyllash</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Ngjyra Rrjete</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">Ngjyrë për Rrjetën e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">Ngjyrë për Rrjetën e &amp;Madhe:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Ngjyrë Udhëzuesash &amp;Përdoruesi:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Vendosje</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">Në &amp;Sfond</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">Në &amp;Ballë</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">O&amp;n</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">O&amp;ff</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Poshtëshkrim</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Zhvendosje:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Ripërmasim:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Sipërshkrim</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Zh&amp;vendosje:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">Ri&amp;përmasim:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Ripërm&amp;asim:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Tjetër</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Degëzim Vertical</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Ndër&amp;vijë Automatike:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">&amp;Gërma Parazgjedhje:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">&amp;Madhësi Parazgjedhje:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">Ngjyrë &amp;Teksti:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">&amp;Boshllëk:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Asnjë</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Ngjyrë &amp;Vije:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Hije:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">Ngjyrë &amp;Mbushjeje:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">H&amp;ije:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">&amp;Tip Vije:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">&amp;Gjerësi Vije:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Hapje:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">Ripërmasim i &amp;Lirë</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">Ripërmasim &amp;Horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">Rpërmasim &amp;Vertical:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Ruaj &amp;Përpjestim</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Ngjyrë M&amp;bushje:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">K&amp;ulme:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Conve&amp;x Polygon</source>
        <translation type="obsolete">Mysë&amp;to Shumëkëndësh</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Mundësi të Tjera</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>&amp;Ruaj Përmbajtje gjatë Ndryshimesh</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Paraparje</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>E vogël</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mesatare</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>E madhe</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Shfaq Faqet &amp;Krah Njëra Tjetrës</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Ngjyra Faqeje</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Sfond:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Mënjana:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Paraqit Zonë të &amp;Pashtypshme në Ngjyrë Mënjanash</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Përdor Karakteristika &amp;Tejdukshmërie PDF 1.4 </translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Ujdis Madhësi Paraqitjeje</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Për të përshtatur ekranin tërhiqni vizoren nën Rrëshqitësin.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Interpretues PostScript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Emër i të Ekzekutueshmit:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Antialias &amp;Tekstesh</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Antialias &amp;Grafikash</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Mjet Përpunimi Pamjesh</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Emër i të &amp;Ekzekutueshmit:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Po shtyp</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Qeth sa &amp;Mënjanat e Faqes</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Zbato Heqje Ngjyre &amp;Nën Të</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Zgjidhni pamjet dhe zbukurim dritaresh parazgjedhje.(new line)
Scribus-i bën të vetën cilëndo temë të mundshme KDE apo Q</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Madhësi parazgjedhje gërmash për menu dhe dritare</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Njësi matëse parazgjedhje për përpunime dokumentesh</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Numër rreshtas të cilët do të kapërcejë Scribus-i për çdo lëvizje të rrotës së miut</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Rrezja e zonës ku Scribus-i do t&apos;ju lejojë të mbërtheni veshët e një objekti</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Numër dokumentesh përpunuar së fundmi për t&apos;u shfaqur te menuja Kartelë </translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Drejtori dokumentesh parazgjedhje</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Drejtori profilesh ICC parazgjedhje</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Drejtori programthesh Scripter parazgjedhje</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Madhësi parazgjedhje e faqes, ose si madhësi standard, ose si madhësi vetjake</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Orientim parazgjedhje faqesh dokumenti</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Gjerësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Lartësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.(new line)
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.(new line)
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Nëse vepruese, Scribus ruan një kopje kopjeruajtjeje të kartelës suaj me zgjatimin .bak(new line)
çdo herë që mbaron periudha kohore</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Periudhë kohore ndërmjet ruajtjesh të vetvetishme</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Hapësirë midis vijash rrjete të vogël</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Hapësirë midis vijash rrjete të madhevogël</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Ngjyrë për vija rrjete të vogël</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Ngjyrë për vija rrjete të madhe</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Ngjyrë e vijave udhëzuese që fusni</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Vendos rrjetën pas objekteve të faqes</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Vendos rrjetën para objekteve të faqes</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Kopjo fluturimthi</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">/Mënyrë Vizualizimi/Pa</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Madhësi relative e mbishkrimit krahasuar me gërmat normale</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Madhësi relative e poshtëshkrimit krahasuar me gërmat normale</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Madhësi relative e gërmave të vogla krahasuar me gërmat normale</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Veti Kornize Teksti</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Veti Kornize Pamjeje</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Veti Vizatimi Formash</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Parazgjedhje Shkalle Zmadhimi</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Veti Vizatimi Vijash</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Veti Vizatimi Shumëkëndëshash</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Gërma për korniza të reja tekstesh</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Madhësi gërmash për korniza të reja teksti</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Ngjyrë gërmash</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Numër shtyllash në një kornizë teksti</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Hapësirë midis shtyllash kornize teksti</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Shembull i gërmave tuaja</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Korniza pamjesh lejojnë pamjet të ripërmasohen në çfarëdo madhësi</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Ripërmasim horizontal i pamjeve</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Ripërmasim vertikal i pamjeve</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Mbaj ripërmasim horizontal dhe vertikal njësoj</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Pamjet e ripërmasuara vetvetiu ruajnë përpjestimet e veta fillestare</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Ngjyrë mbushjeje për korniza pamjesh</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Ngopje e ngjyrës së mbushjes</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Ngjyrë vije formash</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Ngopje për ngjyrë vijash</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Ngjyrë mbushjeje për forma</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Stil vije formash</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Gjerësi vije formash</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Zmadhimi më i vogël i lejuar</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Zmadhimi më i madh i lejuar</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Ndryshimi në zmadhim për çdo veprim &quot;zoom&quot;</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Ngjyrë vijash</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Ngopje ngjyre</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Stil vijash</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Gjerësi vijash</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Numër kulmesh për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Gradë rrotullimi për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Polygons will be convex rather than concave</source>
        <translation type="obsolete">Shumëkëndëshat do të jenë të mysët më tepër se të lugët</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Shembull Shumëkëndëshi</translation>
    </message>
    <message>
        <source>Change the angles at which lines of the polygon join</source>
        <translation type="obsolete">Ndryshoni këndet nën të cilët vijat e shumëkëndëshit takohen</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Zgjidhni madhësinë e paraparjes te paleta e të papastrave</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Ruaj përmbajtjen e të papastrës çdo herë pas një ndryshimi</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Kur përdoren faqe përkarshi, shfaqi dy faqet krah njëra tjetrëa</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Ngjyrë për letrën</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Masko zonën jashtë mënjanash me ngjyrën e mënjanave</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Aktivizo karakteristika tejdukshmërie brenda eksportimi PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Cakto shkallë zmadhimi parazgjedhje</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Vend sistemi kartelash për interpretuesin Ghostscript</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Antialias tekstesh për vizatime EPS dhe PDF në ekran</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Antialias grafikash për vizatime EPS dhe PDF në ekran</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Vend sistemi kartelash për përpunues grafikash</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Mos shfaq objekte jashtë mënjanave te faqe të shtypura ose kartela të eksportuara</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Zgjidhni një Drejtori</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Brenda:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>J&amp;ashtë:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>&amp;Stampa:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>&amp;Ndryshoni...</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Ekzekutive</translation>
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
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Zbato &amp;Faktor</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Drejtori Shtesë për Stampa Dokumentesh</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Zbato Faktor I lugët/I mysët për të ndryshuar formën e Shumëkëndëshave</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Një vlerë negative do ta bëjë shumëkëndëshin të lugët (ose si yll),
një vlerë pozitive do ta bëjë at të mysët</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Një rrugë për të hequr qafe disa shkallë të grisë të përbëra prej cyan, 
yellow dhe magenta dhe që në fakt përdorin të zezën. UCR prek më shumë 
pjesët e pamjeve të cilat janë asnjanëse dhe/ose tone të errët që janë
afër grisë. Përdorimi i kësaj mund të përmirësojë shtypjen e disa pamjeve
dhe lyp ca eksperimente dhe prova hap pas hapi. UCR zvogëlon mundësi
tejngopjeje me bojëra CMY.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Gjuhë:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Njësi:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Zhbëj/Ribëj</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Gjatësi historik veprimesh</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Ndarës</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Gërma</translation>
    </message>
    <message>
        <source>Document Checker</source>
        <translation>Kontrollues Dokumenti</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Administrim Ngjyrash</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Eksport PDF</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Veti Objekti Dokumenti</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabelë Përmbajtjeje dhe Treguesa</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Shkurtprerje Tastiere</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Paraqitje Faqeje</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Ngjyrë:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Shfaq Foto</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Shfaq Zinxhir Tekstesh</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Shfaq Korniza</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Always ask before Fonts are replaced when loading a Document</source>
        <translation>Pyet përherë para se Gërmat të zëvendësohen gjatë ngarkmit të një Dokumenti</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Të ndryshme</translation>
    </message>
    <message>
        <source>Plugin Manager</source>
        <translation>Përgjegjës Shtojcash</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>Shtojca</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Si të xhirojë</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tip</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Ta ngarkoj?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>ID Shtojce</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Kartelë</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Po</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Jo</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Duhet të rinisni zbatimin për t&apos;u zbatuar ndryshimet.</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>Shtojca</translation>
    </message>
    <message>
        <source>Turns the of linked frames on or off</source>
        <translation>Aktivizon ose jo paraqitjen e kornizave të lidhura</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Aktivizon ose jo paraqitjen e kornizave</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Aktivizon ose jo paraqitjen e fotove</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with.
Leave this blank to choose based on environment variables.
You can still override this by passing a command line option when starting Scribus</source>
        <translation>Përzgjidhni gjuhën tuaj parazgjedhje në të cilën të xhirojë Scribus-i.
Lëreni bosh për zgjedhje mbi bazë ndryshoresh mjedisi.
Mundeni ende ta anashkaloni këtë duke përdor rresht urdhrash gjatë nisjes së Scribus-it</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation>Drejtori parazgjedhje profilesh ICC. Kjo nuk
mund të ndryshohet me një dokument të hapur.</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation>Cakton madhësinë në hapa të historikut të veprimeve.
Në u vëntë 0, mund të ruhet një numër i pafund veprimesh.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation>Vend sistemi kartelash për përpunuesët grafikë. Nëse përdorni
Gimp-in dhe shpërndarja juaj e përmban atë, këshillojmë &apos;gimp-remote&apos;,
meqë ju lejon të përpunoni pamjen në një instancë Gimp-i në xhirim e sipër.</translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Parazgjedhje</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>E &amp;kuqe:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>E &amp;gjelbër:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bl&amp;u:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Kanal &amp;alfa:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Ngjyra &amp;bazë</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Ngjyra &amp;vetjake</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Përcaktoni Ngjyra Vetjake &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Shto tek Ngjyra Vetjake</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Përzgjidhni ngjyrë</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Kopjo ose Zhvendos Kartelë</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Lexo: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Shkruaj: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Emër kartele:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Tip kartele:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Një drejtori sipër</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Madhësi</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tip</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datë</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atribute</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Shih &amp;në:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Prapa</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Krijo Dosje të Re</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Parje si Listë</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Parje në Hollësi</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Paraparje Të dhënash Kartele</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Paraparje Përmbajtjeje Kartele</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lexim-shkrim</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Vetëm lexim</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Vetëm shkrim</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>I pakapshëm</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Simlidhje te Kartelë</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Simlidhje te Drejtori</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Simlidhje te Special</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Kartelë</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Dre</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Special</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Ruaj Si</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Hap</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Riemërto</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshij</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>R&amp;ingarko</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Rendit sipas &amp;Emrit</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Rendit sipas &amp;Madhësisë</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Rendit sipas &amp;Datës</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>E &amp;parenditur</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Rendit</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Shfaq kartela të &amp;fshehura</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>kartela</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>drejtoria</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>simlidhja</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Fshij %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Jeni të sigurtë se doni të fshihen %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Po</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Jo</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Dosje e Re 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Dosje e Re</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Dosje e Re %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Gjej Drejtori</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Drejtorira</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Ruaj</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Gabim</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1(new line)
Kartelë që s&apos;gjendet.(new line)
Kontrolloni shteg dhe emër kartele.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Tërë Kartelat (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Përzgjidhni Drejtori</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Drejtori:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Gërma</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Stil gërmash</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Madhësi</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efekte</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>&amp;Hequrvije</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Nënvijë</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>Ngj&amp;yrë</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Shembull</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>&amp;Programth</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Zbato</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Mbyll</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Përzgjidhni Gërma</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Pastro</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Përzgjidhni Tërë</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ribëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Pri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Ngjit</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation type="unfinished">Ngjitu</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Përshtat...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Rreth Qt-së&lt;/h3&gt;&lt;p&gt;Ky program përdor versionin %1 të Qt-së.&lt;/p&gt;&lt;p&gt;Qt është një toolkit C++ për zhvillim GUI &amp;amp; zbatime shumëplatformëshe.&lt;/p&gt;&lt;p&gt;Qt-ja ofron kalueshmëri prej një burimi të vetëm në MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, dhe tërë variantet e njohur komercialë Unix.&lt;br&gt;Qt është gjithaq i mundshëm për pajisje të trupëzuara.&lt;/p&gt;&lt;p&gt;Qt është një produkt Trolltech. Për më tepër të dhëna shihni &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation>Po importoj tekst</translation>
    </message>
    <message>
        <source>Importer</source>
        <translation type="obsolete">Importues</translation>
    </message>
    <message>
        <source>Choose the importer to use</source>
        <translation type="obsolete">Zgjidhni importuar për t&apos;u përdorur</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Tërë Formatet e Mbuluar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Po gatis...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Sfond</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Doni vërtet të mbishkruhet Kartela:(new line)
%1 ?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Paraparje Gërmash</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Fut Special</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Të ri &amp;prej Stampe...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;I ri</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Lajme</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Broshura</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Katalogë</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Trakte</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Tabela</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Kartolina</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Krye Letrash</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Zarfa</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Karta Biznesi</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalendarë</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Reklama</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiketa</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menu</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programe</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Formularë PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Paraqitje PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Revista</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Postera</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Lajmërime</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Dokumente Tekst</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Fletëpalosje</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Stampa Vetjake</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Ruaj si &amp;Pamje...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Ruaj si Pamje</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Gabim në shkrim kartele(ash) përfundim.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Gabim në shkrim kartele(ash) përfundim.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Eksportim i suksesshëm.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Ka një kartelë të tillë. Ta mbishkruaj? </translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>ka një të tillë. Ta mbishkruaj? </translation>
    </message>
    <message>
        <source>No</source>
        <translation>Jo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Po</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Po, tërë</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Paraparje Shtypjeje</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Shtyp...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Para&amp;parje Shtypjeje</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importo &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Tërë Formatet e Mbuluar (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Ruaj si &amp;Stampë...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Oook! Po provoni të ngarkoni një pamje në një objekt që nuk ekziston ose që nuk është përzgjedhur!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Oook! Po provoni të (ç)kyçni një objekt që nuk ekziston! E as është përzgjedhur ndonjë.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Oook! Po provoni të kërkoni rreth një objekti që nuk ekziston! E as është përzgjedhur ndonjë.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Oook! Po thërrisni një objekt që nuk ekziston!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Oook! Po provoni të fshini një objekt që nuk ekziston!</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>&amp;Doracak për Scribus-in...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>Programthe &amp;Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Ekzekuto Programth...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Programthe së &amp;Fundmi</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Shfaq &amp;Konsol</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">P&amp;rogramth</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referencë Online</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Programthe Python (*.py);; Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Ruaj Faqe si &amp;SVG...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Ruaj  si</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Pamje SVG (*.svg *.svgz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Pamje SVG (*.svg);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importo &amp;SVG...</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaanse</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Shqip</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baske</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bullgare</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Braziliane</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalane</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Kineze</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Çeke</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Daneze</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandeze</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Anglishte</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Anglishte (Britanike)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>GjermaneGjermane</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandeze</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Frënge</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galike</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Greke</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hungareze</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indoneziane</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italisht</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreane</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituaneze</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norvegjeze (Bookmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norvegjeze (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norvegjeze</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polake</translation>
    </message>
    <message>
        <source>Portugese (Brazilian)</source>
        <translation type="obsolete">Portugeze (Braziliane)</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruse</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Suedeze</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spanjolle</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Spanjolle (Latine)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovake</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovene</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbe</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turke</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrainase</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Uellsiane</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Gërmat %1 janë të dëmtuara, po i hedh tej</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation>Stampë:</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem ngjyrë me një emër bosh.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Ngjyrë që s&apos;gjendet</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të ndryshoj një ngjyrë me një emër bosh.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="obsolete">Ngjyrë që s&apos;gjendet në dokument</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="obsolete">Ngjyrë që s&apos;gjendet në ngjyrat parazgjedhje</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të krijoj një ngjyrë me një emër bosh.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të fshij një ngjyrë me një emër bosh.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të zëvendësoj një ngjyrë me një emër bosh.</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="obsolete">Dështova në hapjen e dokumentit</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="obsolete">Dështova në ruajtjen e dokumentit</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Njësi jashtë intervali. Përdorni një nga konstantet scribus.UNIT_* .</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Ngjyrë që s&apos;gjendet - gabim python</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Objektivi nuk është një kornizë pamje.</translation>
    </message>
    <message>
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të ripërmasoj dot me 0%</translation>
    </message>
    <message>
        <source>Specified item not an image frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekti i treguar jo një kornizë pamje</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Gërma që s&apos;gjenden</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;vizatoj dot një shembull bosh</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;ruaj dot te një kartelë bosh</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të pranoj një emër bosh shtrese</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Shtresë që s&apos;gjenden</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;heq dot shtresën e fundit</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;krijoj dot shtresë pa emër</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="obsolete">Ka tashmë një objekt e emrin e kërkuar</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Lista e pikave duhet të përmbajë të paktën dy pika (katër vlera)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation type="obsolete">Lista e pikave duhet të përmbajë një numër çift vlerash</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Lista e pikave duhet të përmbajë të paktën tri pika (gjashtë vlera)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Lista e pikave duhet të përmbajë të paktën katër pika (tetë vlera)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation type="obsolete">Lista e pikave duhet të jetë një shumëfish gjashtë vlerash</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekt që s&apos;gjendet</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Stil që s&apos;gjendet</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;caktoj dot stil në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="obsolete">Dështova në ruajtjen e EPS</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="obsolete">Numër faqesh jashtë intervali</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation type="obsolete">Gjerësi vije jashtë kufijsh, duhet të jetë 0 &lt;= line_width &lt;= 12</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Hije vije jashtë kufijsh, duhet të jetë 0 &lt;= shade &lt;= 100</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Hije mbushjeje jashtë kufijsh, duhet të jetë 0 &lt;= shade &lt;= 100</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Rrezja e kulmit duhet të jetë një numër pozitiv.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Stil vije që s&apos;gjendet</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem madhësi gërmash kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem gërma kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem madhësi teksti kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të numëroj shtylla në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem hapësirë vije kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem hapësirë shtyllash kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të kem tekst kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;rregulloj dot tekst kornize jo tekst.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të fus tekst në një kornizë jo tekst.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Vendosje jashtë intervali. Përdorni një nga konstantet scribus.ALIGN* .</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;caktoj dot drejtim teksti në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="obsolete">Madhësi gërmash jashtë kufijsh - duhet të jetë 1 &lt;= size &lt;=512</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;caktoj dot madhësi teksti në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;caktoj dot tekst në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="obsolete">Hapësirë vije jashtë kufijsh, duhet të jetë &gt;= 0.1</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të fus ndërvijë në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="obsolete">Hapësirë shtylle jashtë kufijsh, duhet të jetë pozitive</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të vendos hapësirë shtyllash në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Numër shtylle jashtë kufijsh, duhet të jetë &gt;1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të kem numër shtyllash në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Tregues përzgjedhjeje jashtë kufijsh</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;përzgjedh dot tekst në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;fshij dot tekst prej një kornize jo tekst</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;caktoj dot mbushje teksti në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;caktoj dot hije teksti në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Mund të lidh vetëm korniza teksti</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Korniza objektiv duhet të jetë bosh</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Korniza objektiv lidhet te një tjetër kornizë</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Korniza objektiv është lidhur te një tjetër kornizë</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Pikënisja dhe objektivi janë i njëjti objekt</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të heq lidhje në një kornizë jo tekst</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekti nuk është një kornizë tekst e lidhur, s&apos;heq dot  lidhje.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objekti është korniza e fundit në një varg, nuk mund ta zgjidh. Zgjidhni atëherë kornizën e mëparshme.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="obsolete">S&apos;mund të shndërroj një kornizë jo tekst në jashtëvija</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Provova të caktoj përparim &gt; përparimi më i madh</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Emri i kartelës duhet të jetë një varg.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>S&apos;mund të fshij rregullime tipi pamjeje.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Tipi i pamjes duhet të jetë një varg.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>atributi &apos;allTypes&apos; është VETËM-LEXIM</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Dështova në eksportim pamjeje</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Xhiro një programth Python prej një kartele.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation type="obsolete">&amp;Ngarko Programth Zgjerimi...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Ngarko një programth Python si një zgjerim. Ipërdorur për ngarkim makrosh dhe për programthe të përparuar Python që zgjerojnë ndërfaqen e Scribus-it.</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Shfaq një konsol ndërvepruese Python ku ju mund të shkruani dhe xhironi programe Python që përdorin mjetet Scripter.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>&amp;Rreth Programthi...</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="obsolete">&amp;Rregullimet Scripter-i</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Rreth Programthi</translation>
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
        <translation type="obsolete">Modul ndërfaqeje Python për Scribus-in

Ky modul është ndërfaqja Python për Scribus-in. Ofron funksione
për të kontrolluar Scribus-in dhe për të manipuluar objekte te kanavaca. Çdo
funksion është i dokumentuar veçmas më poshtë.

Ca gjëra janë të përbashkëta nëpër shumicën e ndërfaqes.

Shumica e funksioneve veprojnë mbi korniza. Kornizat identifikohen nga emri i tyre,
një varg - ato nuk janë objekte realë Python. Plot funksione marrin një parametër
opsional (jo-fjalëkyç), një emër kornize.
Ka edhe plot përjashtime të zakonshëm nëpër shumicën e funksioneve. Këta nuk janë 
të dokumentuar për çastin në docstring-un për secilin funksion.
- Shumë funksione do të sjellin një NoDocOpenError nëse përpiqeni t&apos;i provoni pa një 
dokument mbi të cilin të veprohet. 
- Nëse një funksioni nuk i jepet një emër kornize i nevojshëm, funksioni do të përdorë 
kornizën e përzgjedhur për çastin, në pastë, ose përgjigjet ne një NoValidObjectError 
nëse nuk gjen diçka mbi të cilën të veprojë.
- Shumë funksione do të sjellin një WrongFrameTypeError nëse përpiqeni t&apos;i provoni
mbi një tip kornize e cila nuk ka kuptim për ta. Për shembull, rregullimi i ngjyrës
së tekstit në një kornizë grafikësh nuk ka kuptim, dhe do të japë si përfundim
shfaqjen e këtij gabimi.
- Gabime të rrjedhur prej thirrjesh drejt API-t Python nën të do të kalojnë 
të pandryshuar. Prej kësaj, lista e përjashtimeve (gabimeve) kthyer nga 
funksione, ashtu si jepet këtu dhe në docstring-et përkatës është e paplotë.

Hollësi se çfarë përjashtimesh mund të kthejë çdo funksion jepen në 
dokumentimin e funksionit.</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Formësim vetjak (opsional):</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Fomësim standard:</translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>&amp;Fjalë të Shkurtra...</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Po përpunoj Fjalë të Shkurtra. Ju lutem prisni...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Po përpunoj Fjalë të Shkurtra. Kryer.</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importo Vizatim &amp;OpenOffice.org...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>Vizatim OpenOffice.org (*.sxd);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Kartela Vlerash Ndarë me Presje (CSV)</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>Të dhëna CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>Titull CSV</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Kartela HTML</translation>
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
Lidhje të Jashtme
</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Dokumente OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Filtra Tekstesh</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Kartela Teksti</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroate</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugeze</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugeze (BR)</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vetjake</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Faqe</translation>
    </message>
    <message>
        <source>Template </source>
        <translation>Stampë</translation>
    </message>
    <message>
        <source>4A0</source>
        <translation></translation>
    </message>
    <message>
        <source>2A0</source>
        <translation></translation>
    </message>
    <message>
        <source>Comm10E</source>
        <translation></translation>
    </message>
    <message>
        <source>DLE</source>
        <translation></translation>
    </message>
    <message>
        <source>Couldn&apos;t open output file %1</source>
        <translation>S&apos;hapa dot kartelë përfundimi %1</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Verifikimi i rregullimeve dështoi: %1</translation>
    </message>
    <message>
        <source>Couldn&apos;t open input file %1</source>
        <translation>S&apos;hapa dot kartelë futjeje %1</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>I pazoti të lexoj rregullime XML:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (rreshti %2 shtylla %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>I pazoti të lexoj rregullime XML: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; i pavlefshëm</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>u gjetën %1 &lt;%2&gt; nyje, lipset 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>nyje e papritur null &lt;%2&gt;</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>nyja &lt;%1&gt; nuk është një element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elementit &lt;%1&gt; i mungon atributi `value&apos;</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>vlera e elementit &lt;%1&gt; duhet të jetë `true&apos; ose `false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>elementit &lt;lpiSettingsEntry&gt; i mungon atributi `name&apos;</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation>Gërmat %1 (të gjetura duke përdorur fontconfig) janë të dëmtuara, po i shpërfill</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype couldn&apos;t find the font file</source>
        <translation>Dështova të ngarkoj gërmat - freetype s&apos;gjeti dot kartelën e gërmave</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Versioni Scribus Zhvillimi</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x.
The process of saving will make files originating from versions of
Scribus of 1.2.x or lower unusable again in those versions.
Are you sure you wish to proceed with this operation?</source>
        <translation>Po xhironi një version zhvillimi të Scribus 1.3.x.
Procesi i ruajtjes do t&apos;i bëjë kartelat e dala nga versione të
Scribus 1.2.x ose më të hershëm të papërdorshme sërish në këta versione.
Jeni të sigurtë se doni të vazhdohet me këtë veprim?</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation>&amp;Kryej</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source> cm</source>
        <translation> cm</translation>
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
        <translation>inç</translation>
    </message>
    <message>
        <source>p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>cm</source>
        <translation></translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Pikë (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Milimetra (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Inç (inç)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Pikas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Centimetra (cm)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Ka një kartelë të tillë</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation>Ka tashmë një kartelë të quajtur &apos;%1&apos;.
Doni të zëvendësohet me kartelën që po ruani?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Zëvendëso</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tërë</translation>
    </message>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>&amp;Rrethi i Ngjyrave...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Paraparje Gërmash...</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Nuk u gjet ngjyrë.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Nuk u gjet ngjyrë në dokument.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Nuk u gjet ngjyrë në ngjyrat parazgjedhje.</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Dështova në hapjen e dokumentit.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Dështova në ruajtjen e dokumentit.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Argumenti duhet të jetë emër objekti faqeje, ose instancë PyCObject</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Nuk u gjet veti</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Nuk u gjet pjellë</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>S&apos;munda të shndërroj tip përfundimi &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Tip &apos;%1&apos; vetije i pambuluar</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>S&apos;munda të shndërroj &apos;%1&apos; në tip &apos;%2&apos; vetije</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Tipet përputhen, por rregullimi i vetisë dashtoi.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Objekt i treguar jo në një kornizë pamjeje.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Nuk mund të ripërmasoj me 0%.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Nuk u gjetën gërmat.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Nuk mund të skicoj një shembull bosh.</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>I pazoti të ruaj pixmap</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Nuk mund të kem një emër bosh shtrese.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Nuk u gjet shtresë.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Nuk mund të heq shtresën e fundit.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Nuk mund të krijoj shtresë pa emër.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Ka tashmë një objekt me emrin e kërkuar.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Lista e pikave duhet të përmbajë të paktën dy pika (katër vlera).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Lista e pikave duhet të përmbajë një numër çift vlerash.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Lista e pikave duhet të përmbajë të paktën tre pika (gjashtë vlera).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Lista e pikave duhet të përmbajë të paktën katër pika (tetë vlera).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Lista e pikave duhet të ketë një shumëfish gjashtë vlerash.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Nuk u gjetën objekt.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Nuk u gjet stil.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj stil në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Dështova në ruajtjen e EPS-së.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Numër faqeje jashtë intervali.</translation>
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
        <translation>Gjerësi vije jashtë caqesh, duhet të jetë 0 &lt;= gjerësi_vije &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Hije vije jashtë caqesh, duhet të jetë 0 &lt;= hije &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Mbushe hijeje jashtë caqesh, duhet të jetë 0 &lt;= hije &lt;= 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Nuk u gjet stil vije.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj drejtim tekstil në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Madhësi gërmash jashtë caqesh, duhet të jetë 1 &lt;= madhësi &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj madhësi gërmash në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj gërma në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Hapësirë vije jashtë caqesh, duhet të jetë &gt;=0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj hapësirë vije në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Boshllëk shtylle jashtë caqesh, duhet të jetë pozitiv.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj boshllëk shtyllash në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të caktoj numër shtyllash në një kornizë jo për tekst.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Nuk mund të përzgjedh tekst në një kornizë jo për tekst</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nuk mund të fshij tekst prej një kornize jo për tekst.</translation>
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
        <translation>Mund të lidh vetëm korniza teksti.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Korniza objektiv duhet të jetë bosh.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Korniza objektiv lidhet te një tjetër kornizë.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Pikënisja dhe objektivi janë i njëjti objekt.</translation>
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
        <source>Only text frames can be checked for overflowing</source>
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
        <source>&amp;Script</source>
        <translation>&amp;Programth</translation>
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
        <translation>Doracak Fjalësh të Shkurtra</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Dokumente Teksti OpenDocument</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Pastro</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Përzgjidhni Tërë</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ribëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Prij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Ngjit</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menu Sistemi</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Hijeso</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Çhijeso</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizo</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizo</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maksimizo</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Mbyll</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Rikthe</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Zhvendos</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>M&amp;adhësi</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>M&amp;inimizo</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>M&amp;aksimizo</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Rri &amp;Sipër</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizo</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation type="unfinished">Ul Volumin</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Mbyll</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Hi&amp;jezo</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Çhijezo</translation>
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
        <translation>&amp;Anulo</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Rregullim Dokumenti</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Djathtas:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Faqe Majtas fillimisht</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.(new line)
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.(new line)
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Brenda:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>&amp;Jashtë:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Madhësi:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Vetjake</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Orientim:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Së gjeri</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Gjerësi:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Lartësi:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Numër i Faqes së &amp;Parë:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Madhësi:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntim:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Njësi:</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation>Skemë</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Vetëruaj</translation>
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
        <translation>Udhëzuesa</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Ngjyrë:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Paraqit Zonë të &amp;Pashtypshme në Ngjyrë Mënjanash</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Shfaq Foto</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Shfaq Zinxhirë Tekstesh</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Shfaq Korniza</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Shfaq</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Mjete</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Ndarës</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Gërma</translation>
    </message>
    <message>
        <source>Document Checker</source>
        <translation>Kontrollues Dokumenti</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>Eksport PDF</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Veti Objekti Dokumenti</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabelë Përmbajtjeje dhe Treguesa</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Administrim Ngjyrash</translation>
    </message>
    <message>
        <source>Turns the of linked frames on or off</source>
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
        <translation>Ngjyrë për letrën</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Masko zonën jashtë mënjanash me ngjyrën e mënjanave</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation>Rregullime Stili</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stil për paragrafin e çastit</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Ngjyrë mbushjeje teksti</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Ngopje e ngjyrës për mbushje teksti</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation type="unfinished">Rregullime Administrimi Ngjyrash</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Ndërro Ngjyrën e Tekstit</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Rregullime Gërmash</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Gërma teksti të përzgjedhur</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Gjerësi ripërmasimi e gërmave</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Rregullimet Shenjash</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopjim i #%1 nga</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Sfond</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Kartelë</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Krijo një Dokument të ri</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Hap një Dokument</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Ruaj Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Mbyll Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Shtyp Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Ruaj Dokumentin e çastit si PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Po kërkoj për Gërma</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Nuk ka Gërma Postscript në Sistemin tuaj</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Po dal tani</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Gabim Fatal</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Vijë e Mençur</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Vendos Majtas</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Vendos Djathtas</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Vendos në Qendër</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Fut Numër Faqeje</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">bashkangjit Tekst te Shtegu</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Shfaq Shtresa</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Javascripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Zhbëj</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Shfaq Paletë Faqesh</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Blloko/Çblloko</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Hapësirë pas Shtylle</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Po lexoj Parapëlqimet</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Po marr Profile ICC</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Po rregulloj Shkurtprerje</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">Administrimi &amp;Ngjyrash...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Po lexoj nga e Papastër</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Po gatis Shtojca</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Hap...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Hap të &amp;Fundmin</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">E re</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Hap...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Mbyll</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Ruaj</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Ruaj</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Ruaj  si...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Ri&amp;kthehu tek i Ruajtur</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">&amp;Kërko për Ndihmë...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Merr Tekst/Pamje...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Vini &amp;Tekst...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Importo &amp;Faqe(t)...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importo</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Ruaj &amp;Tekst...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Ruaj Faqe si &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Ruaj si &amp;PDF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Eksporto</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">Të &amp;dhëna Dokumenti...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Të dhëna Dokumenti...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">&amp;Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Shtyp...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Shtyp...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Lër</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Lër</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Zhbëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">P&amp;ri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">P&amp;astro</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Përzgjidhni &amp;Tërë</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Pri</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Kopjo</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Ngjit</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Fshi</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Përzgjidhni tërë</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Kërko/Zëvendëso...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">Ngj&amp;yra...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Ngjyra...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">Stile &amp;Paragrafi...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">Stile &amp;Vije...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Stile...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Stampa...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Stampa...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Përzgjidhni Gërma të Reja</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Dyfisho</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">Kopje të &amp;Shumëfishta</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Kopje të Shumëfishta</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Fshi</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Fshi</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">&amp;Grupo</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Grupo</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Çgrupo</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Çgrupo</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">&amp;Blloko</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Shpjere &amp;Prapa</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Shpjere Prapa</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Bjer &amp;Përpara</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Bjere Përpara</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Ul</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Ul</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Ngri</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Ngri</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Shpërnda/&amp;Drejto...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Shpërnda/Drejto...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">&amp;Përpunoni Formë</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Formë</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">&amp;Bashkangjit Tekst tek Shteg</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">&amp;Shqit Tekst prej Shtegu</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">&amp;Ndërthur Shumëkëndësha</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Ndaj &amp;Shumëkëndësha</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">Sh&amp;ndërro në Jashtëvija</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Fut...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Fut...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Fshij...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Fshij...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Zhvendos...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Zhvendos...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Zbato Stampë...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Zbato Stampë...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Administro &amp;Udhëzuesa...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Administro Udhëzuesa...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">&amp;Sa Dritarja</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Sa Dritarja</translation>
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
        <translation type="obsolete">Fi&amp;gurëza</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Figurëza</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Shfaq &amp;Mënjana</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Fshih Mënjana</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Shfaq &amp;Korniza</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Fshih Korniza</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Shfaq &amp;Pamje</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Fshih Pamje</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Shfaq Rrjetë</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Shfaq &amp;Udhëzuesa</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Tekst për tu vizatuar</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Veti</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Veti</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Jashtëvijëzo</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;E papastër</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">E papastër</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Shtresa</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">Paletë Fa&amp;qeje</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Faqerojtësa</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">&amp;Administro Pamje</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Administro Pamje</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">&amp;Vijëzo Tekst</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Vijëzo Tekst</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Ndih&amp;mëza</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Mjete</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">Mjete P&amp;DF</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Mjete</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Ndihmëza</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">Pa&amp;rapëlqime...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Gërma...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Gërma...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">Shkurtprerje &amp;Tastiere...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Rreth Scribus-it</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Rreth Scribus-it</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Rreth &amp;Qt-s</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Rreth Qt-s</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Ndihmë Online...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>St&amp;il</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Faqe</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Parje</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">&amp;Rregullime</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Dritare</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Ndihmë</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Majtas</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Qendër</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">&amp;Djathtas</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Blloko</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Tjetër...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">pt</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Nënvijë</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Hequrvije</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Sipërshkrim</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Poshtëshkrim</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Poz:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>X-Poz:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Gati</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Ujvarë</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Merr Tekst/Pamje...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Merr Pamje...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Ngjyrë</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Përmbys</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">&amp;Merr Tekst...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Gërma</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Madhësi</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efekte</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Drejtim</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Hije</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Tabelëzuesa...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Merr Tekst...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Gërma</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Madhësi</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Stil</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Ngjyrë</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Hije</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Çblloko </translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Po importoj Faqe...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importoni Faqe(t)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Po provoni të importoni më tepër faqe se sa janë të passhme në dokumentin e çastit po qe se numërohen nga faqja aktive.&lt;/p&gt;Zgjidhni një nga vijueset:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Krijo&lt;/b&gt; faqe që mungojnë&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importo&lt;/b&gt; faqe deri te faqja e fundit&lt;/li&gt;&lt;li&gt;&lt;b&gt;Anulo&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Krijo</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importo</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Import i kryer</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nuk u gjet gjë për ta importuar</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Po ngarkoj...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Tërë Formatet e Mbuluar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">S&apos;shkruaj dot Kartelën: 
%1</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Ruaj  si</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Po ruaj...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Po shtyp...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Shtypja dështoi!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Doracak për Scribus-in</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Kartela Tekst (*.txt);;Tërë Kartelat(*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Madhësi:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Hije:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Programet vijues mungojnë:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript: Nuk mund të përdorni Pamje EPS</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tërë</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">Kartela EPS (*.eps);;Tërë Kartelat (*)</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Disa objekte janë të kyçur.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Anulo</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Kyç tërë</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Çkyç tërë</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Çkyç</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Kyç</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Po ngarkoj:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Po ujdis Ngjyra</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Zhbëj Fshirje Objekti</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Zhbëj Zhvendosje Objekti</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Zhbëj Ndryshim Objekti</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Angleze</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Gjermane</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spanjolle</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italisht</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Frënge</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruse</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Daneze</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slovake</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Hungareze</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Çeke</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandeze</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugeze</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukrainase</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polake</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Greke</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalane</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandeze</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irlandeze</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituaneze</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Suedeze</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovene</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Zgjidhni një Drejtori</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Përmbys</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Vithisje Scribus-i</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus-i vithiset për shkak të Sinjalit #%1</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Sistem Gërmash i Gatshëm</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Kartela %1 nuk është në format Scribus</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaanse</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugeze (BR)</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Përpunoni</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Nuk gjenden gërma në sistemin tuaj.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Po dal tani.</translation>
    </message>
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
        <translation>Të migroj Rregullime të Vjetër Scribus-i?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus-i ka zbuluar kartela ekzistuese parapëlqimesh Scribus 1.2.
Doni të sillen ato te versioni i ri i Scribus-it?</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Nivel</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Mundësi &amp;PDF</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>&amp;Shndërro Në</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Fut</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Gërmë</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Hapësirë</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation>Dokumente (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Kartela Postscript (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation>Pamje SVG (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation>Pamje SVG (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>Vizatim OpenOffice.org (*.sxd);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>File %1 
is not in an acceptable format</source>
        <translation>Kartela %1 
nuk është në një format të pranueshëm</translation>
    </message>
    <message>
        <source>Some Fonts used by this Document have been substituted:</source>
        <translation>Disa Gërma të përdorura nga ky Dokument janë zëvendësuar:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation>u zëvendësua nga:</translation>
    </message>
    <message>
        <source>Some ICC-Profiles used by this Document are not installed:</source>
        <translation>Disa Profile ICC të përdorur nga ky Dokument nuk janë të instaluar:</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(shndërruar)</translation>
    </message>
    <message>
        <source>Cannot write the File: 
%1</source>
        <translation>Nuk shkruaj dot Kartelën: 
%1</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Abort</source>
        <translation>Ndërpre</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation>Shpërfill</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Programet vijues mungojnë:</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>Kartela EPS (*.eps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Nuk shkruaj dot kartelën: 
%1</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
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
        <source>Croatian</source>
        <translation>Kroate</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bullgare</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Programi</translation>
    </message>
    <message>
        <source>is already running!</source>
        <translation>është duke xhiruar!</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Të dhëna</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>mungon!</translation>
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
        <translation>Shtresë</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Tërë</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Kopjo Këtu</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Zhvendos Këtu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Ngjit</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Pamje</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Kartelë:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>PPI fillestar: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>PPI i tanishëm: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Tekst i Lidhur</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Kornizë Teksti</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Tekst sipas një Shtegu</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragrafë:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Fjalë:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Gërma:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Shtyp:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Veprues</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Çaktivizuar</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>Të dhë&amp;na</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>&amp;Përditëso Pamje</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Përpunoni Pamje</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>&amp;Përshtatja Kornizën Pamjes</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Është &amp;Faqerojtës PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Është Sh&amp;ënim PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>&amp;Veti Shënimesh</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Veti Fushe</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Mundësi &amp;PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Dërgo te Sh&amp;tresë</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ni&amp;vel</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Shn&amp;dërro si </translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshi</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Doni vërtet të pastroni tërë Tekstin tuaj?</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopje e</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>&amp;Dil Sidoqoftë</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>M&amp;byll Sidoqoftë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>është ndryshuar që prej ruatjes së fundit.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Ruaj Tani</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>Script error</source>
        <translation>Gabim programthi</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Nëse po xhironit një programth zyrtar ju lutemi raportojeni te &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Këtë mesazh e keni edhe në clipboard. Përdorni Ctrl+V për ta hedhur te gjurmues bug-esh.</translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation type="obsolete">Scribus - Parapëlqime Scripter-i</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation type="obsolete">Aktivizo Zgjerime Scripter-i</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation type="obsolete">Vëri në punëprogramthet dhe makrot</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Aktivizimi i zgjerime të programuesit vë në punë funksione shtesë programuesi përfshi makro Python dhe mundësinë e ngarkimit të një programthi Python gjatë fillimit. Duke e bërë veprues këtë mundësi zhbllokohet përbërësi &lt;tt&gt;Ngarko Programth të Jashtëm&lt;/tt&gt; te menuja Programth.&lt;/p&gt;
&lt;p&gt;
Me &lt;tt&gt;Ngarko Programth të Jashtëm&lt;/tt&gt; ose si programthe fillimi do të duhej të përdoreshin vetëm programthet e shkruajtur të xhirojnë si programthe zgjerim. Shihni dokumentimin e programuesit për më tepër hollësi.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation type="obsolete">Përdor një Programth Nisjeje</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Nëse &lt;tt&gt;Përdor një Program Nisjeje&lt;/tt&gt; është e zgjedhur, Scribus-i do të ngarkojë kartelën e programthit të treguar këtu si një programth zgjerimi gjatë nisjes. Është e rëndësishme që programthi të jetë shkruajtur si një programth zgjerimi, ngaqë nëse nuk është shkruajtur me kujdes mundet të shkaktojë probleme.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Përdor një Program Nisjeje&lt;/tt&gt; do të çaktivizohet nëse çaktivizohen zgjerimet e programuesit, ngaqë programthe zgjerimi nuk mund të ngarkohen pa zgjerime programuesi zgjerimesh të aktivizuara.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="obsolete">Shfleto...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="obsolete">Shfleto për një kartelë programthi të ri</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Shfleto për një kartelë programthi të ri&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation type="obsolete">Cilën kartelë programthi të ngarkoj në nisje</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Kartela përmban një programth Python që duhet xhiruar gjatë nisjes si një programth zgjerimi.&lt;/p&gt;
&lt;p&gt;Mbani shënim që kur ky programth xhirohet, Scribus-i nuk ka përfunduar ende nisjen dhe hapësira e punës nuk ekziston ende.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation type="obsolete">Kartelë Programthi:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Anulo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation type="obsolete">Mbyll pa ruajtur ndryshime</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Ok</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="obsolete">Ruaj ndryshime dhe mbyll</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Mundësi të Mëtejshme</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation type="obsolete">Importo Tërë Emrat gjatë Nisjes</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation type="obsolete">Xhiro  gjatë nisjes &apos;from scribus import *&apos; te konsol programthesh</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Importo Tërë Emrat gjatë Nisjes&lt;/tt&gt; është një mundësi e përparuar. Mbase do të duhej ta lini të shenjuar nëse nuk keni lexuar dokumentimin dhe dini se çfarë po bëni.&lt;/p&gt;
&lt;p&gt;Çshenjimi i kësaj mundësie do të pengojë programuesin të xhirojë urdhrin e vet të zakonshëm &lt;tt&gt;from scribus import *&lt;/tt&gt; kur përgatit interpretuesin kryesor (e përdorur për konsol programthesh dhe programthe zgjerimei) gjatë nisjes.&lt;/p&gt;
&lt;p&gt;Kjo mundësi nuk ka veprim para se të riniset Scribus-i.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation type="obsolete">Aktivizo Aliase të Dikurshëm Emrash</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Aktivizo përdorimin e emravë të funksioneve OldStyle&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Aktivizo Aliase të Dikurshëm Emrash&lt;/tt&gt; është një mundësi e mëtejshme. Mbase do të duhej ta lini siç është.&lt;/p&gt;
&lt;p&gt;Në qoftë e shenjuar, kjo mundësi do të bëjë që programuesi të krijojë një numër të madh aliasesh emrash funksionesh dhe konstantesh për përputhshmëri me programthe 1.2.0. Parazgjedhja është shenjimi.&lt;/p&gt;
&lt;p&gt;Kjo mundësi nuk ka veprim para se të rinisset Scribus-i.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="obsolete">Përdor Stdin të Rremë</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Zëvendëso sys.stdin me një kartelë të rreme për të parandaluar ngecjen e Scribus-it kur një programth përpiqet të lexojë prej stdin.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Përdor Stdin të Rremë&lt;/tt&gt; është një mundësi e mëtejshme. Mbaset duhet ta lini siç është.&lt;/p&gt;
&lt;p&gt;Normalisht, Scribus-i do t&apos;i ofrojë Python-it një objekt të rremë kartele për &lt;tt&gt;sys.stdin&lt;/tt&gt;, që leximet prej stdin të kthejnë përherë një varg bosh. Nëse lihet &lt;tt&gt;sys.stdin&lt;/tt&gt; i vërtetë, programthet që përpiqen të lexojnë prej tij do të bllokohen - e kështu do të bllokojnë edhe punën e Scribus-it, duke bërë që zbatimi i tërë të duket sikur ngeci - deri sa të ketë futje në stdin. Është e pazakontë për zbatime GUI të presin diçka në stdin, kështu që shumica e përdoruesve do të mendojnë se Scribus-i u vithis.&lt;/p&gt;
&lt;p&gt;Mund ta çaktivizoni këtë mundësi nëse doni të pranohen futje prej stdin. Përgjithësisht më mirë do të duhej të përdornit &lt;tt&gt;os.popen&lt;/tt&gt; për të pasur një kanal lidhës, ose përdorni ndonjë mekanizëm tjetër futjesh, por kjo mundësi mbetet këtu për çdo rast.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Shfaq Paraparje Faqeje</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Shfaq Emra Stampe</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Kërko/Zëvendëso</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Kërko për:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Stil Paragrafi</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Gërma</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efekte Gërmash</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Ngjyrë Mbushjeje</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>HIje Mbushjeje</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation type="unfinished">Ngjyrë sfondi:</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation type="unfinished">Tejdukshmëri me nuancë (hije)</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Majtas</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Qendër</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Djathtas</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blloko</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Detyrueshëm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Zëvendëso me:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Tërë Fjalën</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Shpërfill të Mëdhaja/Vogla</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Kërko</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Zëvendëso</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Zëvendëso &amp;Tërë</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Mbyll</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Kontroll i përfunduar</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Pastro</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Sistemo Faqe</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Stampa të Mundshme:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Faqe Dokumenti:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Faqe Përkarshi</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Faqja Majtas e Para</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Hiqni Faqe apo Stampa Faqesh zvarrë te koshi i Hedhurinave për t&apos;i fshirë.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Parasheh tërë faqet e dokumentit tuaj.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Këtu keni tërë Stampat tuaja, për të krijuar Faqe të re(new line)
tërhiqni një Stampë te parja e Faqes më poshtë.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normale</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Përzgjidhni Fusha</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Fusha të Mundshme</translation>
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
        <translation>Fusha të Përzgjedhura</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Tjetër...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Hije:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Hije</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Pa Stil</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Përpunoni Stile...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Pa Stil</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Përpunoni Stile</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshi</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopje e %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Stil i Ri</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you really want do delete this Style?</source>
        <translation type="obsolete">Doni vërtet të fshini këtë Stil?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Jo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Po</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Doni vërtet të fshini këtë Stil?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Përpunues Historie</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;I ri</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Ringarko Tekst prej Kornize</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Ruaj te Kartelë...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Ngarko prej Kartele...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Ruaj &amp;Dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Përditëso Kornizë Teksti dhe Dil</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Dil Pa Përditësuar Kornizë Teksti</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Përzgjidhni &amp;Tërë</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>P&amp;ri</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Kërko/Zëvendëso...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Fut Special...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Përpunoni Stile...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Paraparje Gërmash...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Përditëso Kornizë Teksti</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Sfond...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Shfaq Gërma...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Rregullime</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Kartelë</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Pastro tërë Tekstet</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Ngarko Tekst prej Kartele</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Ruaj Tekst në Kartelë</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Përditëso Kornizë Teksti dhe Dil</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Dil Pa Përditësuar Kornizë Teksti</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Ringarko Tekst prej Kornize</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Përditëso Kornizë Teksti</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Kërko/Zëvendëso</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Paragraf i Çastit:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Fjalë:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Gërma:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Gjithsej:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Paragrafë:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Kujdes</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Doni të ruani ndryshimet tuaj?</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Fut Speciale</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Paraparje Gërmash</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Doni vërtet të humbasin tërë Ndryshimet tuaj?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Doni vërtet të pastroni tërë Tekstin tuaj?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Hap</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Kartela Tekst (*.txt);;Tërë Kartelat(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Ruaj  si</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>Përzgjedhje e &amp;mençur teksti</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Fut Shenjë...</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Nënvijë</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Poshtëshkrim</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Sipërshkrim</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Hequrvije</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Jashtëvijëzo Tekst</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Jashtëvijë</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Mundësi Importuesi për OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation>Përditëso stile paragrafësh</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation>Nëse një stil paragrafi me të njëjtin numër si paragrafi i dokumentit OpenOffice.org
ekziston tashmë, stili në Scribus do të duhej të të përpunohej për përputhje me
atë të importuar, ose të lihet i paprekur</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation>Paketo stile pargrafësh</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation>Grupo stile paragrafesh sipas atributesh.
Më pak stile paragrafesh por kontrolli i tyre mund të jetë i vështirë.
Do të duhej të përdorej nëse dihet që teksti nuk duhet përpunuar
pas përpunimi.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Përdor emra dokumentesh si parashtesë për stile paragrafësh </translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation>Nëse do të duhej që importuesi të shtonte emrin e dokumentit
në ballë të emrit të stilit të paragrafit në Scribus</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Mos pyet sërish</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation>Nëse do të duhej që importuesi të përdorë përherë vlerën
e vendosur për çastin kur importon dokument OpenOffice.org dhe
të mos kërkojë ripohimin tuaj sërish</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Tabelë Përmbajtjeje dhe Treguesa</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Tabelë Përmbajtjeje</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Shto</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshi</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Emër Atributi Objekti:</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Basic</source>
        <translation>Bazë</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Kornizë Destinacion:</translation>
    </message>
    <message>
        <source>Index</source>
        <translation>Tregues</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Shpërfill tërë gabimet</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Kontroll automatik para shtypjeje apo eksportimi</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Kontrollo në mungojnë shenja</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Kontrollo për objekte jo në një faqe</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Kontrollo për tejdukshmëri të përdorura</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Kontrollo në mungojnë pamje</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Kontrollo qartësi pamjesh</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Qartësia më e vogël e lejuar</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Kontrollo për Kartela PDF-sh të vendosura</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Kontrollo për Shënime ose Fusha PDF-je</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Shto Profil</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Hiq Profil</translation>
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
        <translation>Në Sfond</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>Në Ballë</translation>
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
        <translation>Merr Rrënjë:</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Shfaq Udhëzuesa</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Ngjyrë:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Shfaq Mënjana</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Shfaq Rrjetë Faqeje</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Rrjetë e Madhe</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Rrjetë e Vogël</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Hapësirë midis vijash rrjete të vogël</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Hapësirë midis vijash rrjete të madhevogël</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Rrezja e zonës ku Scribus-i do t&apos;ju lejojë të mbërtheni veshët e një objekti</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Ngjyrë e vijave të rrjetës së vogël</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Ngjyrë e vijave të rrjetës së madhe</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Ngjyrë e vijave udhëzuese që fusni</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Ngjyrë për vija mënjanash</translation>
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
        <translation>Administro Tabelëzues</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Eksporto Interval</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Tërë Faqet</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Mundësi Kartele</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Përp&amp;uthshmëri:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Mënjanë Majtas</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Mënjanë Djathtas</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Ruaj Korniza të &amp;Lidhura Tekstesh si Artikuj PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Përfshi Faqerojtësa</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation>Përfshi Shtresa</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>N&amp;gjesh Tekst dhe Grafikë Vektorialë</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Rregullime Pamjeje</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatike</translation>
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
        <translation>Asnjë</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Metodë:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Cilësi:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Më i madhi</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Lartë</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mesatare</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Ulët</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Më i vogli</translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Të përgjithshme</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Trupëzo tërë Gërmat</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Po trupëzoj</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Gërma të Mundshme:</translation>
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
        <translation>Gërma për t´u trupëzuar:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Gërma</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>Aktivizo Efekte &amp;Paraqitjesh</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Faqe</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Shfaq P&amp;araparje Faqesh</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efekte</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Shfaq Kohëzgjatje:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Kohëzgjatje Efek&amp;ti:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>T&amp;ip Efekti:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">Po zh&amp;vendos Rreshta:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>P&amp;rej:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>Drejt&amp;im:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation>sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Pa Efekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Kuti</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Ndaj</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontalisht</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikalisht</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Brenda</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Jashtë</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Nga e Majta në të Djathtë</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Nga Sipër Poshtë</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Nga Poshtë Sipër</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Nga e Djathta në të Majtë</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Nga Sipër-majtas Poshtë-djathtas</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Zbato Efekt te tërë Faqet</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Përdor Kodim</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Fjalëkalime</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Përdorues:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>Pr&amp;onar:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Rregullime</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Lejo &amp;Shtypjen e Dokumentit</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Lejo &amp;Ndryshimin e Dokumentit</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Lejo Kop&amp;jim Tekstesh dhe Grafikash</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Lejo Shtim Shëni&amp;mesh dhe Fushash</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>S&amp;iguri</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Të përgjithshme</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Ekran / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Shtypës</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Shkallë të Grisë</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Përdor Rregullime Vetjake Vizatimi</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Rregullime Vizatimi</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Shpe&amp;shti:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Kënd:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Pikë e Thjeshtë</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Vijë</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Rreth</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Ngjyra Të plota:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Përdor Profil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Kolorimetrik Relativ</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Kolorimetrik Absolut</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Pamje:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Mos përdor profile ICC të trupëzuar</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>Ngj&amp;yrë</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>Varg të &amp;Dhënash:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Profil Për&amp;fundimesh:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Listë Kuti</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Trupëzo gërma te PDF-ja. Trupëzimi i gërmave(new line)
do të ruajë skemën dhe dukjen e dokumentit tuaj.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Shfaq paraparje faqeje për secilën nga faqet e listuara më sipër. </translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Koha gjatë së cilës tregohet faqja para se të nisë paraqitja në faqen e përzgjedhur.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Zbato efektin e përzgjedhur te tërë faqet.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Eksporto tërë faqet si PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Eksporto interval faqesh si PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Jepni një listë objektesh të ndarë me presje ku(new line)
objekti mund të jetë * për tërë faqet, 1-5 për(new line)
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Prodhon figurëza për çdo faqe të PDF-së.(new line)
Disa parësa mund të përdorin figurëza për lundrim.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Prodho Artikuj PDF, çka është e dobishme për lundrim artikujsh në një PDF.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation>Shtresat në dokumentin tuaj janë u eksportuan te PDF-ja
Të passhëm vetëm nëse zgjidhet PDF 1.5.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Trupëzo faqerojtësit që krijuat në dokumentin tuaj.
Këta janë të dobishëm për lundrim dokumentesh të gjatë PDF.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Eksporto qartësi teksti dhe grafikësh vektorialë.(new line)
Kjo nuk prek qartësinë e pamjeve bitmap, si fotot p.sh.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Ngjeshje teksti dhe grafike.(new line)
Nëse nuk keni arsye, lëreni të shenjuar. Kjo zvogëlon madhësinë e PDF-së.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Version ngjeshjeje për pamje.(new line)
Automatikja i lejon Scribus-it të zgjedhë metodën më të mirë.(new line)
ZIP është i mirë për pamje me ngjyra të plota.(new line)
JPEG është më i mirë pëër krijim kartelash PDF më të vogla të cilat mund të kenë plot foto (me pakëz humbje në pamje, si mundësi).(new line)
Lëreni të caktuar te automatike, veç në paçi nevojë për mundësira speciale ngjeshjeje.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Shkallë ngjeshjeje: Më e ulta (25%), E ulët (50%), E mesme (75%), E lartë (85%), Më e larta (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Thjeshtoni pamjet tuaja bitmap deri te vlera DPI e përzgjedhur.(new line)
Po e latë të pashenjuar dotë vizatohen me qartësinë origjinale.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Pika për Inç) për eksport pamjesh.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Aktivizoni karakteristika sigurie në PDF-të tuaja të eksportuara.
Nëse përzgjodhët PDF 1.3, PDF-ja do të mbrohet me kodim 40 bitësh.
Nëse përzgjodhët PDF 1.4, PDF-ja do të mbrohet me kodim 128 bitësh.
Klauzolë: Kodimi PDF nuk është aq i besueshëm sa kodimi GPG ose PGP dhe ka disa kufizime.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Zgjidhni një fjalëkalim të përgjithshëm i cili aktivizon ose çaktivizon tërë
karakteristikat e sigurisë në PDF-në tuaj të eksportuar</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Zgjidhni një fjalëkalim i cili t&apos;u lejojë përdoruesve të lexojnë PDF-të tuaja.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Lejo shtypjen e PDF-së. Nëse nuk shenjohet, parandalohet shtypja.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Lejo ndryshimin e PDF-së. Nëse nuk shenjohet, parandalohet ndryshimi i PDF-së.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Lejo kopjim teksti ose grafikash prej PDF-së. 
Në u lëntë pa shenjuar, nuk do të mund të kopjohen tekste dhe grafika.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation>Lejo shtim shënimesh dhe fushash te PDF-ja. 
Në u lëntë pa shenjuar, pengohet përpunimi i shënimeve dhe fushave.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Model ngjyre për përfundimin e PDF-së tuaj.(new line)
Zgjidhni Ekran/Web për PDF që përdoren për shfaqje në ekran dhe për shtypje në shtypësa inkjet të zakonshëm.(new line)
Zgjidhni Shtypës kur shtypet vërtet me një shtypës 4 ngjyrësh CMYK.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Ky është një rregullim i mëtejshëm i cili nuk është i aktivizuar paraprakishtt. Do të duhej 
të aktivizohej vetëm kur kërkohet shprehimisht nga shtypësi juaj dhe ju kanë dhënë 
hollësitë e sakta të nevojshme. Përndryshe, PDF-ja juaj e eksportuar mund të mos shtypet
si duhet dhe të mos jetë vërtet e kalueshme nga një sistem në një tjetër.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Trupëzo një profil ngjyre për ngjyra të plota</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Profil ngjyre për ngjyra të plota</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Trupëzo profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Mos përdor profile ngjyrash që genden të trupëzuar në burimin e pamjes</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Profil përfundimesh për shtypjen. Nëse mundet, merrni ca udhëzime prej shtypësit tuaj rreth përzgjedhjes së profilit.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Vargu i detyrueshëm për PDF/X-3 ose PDF nuk do të plotësojë
përputhshmëri me PDF/X-3. Këshillojmë të përdorni titullin e dokumentit.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Pasqyro Faqe(t) horizontalisht</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Pasqyro Faqe(t) vertikalisht</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Gërma:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Madhësi:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Asnjë</translation>
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
        <translation>Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Ngjyrë &amp;Vije:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Hije:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>H&amp;ije:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Stil Vije:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Gjerësi Vije:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>&amp;Stil Vije:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Shigjeta:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Fillim:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Fund:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Ripërmasim i &amp;Lirë</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Ripërmasim &amp;Horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Rpërmasim &amp;Vertical:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Ruaj &amp;Përpjestim</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Ngjyrë M&amp;bushje:</translation>
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
        <translation>Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Hapje:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Veti Kornize Teksti</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Veti Kornize Pamjeje</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Veti Vizatimi Formash</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Parazgjedhje Shkalle Zmadhimi</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Veti Vizatimi Vijash</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Veti Vizatimi Shumëkëndëshash</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Gërma për korniza të reja tekstesh</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Madhësi gërmash për korniza të reja teksti</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Ngjyrë gërmash</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Numër shtyllash në një kornizë teksti</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Hapësirë ndërmjet shtyllash kornize teksti</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Shembull i gërmave tuaja</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Korniza pamjesh lejojnë pamjet të ripërmasohen në çfarëdo madhësi</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Ripërmasim horizontal i pamjeve</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Ripërmasim vertikal i pamjeve</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Mbaj ripërmasim horizontal dhe vertikal njësoj</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Pamjet e ripërmasuara vetvetiu ruajnë përpjestimet e veta fillestare</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Ngjyrë mbushjeje për korniza pamjesh</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Ngopje ngjyre mbushjeje</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Ngjyrë vije formash</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Ngopje e ngjyrës për vija</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Ngjyrë mbushjeje për forma</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Stil vije formash</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Gjerësi vije formash</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Zmadhimi më i vogël i lejuar</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Zmadhimi më i madh i lejuar</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Ndryshimi në zmadhim për çdo veprim &quot;zoom&quot;</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Ngjyrë vijash</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Ngopje e ngjyrës</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Stil vijash</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Gjerësi vijash</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Poshtëshkrim</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Zhvendosje:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Ripërmasim:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Sipërshkrim</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>Zh&amp;vendosje:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Ri&amp;përmasim:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ripërm&amp;asim:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>&amp;Ndërvijë Automatike</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Ndërvijë:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Madhësi relative e mbishkrimit krahasuar me gërmat normale</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Madhësi relative e poshtëshkrimit krahasuar me gërmat normale</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Madhësi relative e gërmave të vogla të mëdha krahasuar me gërmat normale</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Rritje në përqindje mbi madhësi gërmash për ndërvijën</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Majtas</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Djathtas</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Ndalesë e Plotë</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Presje</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Qendër</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Pozicion:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>R&amp;reshti i Parë:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Fshij Tërë</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Kryeradhë për rreshtin e parë të paragrafit</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Kryeradhë nga e majta për tërë paragrafin</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Fshi tërë Tabelëzuesit</translation>
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
        <translation type="obsolete">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">p</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation>Krye&amp;radhë Majtas:</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Element</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tip</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Të dhëna</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Emri &quot;%1&quot; s&apos;është unik.
Ju lutem zgjidhni një tjetër.</translation>
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
        <translation type="obsolete">Gërma:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grup</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Pamje</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Rresht</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Shumëkëndësh</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Shumëvijë</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Tekst Shtegu</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Faqe</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Objekte të Lirë</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Faqe</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Shto udhëzues vertikal</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Shto udhëzues horizontal</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Hiq udhëzues vertikal</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Hiq udhëzues horizontal</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Lëviz udhëzues vertikal</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Lëviz udhëzues horizontal</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Kyç udhëzuesa</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Çkyç udhëzuesa</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Zhvendos</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Ripërmaso</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Rrotullo</translation>
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
        <translation>Përzgjedhje</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Grup</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Përzgjedhje/Grup</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Krijo</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Drejto/Shpërnda</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Objekte të përfshirë</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Anulo</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Caktoni ngjyrë mbushjeje</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Color1: %1, Color2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Caktoni ngjyrë vijash</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Caktoni hije ngjyre vijash</translation>
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
        <translation>Kyç</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Çkyç</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Kyç madhësi</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Çkyç madhësi</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Çgrupo</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Fshi</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Riemërto</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Nga %1
te %2</translation>
    </message>
    <message>
        <source>Apply template</source>
        <translation>Zbato Stampë</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Ngjit</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Pri</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Rregulloni tejdukshmërinë për ngjyrë mbushjeje</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Rregulloni tejdukshmërinë për ngjyrë vije</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Caktoni stil vije</translation>
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
        <translation>Caktoni gjerësi vije</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Pa Stil</translation>
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
        <translation>Krijoni tabelë</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Rreshta: %1, Shtylla: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Caktoni gërma</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Caktoni madhësi gërmash</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Caktoni gjerësi gërmash</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Caktoni ngjyrë mbushjeje gërmash</translation>
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
        <translation>Caktoni ndërvijë</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Caktoni stil paragrafi</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Caktoni gjuhë</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Drejtoni tekstin</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Caktoni efekte gërmash</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Kornizë pamjesh</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Kornizë tekstesh</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Shumëkëndësh</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Kurbë Bezier</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Shumëvijë</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Shndërro në</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>Importo pamje SVG</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>Importo pamje EPS</translation>
    </message>
    <message>
        <source>Import OpenOffice draw image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Teksti rrjedh rreth kornizës</translation>
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
        <translation>Pa  vijë rrethine</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Faqe %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Madhësi kornize</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Ripërmasim i lirë</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Ruaj përpjestim</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Prish përpjestim</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Përpunoni  vijë rrethine</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Përpunoni formë</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Rirregulloni vijë rrethine</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Shto faqe</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Shto faqe</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Fshi faqe</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Fshi faqe</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Shto shtresë</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Fshi shtresë</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Riemërto shtresë</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Ngri shtresë</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Ul shtresë</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Dërgo te shtresë</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Aktivizo shtypje shtrese</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Çaktivizo shtypje shtrese</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Ndrysho emrin e shtresës</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Merr pamje</translation>
    </message>
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Action History</source>
        <translation>Historik Veprimesh</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Shfaq vetëm objektin e përzgjedhur</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Ribëj</translation>
    </message>
    <message>
        <source>Initial State</source>
        <translation>Gjendje Fillestare</translation>
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
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Futni vlerë</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Jepni një vlerë e mandej shtypni OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Jepni një vlerë e mandej shtypni OK</translation>
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
        <translation>Dërgoni vlerën tuaj te programthi</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Fjalë të Shkurtra</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Zbato hapësirë të pandashme në:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>Korniza të &amp;Përzgjedhura</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>&amp;Faqe aktive</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Tërë objektet</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Të dhëna dhe 
Gjuhë</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation>Zëvendëso parazgjedhje me formësime përdoruesi</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation>Kur ka kartelë formësimesh përdoruesi
(%1)
mund të zgjidhni nëse doni të shtoni formësimin tuaj
te formësimi i përgjithshëm pa vënë shenjë te butoni.

Mund të zëvendësoni vlerat e parazgjedhura me tuajat
duke vënë shenjë te butoni.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Vetëm korniza të përzgjedhura të përpunuara.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Vetëm faqen aktuale të përpunuar.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Tërë objektet e përpunuar te dokumenti.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation>Fjalë të Shkurtra për Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation>Të passhme në gjuhët vijuese</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation>Rreth Fjalësh të Shkurtra</translation>
    </message>
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation>Përpunoni formësim &amp;sistemi...</translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation>Përpunoni formësim &amp;përdorues...</translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Përpunoni</translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation>&amp;Të dhëna dhe Gjuhë...</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Ndihmë</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Fjalë të Shkurtra</translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation>Po filloni të përpunoni një kartelë që vetëm mund të lexohet.
%1</translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation>Gabim zbatimi &apos;%1&apos;. Nuk mund të fillohet.</translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation>Rregullim Fjalësh të Shkurtra</translation>
    </message>
    <message>
        <source>Enter name of the plain text editor executable:</source>
        <translation>Jepni emrin e të ekzekutueshmit për përpunues të thjeshtë tekstesh:</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Mjete</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Përzgjidhni Elementë</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Fut Kornizë Teksti</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Fut Pamje</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Fut Tabelë</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Veti...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Fut Shumëkëndësha</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Fut Vija</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Fut Kurba Bezier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Fut Vijë me Dorë të Lirë</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Rrotullo Element</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Zmadho ose zvogëlo</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Përpunoni Përmbajtje Kornize</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Përpunoni tekstin me Përpunues Historie</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Lidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Zgidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Kryej matje</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Vizatoni Forma të ndryshme</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>Mjete PDF</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Buton</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Fushë Tekstesh</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kutizë</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Listë Kuti</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Kuti Liste</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Fusni Fusha PDF</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Lidhje</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Fusni Shënime PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Përzgjidhni Gërma:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Fut</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Pastro</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Fut gërma në tekst atje ku gjendet kursori</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Fshi përzgjedhjen(t) e çastit.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Mbyll këtë dialog dhe kthehu te përpunimi i tekstit.</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Zgjidhni importuar për t&apos;u përdorur</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatike</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation>Merr vetëm tekst</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importo tekst pa e formatuar</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importues:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodim:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Zgjidhni importues për t&apos;u përdorur</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Mbaj mend shoqërimin</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Mbaj mend shoqërimin importues-prapashtesë kartele
dhe mos pyet sërish për të përzgjedhur një importues për
kartela të këtij tipi.</translation>
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
        <translation>Të ri Prej Stampe</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Hiq</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Hap</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Tërë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Ngjyra</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Përshkrim</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Përdorim</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Krijuar me</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datë</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Po shkarkoj Stampa</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Stampa dokumentesh mund të gjenden në http://www.scribus.net/ te pjesa Downloads.</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Po instaloj Stampa</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Përfto paketën te drejtoria e stampave ~/.scribus/templates për përdoruesin e çastit ose PREFIX/share/scribus/templates për tërë përdoruesit te sistemi.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Po përgatis stampë</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Sigurohuni që pamjet dhe gërmat që përdorni mund të përdoren lirisht. Nëse gërmat nuk mund të kihen  lirisht i koleksiononi gjatë ruajtjesh stampash.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Krijuesi i stampave do të duhej të siguronte që pjesa Instalim Stampash më sipër vlen për stampat e tyre po aq. Kjo do të thotë që një përdorues të ish në gjendje të shkarkonte një paketë stampash dhe të t&apos;i përftonte ato te drejtoria e stampave dhe të fillonte përdorimin e tyre.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Po heq stampë</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Heqja e një stampe prej dialogut Të ri Prej Stampe do të heqë vetëm zërin prej template.xml, nuk do të fshijë kartelat e dokumentit. Menuja flluckë për heqje shfaqet vetëm nëse keni të drejta shkrimi mbi kartelën
template.xml.</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Po përkthej stampë .xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopjoni një template.xml te një kartelë e quajtur template.lang_COUNTRY.xml (përdorni të njëjtën kod për gjuhën si ai që është i pranishëm te kartela juaj qm për gjuhën), për shembull template.sq.xml për template.xml për gjuhën Shqipe. Kopja duhet të gjendet në të njëjtën drejtori me template.xml origjinale që Scribus-i të mund ta ngarkojë.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Ruaj si Stampë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Emër</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategori</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Ngjyra</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Përshkrim</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Përdorim</translation>
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
        <translation>Më tepër Hollësi</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Më pak Hollësi</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Letër</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>së gjeri</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>portret</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>vetjak</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Krijo filtër</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Pastro</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Fshi</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Zgjidhni një filtër të ruajtur më parë</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Jeoni një emër për ruajtjen e këtij filtri</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Jeoni një emër për ta ruajtur</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Çaktivizoni ose aktivizoni këtë rresht filtri</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Hiq këtë rresht filtri</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Shto një rresht të ri flitri</translation>
    </message>
    <message>
        <source>to</source>
        <translation>te</translation>
    </message>
    <message>
        <source>and</source>
        <translation>dhe</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>hiq përputhje</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>mos hiq përputhje</translation>
    </message>
    <message>
        <source>words</source>
        <translation>fjalë</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Hiq</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Zëvendëso</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Zbato</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Vlera majtas është shprehje e rregullt</translation>
    </message>
    <message>
        <source>with</source>
        <translation>me</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>stil paragrafi</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>tërë hasjet e</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>tërë paragrafët</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>paragrafë që fillojnë me</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>paragrafë me më pak se</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>paragrfë me më shumë se</translation>
    </message>
</context>
</TS>
