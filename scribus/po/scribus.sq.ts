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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">Ngjyrë sfondi:</translation>
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
        <translation type="obsolete">Kufiri</translation>
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
        <translation type="obsolete">Shkallëzim Ndër Diagonal</translation>
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
        <translation type="obsolete">Profil Bazë</translation>
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
        <translation type="obsolete">Jepni URL për t&apos;u shtuar:</translation>
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
        <translation type="obsolete">Rregullimet në Fuqi</translation>
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
        <translation type="obsolete">Ngjyra Teksti</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Ngjyra Teksti</translation>
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
        <translation type="obsolete">&amp;Ndërmjet</translation>
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
        <translation type="obsolete">Rregu&amp;llim Faqeje</translation>
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
        <translation type="obsolete">/Mënyrë Analizuesi/Shtylla</translation>
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
        <translation type="obsolete">Hiq Fundfaqe</translation>
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
        <translation type="obsolete">/Mënyrë Analizuesi/Shtylla</translation>
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
        <translation type="obsolete">Profil Bazë</translation>
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
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame. Text
must be UTF encoded (see setText() as reference) The first character has an
index of 0. &quot;name&quot; If &quot;name&quot; is not given the currently selected Item is
used.

May throw IndexError for an insertion out of bounds.
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
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
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
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
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
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the Layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

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
        <translation type="unfinished"></translation>
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
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
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
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="unfinished">%1. %2 %3 </translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="unfinished">Version Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation type="unfinished">Build-ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation type="unfinished">&amp;Rreth</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation type="unfinished">Pjesëmarrje prej:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="unfinished">Portë Windows:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation type="unfinished">A&amp;utorë</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="unfinished">Çeke:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="unfinished">Daneze:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="unfinished">Finlandeze:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="unfinished">Frënge:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="unfinished">Gjermane:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="unfinished">Italiane:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="unfinished">Norvegjeze:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="unfinished">Polake:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="unfinished">Ruse:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="unfinished">Slovake:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="unfinished">Slovene:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="unfinished">Ukrainase:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="unfinished">Welsh:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="unfinished">Baske:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="unfinished">Braziliane:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="unfinished">Bulgare:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="unfinished">Galike:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="unfinished">Hungareze:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="unfinished">Lituaneze:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="unfinished">Turke:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation type="unfinished">&amp;Përkthime</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="unfinished">Referencë Online</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation type="unfinished">I &amp;lidhur</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="unfinished">Mundësi të Mëtejshme</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="unfinished">Pasqyro Faqe(t) Horizontalisht</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="unfinished">Pasqyro Faqe(t) Vertikalisht</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="unfinished">Zbato Profile &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="unfinished">PostScript Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="unfinished">PostScript Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="unfinished">PostScript Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="unfinished">Krijo PostScript Level 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="unfinished">Krijon vetëm PostScript Level 2, mbani parasysh,
që kjo mund të krijojë kartela të stërmëdha</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="unfinished">Krijon vetëm PostScript Level 1, mbani parasysh,
që kjo mund të krijojë kartela të stërmëdha</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Një rrugë braktisjeje e disa shkallëve të grisë të përbëra nga
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.
UCR prek më tepër pjesë pamjeje  që janë tone neutrale dhe/ose të errët
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje
dhe lipsen ca eksperimente dhe prova për çdo herë.
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="unfinished">Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="unfinished">Drejto</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Horizontalisht</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="unfinished">Anët Majtas</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="unfinished">Programth vetjak vleftësimi:</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="unfinished">Anët Djathtas</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="unfinished">&amp;Ndërmjet</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="unfinished">&amp;Mos Ndrysho</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="unfinished">D&amp;rejto</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="unfinished">Zh&amp;vendoje</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="unfinished">Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Vertikalisht</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="unfinished">Anë të Sipërme</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="unfinished">Anë të Poshtme</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="unfinished">Ndër&amp;mjet</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="unfinished">Mos &amp;Ndrysho</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="unfinished">Dr&amp;ejto</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="unfinished">Zh&amp;vendosje</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="unfinished">Shpërnda/Drejto</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Zbato</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation type="unfinished">Vendos Tekstin Majtas</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation type="unfinished">Vendos Tekstin Djathtas</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation type="unfinished">Vendos Tekstin në Qendër</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation type="unfinished">Vendos Tekstin Përligjshëm</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation type="unfinished">Veti Fushe</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="unfinished">Tip:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation type="unfinished">Buton</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation type="unfinished">Fushë Tekstesh</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation type="unfinished">Kutizë</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation type="unfinished">Listë Kuti</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation type="unfinished">Kuti Liste</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Veti</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Emër:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation type="unfinished">Ndihmëz</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation type="unfinished">Gërma për përdorim me PDF 1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation type="unfinished">Anë:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Ngjyrë:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Gjerësi:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation type="unfinished">E ngushtë</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation type="unfinished">E gjerë</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation type="unfinished">Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation type="unfinished">e Plotë</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation type="unfinished">me Vija</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished">Nënvijë</translation>
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
        <translation type="unfinished">Tjetër</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation type="unfinished">Vetëm Lexim</translation>
    </message>
    <message>
        <source>Required</source>
        <translation type="unfinished">E nevojshme</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation type="unfinished">Mos Eksporto Vlerë</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation type="unfinished">Dukshmëri:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation type="unfinished">I dukshëm</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation type="unfinished">Padukshëm</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation type="unfinished">Pa Shtypje</translation>
    </message>
    <message>
        <source>No View</source>
        <translation type="unfinished">Pa Parje</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation type="unfinished">Dukje</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation type="unfinished">Përpuno tituj për CD-Tekst</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation type="unfinished">Përpuno tituj për CD-Tekst</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation type="unfinished">Ikona</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation type="unfinished">Përdor ikona</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished">Hiq</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation type="unfinished">Rock &amp; Roll</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation type="unfinished">Vendosje Ikone...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation type="unfinished">Thekso</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="unfinished">Përmbys</translation>
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
        <translation type="unfinished">Shumëvijëshe</translation>
    </message>
    <message>
        <source>Password</source>
        <translation type="unfinished">Fjalëkalim</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation type="unfinished">Kufiri</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation type="unfinished">Gërma</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation type="unfinished">Mos Rrëshqit</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation type="unfinished">Mos Kontrollo Drejtshkrimin</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation type="unfinished">Kontrollo Stil:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation type="unfinished">Kontrollo</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation type="unfinished">Shkallëzim Ndër Diagonal</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation type="unfinished">Diamant</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation type="unfinished">Rreth</translation>
    </message>
    <message>
        <source>Star</source>
        <translation type="unfinished">Yllkë</translation>
    </message>
    <message>
        <source>Square</source>
        <translation type="unfinished">Katror</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation type="unfinished">Nëse &quot;widget&quot;-i është &quot;widget&quot;-i parazgjedhje</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation type="unfinished">E përpunueshme</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Mundësi</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="unfinished">Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation type="unfinished">Shko Tek</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation type="unfinished">Parashtro Formular</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation type="unfinished">Rirregullo Formular</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation type="unfinished">Importo të Dhëna</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation type="unfinished">Ngjarje:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation type="unfinished">Ngjitu</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation type="unfinished">Ul Volumin</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation type="unfinished">Jep Fjalëkalimin</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation type="unfinished">Kërkesë Marrëveshjeje Kyçash</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation type="unfinished">Në Fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation type="unfinished">Oshiloskop Turbullt %s</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation type="unfinished">Programth:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation type="unfinished">Përpuno...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation type="unfinished">Jepni URL për t&apos;u shtuar:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation type="unfinished">Parashtro të Dhëna si HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation type="unfinished">Importo të Dhëna prej:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation type="unfinished">Vendmbrritje</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation type="unfinished">Në Kartelë</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="unfinished">Ndërro...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation type="unfinished">Faqe:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="unfinished">X-Poz:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="unfinished">X-Poz:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation type="unfinished">Veprim</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation type="unfinished">Fusha është formatuar si:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation type="unfinished">Thjesht</translation>
    </message>
    <message>
        <source>Number</source>
        <translation type="unfinished">Numër</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation type="unfinished">Përqindje</translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="unfinished">Datë</translation>
    </message>
    <message>
        <source>Time</source>
        <translation type="unfinished">Kohë</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vetiake</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation type="unfinished">Format Numrash</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation type="unfinished">Dhjetore:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation type="unfinished">Përdor Simbol Paraje</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation type="unfinished">Përdor Simbol Paraje</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation type="unfinished">Po formatoj</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation type="unfinished">Format Përqindjesh</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation type="unfinished">Format Date</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation type="unfinished">Format Kohore</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation type="unfinished">Programthe Vetjakë:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation type="unfinished">Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Format</source>
        <translation type="unfinished">Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation type="unfinished">Vlera nuk është vleftësuar</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation type="unfinished">Vlera duhet të jetë më e madhe ose e baraz:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation type="unfinished">dhe më e vogël ose baraz me:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation type="unfinished">Programth vetjak vleftësimi:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation type="unfinished">Vleftëso</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation type="unfinished">Vlera nuk është llogaritur</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation type="unfinished">Vlera është</translation>
    </message>
    <message>
        <source>sum</source>
        <translation type="unfinished">mbledhje</translation>
    </message>
    <message>
        <source>product</source>
        <translation type="unfinished">produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation type="unfinished">mesatare</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation type="unfinished">më i vogël</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation type="unfinished">më i madh</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation type="unfinished">e fushave vijuese:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation type="unfinished">Merr...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation type="unfinished">Programth vetjak llogaritjeje:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation type="unfinished">Llogarit</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation type="unfinished">Flamurka është shpërfillur për PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation type="unfinished">Jepni këtu një listë fushash ndarë me presje</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation type="unfinished">Ju duhet të paktën Ikona për Normal që të përdorni Ikona Për Butona</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="unfinished">Pamje (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation type="unfinished">Shembull:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation type="unfinished">Ndryshim Përzgjedhjeje</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished">Kartela PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation type="unfinished">Veti Shënimesh</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="unfinished">Lidhje</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation type="unfinished">Lidhje e Jashtme</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation type="unfinished">Lidhje Web e Jashtme</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Destination</source>
        <translation type="unfinished">Vendmbrritje</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished">&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation type="unfinished">Dokumente PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="unfinished">Zbato Stampë</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="unfinished">&amp;Stampë:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="unfinished">Zbato te Faqe e Ça&amp;stit</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="unfinished">Zbato te tërë Faqet &amp;çifte</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="unfinished">Zbato te tërë Faqet &amp;teke</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="unfinished">Zbato prej &amp;Faqeje:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="unfinished">Për:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">E papastër</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation type="unfinished">&amp;Ngarko...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Ruaj</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="unfinished">E &amp;vogël</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="unfinished">E &amp;mesme</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation type="unfinished">&amp;Paraparje</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="unfinished">Scrapbooks (*.scs);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished">Riemërto</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="unfinished">Fshij</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="unfinished">Emri &quot;%1&quot; s&apos;është unik.
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Object</source>
        <translation type="unfinished">Objekt</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Emër:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation type="unfinished">Zë i Ri</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation type="unfinished">Faqerojtësa</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation type="unfinished">Lëviz Faqerojtës</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation type="unfinished">Fut Faqerojtës</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation type="unfinished">Faqerojtësa</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation type="unfinished">Vendosje Ikone</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation type="unfinished">Skemë:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation type="unfinished">Vetëm me Ftesa</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation type="unfinished">Vetëm ikona</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation type="unfinished">Tekst dhe Figura</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation type="unfinished">Tekst dhe Figura</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation type="unfinished">Set ikonash për t&apos;u shfaqur</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation type="unfinished">Set ikonash për t&apos;u shfaqur</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation type="unfinished">Tekst dhe Figura</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation type="unfinished">Shkallë:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation type="unfinished">Përherë</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation type="unfinished">Kur një Ikonë është tepër e vogël</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation type="unfinished">Kur një Ikonë është tepër e madhe</translation>
    </message>
    <message>
        <source>Never</source>
        <translation type="unfinished">Kurrë</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation type="unfinished">S&amp;hfaq rishikime</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation type="unfinished">Përpjestimore</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation type="unfinished">Jo Përpjestimore</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation type="unfinished">Ikonë</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation type="unfinished">Rimerr</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="unfinished">Rregullime Administrimi Ngjyrash</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation type="unfinished">&amp;Aktivizo Administrim Ngjyrash</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation type="unfinished">Profile Sistemi</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="unfinished">&amp;Pamje:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation type="unfinished">Ngjyra Teksti</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation type="unfinished">&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation type="unfinished">Sht&amp;ypës:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Koordinata &amp;Absolute</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation type="unfinished">M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation type="unfinished">Sht&amp;ypje:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation type="unfinished">Sim&amp;ulo Shtypës në Ekran</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation type="unfinished">Përdor Radhitjen DNM</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="unfinished">Profil parazgjedhje ngjyrash për pamje të importuara</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation type="unfinished">Profil ngjyrash i prodhuar nga ju apo marrë prej prodhuesit.
Ky profil do të duhej të ishte i veçantë për monitorin tuaj dhe jo një profil i përgjithshëm (bie fjala sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation type="unfinished">Profil ngjyrë për modelin tuaj të shtypësit nga prodhuesi.
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation type="unfinished">Metodë shfaqjeje në ekran ngjyrash të cilat mund të mos shtypen si duhet.
Kjo lyp profile shumë të saktë dhe shërben vetëm si sinjalizim.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation type="unfinished">Kompensim Pikash të Zeza është një metodë për përmirësimin e kontrastit në foto.
Këshillohet ta aktivizoni këtë nëse keni foto në dokumentin tuaj.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation type="unfinished">Përpunoni Ngjyrë</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Emër:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation type="unfinished">&amp;Model Ngjyre</translation>
    </message>
    <message>
        <source>CMYK</source>
        <translation type="unfinished">CMYK</translation>
    </message>
    <message>
        <source>RGB</source>
        <translation type="unfinished">RGB</translation>
    </message>
    <message>
        <source>Web Safe RGB</source>
        <translation type="unfinished">Web Safe RGB</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="unfinished">e Re</translation>
    </message>
    <message>
        <source>Old</source>
        <translation type="unfinished">I vjetër</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation type="unfinished">S&apos;mund të vendos strukturë colormap</translation>
    </message>
    <message>
        <source>C:</source>
        <translation type="unfinished">C:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>M:</source>
        <translation type="unfinished">M:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">Y:</translation>
    </message>
    <message>
        <source>K:</source>
        <translation type="unfinished">K:</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation type="unfinished">/Mënyrë Analizuesi/Shtylla</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation type="unfinished">/Mënyrë Analizuesi/Shtylla</translation>
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
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="unfinished">Emri i Ngjyrës nuk është i vetëm</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Hije:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation type="unfinished">Patejdukshmëri:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation type="unfinished">Shkallëzim Horizontal</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation type="unfinished">Shkallëzim Vertikal</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation type="unfinished">Shkallëzim Diagonal</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation type="unfinished">Shkallëzim Ndër Diagonal</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation type="unfinished">Shkallëzim Radial</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation type="unfinished">Shkallëzim linear i lirë</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation type="unfinished">Shkallëzim rrezor i lirë</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation type="unfinished">X1:</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation type="unfinished">Y1:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation type="unfinished">X2:</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation type="unfinished">Y2:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation type="unfinished">Përpunoni Veti Ngjyre Vije</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation type="unfinished">Përpunoni Veti Ngjyre Mbushëse</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished">Ngopje ngjyre:</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation type="unfinished">Metodë mbushjeje normale ose me shkallëzim</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation type="unfinished">Rregulloni tejdukshmërinë për ngjyrën e përzgjedhur</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="unfinished">Ngjyrë objekti të përzgjedhur</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation type="unfinished">Mundësi Importuesi CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation type="unfinished">Kufizues fushe:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation type="unfinished">(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation type="unfinished">Kufizues vlere:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation type="unfinished">Rreshti i parë është titull</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation type="unfinished">Mundësi Shtypësi</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation type="unfinished">Rregu&amp;llim Faqeje</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation type="unfinished">Tërë Faqet</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation type="unfinished">Vetëm Faqe Çift</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation type="unfinished">Vetëm Fqe Tek</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation type="unfinished">Pasqyro</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Jo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Po</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation type="unfinished">Orientim</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Së gjeri</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation type="unfinished">Po shtyp Dokument...</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation type="unfinished">Faqe për Fletë</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation type="unfinished">Faqe për Fletë</translation>
    </message>
    <message>
        <source>Option</source>
        <translation type="unfinished">Mundësi</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Vlerë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="unfinished">Ky panel paraqet mundësi të ndryshme CUPS kur shtypet. 
Parametrat e saktë të mundshëm do të varen nga &quot;driver&quot;-i i shtypësit tuaj.
Mund të keni suport CUPS duke përzgjidhur Ndihmë &gt; Rreth.
Shih për përmendje të: C-C-T Kjo është e barazvlefshme me C=CUPS C=littlecms T=TIFF suport.
Mungesa e suportit për librarinë tregohet me një *</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation type="unfinished">&amp;Ngjesh Kartelë:</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation type="unfinished">&amp;Përfshi Gërma</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation type="unfinished">Kodim:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation type="unfinished">Shkon te Drejtori juaja Dokumentesh.
Kjo mund të rregullohet te Parapëlqimet.</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation type="unfinished">Fshij Ngjyrë</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="unfinished">Fshij ngjyrë:</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="unfinished">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="unfinished">Zëvendësoje me</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation type="unfinished">Fshij Faqe</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="unfinished">Fshij prej:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="unfinished">për:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="unfinished">Gërma që Mungojnë</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished">Gërmat %1 nuk janë instaluar.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished">Përdor</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished">më mirë</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Të dhëna Dokumenti:</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation type="unfinished">&amp;Titull:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation type="unfinished">&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation type="unfinished">&amp;Fjalëkyça:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation type="unfinished">Pë&amp;rshkrim:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation type="unfinished">B&amp;otues:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation type="unfinished">&amp;Pjesëmarrës:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation type="unfinished">Dat&amp;ë:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation type="unfinished">Ti&amp;p:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation type="unfinished">F&amp;ormat</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation type="unfinished">Identi&amp;fikues:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation type="unfinished">&amp;Burim:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Gjuhë:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation type="unfinished">&amp;Relacion:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation type="unfinished">Mb&amp;ulim</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation type="unfinished">Të &amp;drejta</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="unfinished">&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation type="unfinished">Të &amp;dhëna të Mëtejshme</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished">Personi apo organizmi përgjegjës fillimisht për hartimin e përmbajtjes së dokumentit.
Kjo fushë mund të trupëzohet në dokumente Scribus për referencë, si dhe në metadatat e një PDF-je</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="unfinished">Emër i dhënë dokumentit.
Kjo fushë mund të trupëzohet në dokumente Scribus për referencë, dhe edhe në metadatën e një PDF-je</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="unfinished">Një përshkrim i përmbajtjes së dokumentit.
Roli i kësaj fushe është një përshkrim të shkurtër ose abstrakt i dokumentit. Gjatë eksportimit trupëzohet në PDF-në</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="unfinished">Tema e përmbajtjes së dokumentit.
Kjo fushë është për fjalëkyça dokumenti që doni të trupëzohen në një PDF, për të ndihmuar kërkime dhe indexim kartelash PDF</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation type="unfinished">Person ose organizëm përgjegjës për pasjen e dokumentit të mundshëm</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation type="unfinished">Person ose organizëm përgjegjës për pasje pjesëmarrjeje në përmbajtjen e dokumentit</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation type="unfinished">Datë e shoqëruar një ngjarjeje në jetën e dokumentit, në format VVVV-MM-DD, si në ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation type="unfinished">Natyra ose zhanri i përmbajtjes së dokumentit, psh. kategori, funksione, zhanre, etj</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="unfinished">Paraqitja fizike ose numerike e dokumentit. Do të ishte me leverdi përmendja e tipit të medias dhe përmasat.
Gjithashtu të dobishme janë RFC2045, RFC2046 për tipe MIME</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation type="unfinished">Një referencë jo e dykuptimtë te dokumenti brenda një konteksti të caktuar si p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation type="unfinished">Një referencë te një dokument prej të cilit dokumenti i pranishëm ka rrjedhur, p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="unfinished">Gjuha në të cilën përmbajtja e dokumentit është shkruajtur, zakonisht kod ISO-639 gjuhe
mundet edhe ndarë me vizë nga prapashtesë kodi ISO-3166 vendi, p.sh. en-GB, fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation type="unfinished">Një referencë te një dokument i lidhur, mundësisht duke përdorur një identifikues formal si p.sh. ISBN ose URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation type="unfinished">Shtrirja ose fokusi i përmbajtjes së dokumentit, mundësisht duke përfshirë vendndodhje, kohe dhe shtrirje juridike</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation type="unfinished">Të dhëna rreth të drejtash të ruajtura rreth dhe mbi dokumentin, p.sh të drejta kopjimi, patenta ose shenja tregtare</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation type="unfinished">Rregulloni Shtypës:</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation type="unfinished">Destinacion Shtypësi</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Kartelë</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation type="unfinished">&amp;Mundësi...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation type="unfinished">&amp;Kartelë:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation type="unfinished">Urdhër &amp;Alternativ Shtypësi</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="unfinished">Përdorni një përgjegjës shtypjesh alternativ, si kprinter ose gtklp,
për të bërë të përdorshëm mundësi shtypjesh shtesë</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation type="unfinished">Ur&amp;dhër:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation type="unfinished">Interval</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation type="unfinished">Shtyp &amp;Tërë</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation type="unfinished">Shtyp Fa&amp;qen e Çastit</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation type="unfinished">&amp;Interval Shtypjeje</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished">Jepni një listë objektesh të ndarë me presje ku
objekti mund të jetë * për tërë faqet, 1-5 për
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation type="unfinished">N&amp;umër Kopjesh</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Mundësi</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="unfinished">Shtypje &amp;Normale</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="unfinished">&amp;Veçime Shtypjeje</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tërë</translation>
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
        <translation type="unfinished">Yellow</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Zi</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="unfinished">Sht&amp;yp Me Ngjyra Nëse Mundet</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="unfinished">Shtyp Me Shkallë të &amp;Grisë</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="unfinished">Mundësi të Mët&amp;ejshme...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation type="unfinished">&amp;Shtyp</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Ruaj  si</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="unfinished">Kartela Postscript (*.ps);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation type="unfinished">Importimi i Kartelës:
%1
dështoi!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="unfinished">Gabim Fatal</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation type="unfinished">Përpunoni Stil</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Emër:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="unfinished">Gërma</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="unfinished">&amp;Gërma:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="unfinished">Madhë&amp;si:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="unfinished">Efekt:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="unfinished">&amp;Drejtim:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="unfinished">&amp;Ndërmjet</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation type="unfinished">&amp;Rreshta:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished">Ngjyrë sfondi:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="unfinished">Ndërro Ngjyrën e Sfondit</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="unfinished">Hapësira Vertikale</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="unfinished">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="unfinished">Si&amp;për:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="unfinished">P&amp;oshtë:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation type="unfinished">&amp;Krijo dhe Ndrysho</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation type="unfinished">Emër i stilit tuaj për paragrafin</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation type="unfinished">Gërma për tekstin apo objektin e përzgjedhur</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="unfinished">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Ndërro Ngjyrën e Tekstit</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="unfinished">Mundëson një gërmë të stërmadhe për gërmën e parë të një paragrafi. E përdorur për efekte stilistikë</translation>
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
        <translation type="unfinished">Hapësirë sipër paragrafit</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation type="unfinished">Hapësirë poshtë paragrafit</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation type="unfinished">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="unfinished">Emri i Stilit nuk është i vetëm</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation type="unfinished">Përpunues</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Hap...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation type="unfinished">&amp;Ruaj dhe Dil</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation type="unfinished">&amp;Dil pa Ruajtur</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Ribëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">P&amp;rij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation type="unfinished">&amp;Merr Emra Fushe</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="unfinished">Javascripts (*.js);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation type="unfinished">Zgjidhni Drejtori Eksportimi</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation type="unfinished">Eksporto si Pamje</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation type="unfinished">&amp;Eksporto te Drejtori:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Mundësi</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation type="unfinished">T&amp;ip Pamjeje: </translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished">&amp;Cilësi:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="unfinished">&amp;Qartësi:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Madhësi:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>Range</source>
        <translation type="unfinished">Interval</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation type="unfinished">Faqe e çastit</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished">&amp;Tërë faqet</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation type="unfinished">&amp;Interval</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation type="unfinished">Eksportoni një interval faqesh</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished">Jepni një listë objektesh të ndarë me presje ku
objekti mund të jetë * për tërë faqet, 1-5 për
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation type="unfinished">Eksportoni tërë faqet</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation type="unfinished">Eksportoni vetëm faqen e çastit</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation type="unfinished">Qartësi Pamjesh
Përdorni 72 dpi për Pamje të paramenduara për Ekranin</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation type="unfinished">Cilësia e pamjeve tuaja - 100% është më e mira, 1% cilësia më e ulët</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation type="unfinished">Formate të mundshëm eksporti</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation type="unfinished">Ndrysho drejtori përfundimesh</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Madhësi:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation type="unfinished">Titull:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation type="unfinished">Pa Titull</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation type="unfinished">Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">E panjohur</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation type="unfinished">Dokument Scribus</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Ngjyra</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished">Hiq Fundfaqe</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="unfinished">Sete Ngjyrash</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="unfinished">Set Ngjyrash i Çastit:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished">&amp;Ruaj Set Ngjyrash</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="unfinished">Zgjidhni një set njgyrash për t&apos;u ngarkuar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="unfinished">Ruaj setin e çastit për ngjyrat</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished">Hiqni ngjyra të papërdorura prej seti ngjyrash të dokumentit të çastit</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="unfinished">Vini ngjyra prej një dokumenti ekzistues te seti i çastit</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="unfinished">Krijoni ngjyrë të re brenda setit të çastit</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="unfinished">Përpunoni ngjyrën e përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="unfinished">Bëni një kopje të ngjyrës së përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="unfinished">Fshij ngjyrën e përzgjedhur për çastin</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="unfinished">Bëj set ngjyrash të çastit set parazgjedhje të çastit</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Emër:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="unfinished">Zgjidhni Emër</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopje e %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="unfinished">Ngjyrë e Re:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="unfinished">Rregullime të Përgjithshme Gërmash</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation type="unfinished">Gërma të Mundshme</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation type="unfinished">Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation type="unfinished">Shtigje Shtesë</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Po</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation type="unfinished">Emër Gërmash</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="unfinished">Përdor Gërma </translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="unfinished">Trupëzo në:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="unfinished">Nënset</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Tip</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="unfinished">Shteg për te Kartelë Gërmash</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation type="unfinished">Gërma të &amp;Mundshme</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation type="unfinished">Zëvendësim</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation type="unfinished">&amp;Zëvendësim Gërmash</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation type="unfinished">Sht&amp;o...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;Hiq</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation type="unfinished">&amp;Shtigje Shtesë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Zgjidhni një Drejtori</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="unfinished">Paraparje Gërmash</translation>
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
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation type="unfinished">Pozicion:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation type="unfinished">Administroni Udhëzuesa</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation type="unfinished">Udhëzuesa Horizontalë</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished">&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">&amp;Shto</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation type="unfinished">Fshi&amp;j</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation type="unfinished">Udhëzuesa Vertikalë</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished">&amp;X-Poz:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation type="unfinished">Sht&amp;o</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation type="unfinished">Fshi&amp;j</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation type="unfinished">&amp;Kyç Udhëzuesa</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="unfinished">Lidhje</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation type="unfinished">Ndarje me Vizë e Mundshme</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation type="unfinished">Prano</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation type="unfinished">Anashkalo</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="unfinished">Rregullimet në Fuqi</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Gjuhë:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation type="unfinished">Fjala më e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation type="unfinished">Gjatësi e fjalës më të vogël për ta ndarë me vijë.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation type="unfinished">Numër më i madh Ndarjesh me Vizë që njekin njëra tjetrën.
Një vlerë 0 do të thotë ndarje me vizë të pakufizuara.</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation type="unfinished">Fut Faqe</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="unfinished">&amp;Po fus</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation type="unfinished">Faqe</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="unfinished">para Faqes</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="unfinished">pas Faqes</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="unfinished">në Fund</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="unfinished">Po fus</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="unfinished">&amp;Stampë (Faqe Majtas):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="unfinished">&amp;Stampë:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="unfinished">Stampë (Faqe Djathtas):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished">Fut Tabelë</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="unfinished">Numri i Rreshtave:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="unfinished">Numri i Shtyllave:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>Edit JavaScripts</source>
        <translation type="unfinished">Përpuno JavaScript</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="unfinished">&amp;Përpuno...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation type="unfinished">&amp;Shto...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation type="unfinished">Programth i &amp;Ri</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation type="unfinished">Programth i Ri</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Po</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="unfinished">Administro Shkurtprerje Tastiere</translation>
    </message>
    <message>
        <source>Action</source>
        <translation type="unfinished">Veprim</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation type="unfinished">Kyç i Çastit</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation type="unfinished">Përzgjidhni një Kyç për këtë Veprim</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation type="unfinished">&amp;Pa Tast Prishkurt</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation type="unfinished">Taste të Përcaktuar nga &amp;Përdoruesi</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation type="unfinished">ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation type="unfinished">Cakto &amp;Kyç</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
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
        <translation type="unfinished">Shift</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation type="unfinished">Shift+</translation>
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
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="unfinished">Kjo Radhë Tastesh është në përdorim</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation type="unfinished">SHtresa</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="unfinished">Shto Shtresë të re</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation type="unfinished">Fshij Shtresë</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="unfinished">Ngri Shtresë</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="unfinished">Ul Shtresë</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="unfinished">Shtresë e Re</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="unfinished">Doni të fshini tërë Objektet edhe në këtë Shtresë?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation type="unfinished">Përpunoni Stile Vije</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopje e %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Stil i Ri</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Po</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="unfinished">Dyfisho CD</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation type="unfinished">&amp;Numër Kopjesh:</translation>
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
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation type="unfinished">Largësira</translation>
    </message>
    <message>
        <source>X1:</source>
        <translation type="unfinished">X1:</translation>
    </message>
    <message>
        <source>Y1:</source>
        <translation type="unfinished">Y1:</translation>
    </message>
    <message>
        <source>X2:</source>
        <translation type="unfinished">X2:</translation>
    </message>
    <message>
        <source>Y2:</source>
        <translation type="unfinished">Y2:</translation>
    </message>
    <message>
        <source>DX:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DY:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle:</source>
        <translation type="unfinished">Kënd:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation type="unfinished">Gjatësi:</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="unfinished">Gabim programthi</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Shfaq &amp;Konsol</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="unfinished">Fshih &amp;Konsol</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation type="unfinished">Importoni Stampë</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="unfinished">Importoni Faqe(t)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="unfinished">Prej Dokumenti:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="unfinished">Ndërro...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="unfinished">Importoni Faqe(t):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished">Jepni një listë objektesh të ndarë me presje ku
objekti mund të jetë * për tërë faqet, 1-5 për
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation type="unfinished">prej 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation type="unfinished">Krijoni Faqe(t)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="unfinished">para Faqes</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="unfinished">pas Faqes</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="unfinished">në Fund</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="unfinished">Importo</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation type="unfinished">nga %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation type="unfinished">Zhvendos Faqe</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation type="unfinished">Kopjo Faqe</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation type="unfinished">Zhvendos Faqe:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="unfinished">për:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="unfinished">para Faqes</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="unfinished">pas Faqes</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="unfinished">në Fund</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Veti</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation type="unfinished">X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Formë</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation type="unfinished">&amp;Tekst</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation type="unfinished">&amp;Pamje</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished">&amp;Rresht</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation type="unfinished">&amp;Ngjyra</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Emër</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation type="unfinished">Gjeometri</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished">&amp;X-Poz:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished">&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">Nivel</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation type="unfinished">Formë</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished">&amp;Përpuno Formë...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation type="unfinished">K&amp;ulme
të Rrumbullakët:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation type="unfinished">Largësi e Tekstit</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">&amp;Boshllëk</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation type="unfinished">Si&amp;për:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Djathtas:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation type="unfinished">Veti Teksti Shtegu</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation type="unfinished">Shfaq Kurbë</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation type="unfinished">Degëzim Vertical</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation type="unfinished">Largësi prej Kurbe:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation type="unfinished">Teksti &amp;Rrjedh Rreth Kornizës</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation type="unfinished">Kopjo Kuti Teksti</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation type="unfinished">Përpunoni &amp;Rrethina Vije</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="unfinished">&amp;Madhësi Gërmash:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Hije:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="unfinished">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="unfinished">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation type="unfinished">St&amp;il:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation type="unfinished">Gj&amp;uhë:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished">Ripërmasim i &amp;Lirë</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation type="unfinished">X-Shk&amp;allë:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation type="unfinished">Y-Shkall&amp;ë:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation type="unfinished">Ripërmaso &amp;Sa Madhësi Kornize</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation type="unfinished">P&amp;ërpjestimor</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation type="unfinished">Profil Bazë</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Koordinata &amp;Absolute</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation type="unfinished">Shtylla &amp;Majtas</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation type="unfinished">Pikë gërmash</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation type="unfinished">T&amp;ip Vije:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">&amp;Gjerësi Vije:</translation>
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
        <translation type="unfinished">S&amp;kaje:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation type="unfinished">Majë e Sheshtë</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation type="unfinished">Majë Katrore</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation type="unfinished">Majë e Rrumbullakët</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation type="unfinished">&amp;Funde:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Pa Stil</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation type="unfinished">Vija Kutie</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation type="unfinished">Vijë Sipër</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation type="unfinished">Vijë Majtas</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation type="unfinished">Vijë Djathtas</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation type="unfinished">Vijë Poshtë</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation type="unfinished">Emër i objekti të përzgjedhur</translation>
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
        <translation type="unfinished">Gjerësi</translation>
    </message>
    <message>
        <source>Height</source>
        <translation type="unfinished">Lartësi</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation type="unfinished">Pikë së cilës i referehon matjet ose këndet e rrotullimeve</translation>
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
        <translation type="unfinished">Horizontalisht</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation type="unfinished">Vertikalisht</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation type="unfinished">Ngrije një nivel</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation type="unfinished">Zbrite një nivel</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation type="unfinished">Zhvendos përpara</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation type="unfinished">Zhvendos prapa</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation type="unfinished">Tregon nivelin në të cilin gjendet objekti, 0 do të thotë që objekti është në fund fare</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation type="unfinished">Kyç ose çkyç objektin</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation type="unfinished">Kyç ose çkyç madhësinë e objektit</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation type="unfinished">Aktivizo ose çaktivizo shtypjen e objektit</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation type="unfinished">Bëj tekst nga kornizat e poshtme të rrjedhin përreth formës së objektit</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation type="unfinished">Përdor kuti rrethuese në vend të formës së kornizës për rrjedhë teksti</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation type="unfinished">Përdor një vijë të dytë bazuar fillimisht në formën e kornizës për rrjedhë teksti</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation type="unfinished">Gërma për tekstin apo objektin e përzgjedhur</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished">Gërma (pa hapësira):</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="unfinished">Ndërro Ngjyrën e Tekstit</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="unfinished">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation type="unfinished">&amp;Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="unfinished">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation type="unfinished">Ndërvijë Dyshe</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation type="unfinished">Stil për paragrafin e çastit</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation type="unfinished">Tekst i etiketës së kornizës</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation type="unfinished">Ndrysho rregullime për pika majtas ose në fund </translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation type="unfinished">Skemë vije</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation type="unfinished">Trashësi vije</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation type="unfinished">Tip bashkimi vije</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation type="unfinished">Tip fundi vije</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation type="unfinished">Stil vije e objektit të çastit</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation type="unfinished">Zgjidhni formën e kornizës...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation type="unfinished">Përpunoni formën e kornizës...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation type="unfinished">Caktoni rrezen e lakimit të kulmit</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation type="unfinished">Numër shtyllash në kornizë teksti</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation type="unfinished">Largësi ndërmjet shtyllash</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation type="unfinished">Largësi teksti nga kreu i kornizës</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation type="unfinished">Largësi teksti nga fundi i kornizës</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation type="unfinished">Largësi teksti nga e majta e kornizës</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation type="unfinished">Largësi teksti nga e djathta e kornizës</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation type="unfinished">Përpunoni rregullimet tab për kornizë tekstesh...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation type="unfinished">Lejoje pamjen të ketë tjetër madhësi në kornizë</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation type="unfinished">Shmangje horizontale e pamjes brenda kornizës</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation type="unfinished">Shmangje vertikale e pamjes brenda kornizës</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation type="unfinished">Ripërmaso pamjen jorizontalisht</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation type="unfinished">Ripërmaso pamjen vertikalisht</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation type="unfinished">Mbaj ndryshim të X dhe Y njësoj</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation type="unfinished">Ruaj përpjestim</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation type="unfinished">Bëje pamjen të hyjë brenda madhësisë së kornizës</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation type="unfinished">Përdor përpjestimet e pamjes më mirë se ata të kornizës</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation type="unfinished">Profil burim i pamjes</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation type="unfinished">Të dhëna titujsh për pamje ISO9660 </translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation type="unfinished">&amp;X1:</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation type="unfinished">X&amp;2:</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation type="unfinished">Y&amp;1:</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation type="unfinished">&amp;Y2:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column width</source>
        <translation type="unfinished">Gjerësi shtylle</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="unfinished">Emri &quot;%1&quot; s&apos;është unik.
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation type="unfinished">Përpunoni Stil</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation type="unfinished">Majë e Sheshtë</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation type="unfinished">Majë Katrore</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation type="unfinished">Majë e Rrumbullakët</translation>
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
        <translation type="unfinished">Gjerësi Vije:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source> pt </source>
        <translation type="unfinished"> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation type="unfinished">Vijë e Plotë</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation type="unfinished">Vijë e Ndërprerë</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation type="unfinished">Vijë me Pika</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation type="unfinished">&amp;Hiq numra rreshtash</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation type="unfinished">&amp;Hiq numra rreshtash</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="unfinished">Emri &quot;%1&quot; s&apos;është unik.
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="unfinished">Përpuno Stampa</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Jo</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Po</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Emër:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="unfinished">Stampë e Re</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopje e %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Emër:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopjo #%1 nga</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Document</source>
        <translation type="unfinished">Dokument i Ri</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Madhësi Faqeje</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Madhësi:</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Letër</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished">Tabloid</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vetiake</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntim:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Së gjeri</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="unfinished">Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="unfinished">&amp;Faqe Majtas fillimisht</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Djathtas:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Poshtë:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Mundësi</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished">Numër i Faqes së Parë</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation type="unfinished">Njësi &amp;Parazgjedhje</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="unfinished">Pikë (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="unfinished">Milimetra (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Inç (inç)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Pikas (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation type="unfinished">Korniza Teksti &amp;Vetvetiu</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="unfinished">Udhëzuesa Shtylle</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">&amp;Boshllëk</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation type="unfinished">Madhësi faqeje dokumenti, ose si madhësi standard ose si madhësi vetjake</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation type="unfinished">Drejtim i faqeve të dokumentit</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Gjerësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Lartësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="unfinished">Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="unfinished">Numër për faqen e parë të dokumentit</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished">Njësi matëse parazgjedhje për përpunime dokumentesh</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation type="unfinished">Krijo vetvetiu korniza teksti kur shtohen faqe të reja</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation type="unfinished">Numër shtyllash për t&apos;u krijuar në korniza teksti të krijuara vetvetiu</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation type="unfinished">Largësi ndërmjet shtyllash të krijuara vetvetiu</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">&amp;Brenda:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">J&amp;ashtë:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="unfinished">Faqe Majtas</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="unfinished">Faqe Djathtas</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation type="unfinished">Nyje</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation type="unfinished">Koordinata &amp;Absolute</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation type="unfinished">&amp;X-Poz:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation type="unfinished">&amp;Y-Poz:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation type="unfinished">Përpunoni &amp;Rrethina Vije</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation type="unfinished">Përpunoni &amp;Rrethina Vije</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation type="unfinished">&amp;Përfundo Përpunim</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation type="unfinished">Zhvendos Nyje</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation type="unfinished">Kontroll &amp;Jetim/E Ve</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation type="unfinished">Shto Nyje</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation type="unfinished">Fshij Nyje</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation type="unfinished">Lëviz Pika Kontrolli në Mënyrë të Pavarur</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation type="unfinished">Lëviz Pika Kontrolli Simetrikisht</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation type="unfinished">Kontroll &amp;Jetim/E Ve</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation type="unfinished">Rirregulloni këtë Pikë Kontrolli</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation type="unfinished">Hap një Shumëkëndësh ose Pret një Kurbë Bezier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation type="unfinished">Mbyll këtë Kurbë Bezier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation type="unfinished">Pasqyro Shtegun Horizontalisht</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation type="unfinished">Pasqyro Shtegun Vertikalisht</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation type="unfinished">Qeth Shtegun Horizontalisht Majtas</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation type="unfinished">Qeth Shtegun Vertikalisht Majtas</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation type="unfinished">Qeth Shtegun Vertikalisht Sipër</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation type="unfinished">Qeth Shtegun Vertikalisht Poshtë</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation type="unfinished">Rrotullo Shtegun në Kah Antiorar</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation type="unfinished">Rrotullo Shtegun në Kah Orar </translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation type="unfinished">Kënd Rrotullimi</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="unfinished">Zvogëlo ose zmadho shfaqjen e dokumentit </translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="unfinished">Kur zgjidhet përdoren Koordinata relative te Faqja,
përndryshe Koordinatat janë relative te Objekti.</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation type="unfinished">Konsol Programthesh</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Create PDF File</source>
        <translation type="unfinished">Kroji Kartelë PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="unfinished">Për&amp;fundimi në Kartelë:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">N&amp;dryshoni...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Eksporto Interval</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">&amp;Tërë Faqet</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">Z&amp;gjidhni Faqe</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">Mundësi Kartele</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">Përp&amp;uthshmëri:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Mënjanë Majtas</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Mënjanë Djathtas</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Prodho bazë të dhënash</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Ruaj Korniza të &amp;Lidhura Tekstesh si Artikuj PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">&amp;Përfshi Faqerojtësa</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="unfinished">&amp;Qartësi:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="unfinished">Rregullime Pamjeje</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Automatike</translation>
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
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="unfinished">&amp;Metodë:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished">&amp;Cilësi:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Më i madhi</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">Lartë</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Mesatare</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Ulët</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Më i vogli</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="unfinished">Shto tek stampë</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">&amp;Të përgjithshme</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">&amp;Trupëzo tërë Gërmat</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Gërma të Mundshme:</translation>
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
        <translation type="unfinished">Gërma për trupëzim</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Gërma</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Faqe</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">Shfaq P&amp;araparje Faqesh</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Efekte</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">&amp;Shfaq Kohëzgjatje:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Kohëzgjatje Efek&amp;ti:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">T&amp;ip Efekti:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">Po zh&amp;vendos Rreshta:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">P&amp;rej:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">Drejt&amp;im:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished">sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">Pa Efekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="unfinished">Kuti</translation>
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
        <translation type="unfinished">Ndaj</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Horizontalisht</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Vertikalisht</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Brenda</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Jashtë</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Nga e Majta në të Djathtë</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Nga Sipër Poshtë:</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Nga Poshtë Sipër</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Nga e Djathta në të Majtë</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Nga Sipër-majtas Poshtë-djathtas</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">&amp;Zbato Efekt te tërë Faqet</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">&amp;Përdor Kodim</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Fjalëkalime</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">&amp;Përdorues:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">Pr&amp;onar:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Rregullime</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">Lejo &amp;Shtypjen e Dokumentit</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Lejo &amp;Ndryshimin e Dokumentit</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Lejo Kop&amp;jim Tekstesh dhe Grafikash</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Lejo Shtim Shëni&amp;mesh dhe Fushash</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">S&amp;iguri</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Të përgjithshme</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished">&amp;Kërko për Ndihmë</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="unfinished">Ekran / Web</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Shtypës</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished">Përdor Rregullime të Përgjithshme &quot;Proxy&quot;</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished">Rregullimet në Fuqi</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished">Shpe&amp;Sti:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished">&amp;Kënd:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">Funksion i panjohur</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished">Pikë e Thjeshtë</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Rresht</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished">e Rrumbullakët</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished">Elips</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="unfinished">Ngjyra Teksti</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished">Përdor Profil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="unfinished">Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Ngopje</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Koordinata &amp;Absolute</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="unfinished">Pamje:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished">Mos përdor profile ICC të trupëzuar</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">Ngj&amp;yrë</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">Ruajtje përfundimesh e suksesshme</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished">Të &amp;dhëna Varg:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished">Profil Bazë</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="unfinished">Listë Kuti</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Eksporto tërë faqet si PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Eksporto interval faqesh si PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished">Jepni një listë objektesh të ndarë me presje ku
objekti mund të jetë * për tërë faqet, 1-5 për
një interval faqesh ose një numër faqeje të vetme.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="unfinished">Përcakton përputhshmërinë me. Parazgjedhja është Acrobat 4.0 çka ju jep përputhshmërinë më të madhe.
Zgjidhni Acrobat 5.0 nese kartela juaj ka karakteristika PDF 1.4 si p.sh. tejdukshmëri ose nëse keni nevojë për kodim 128 bit.
PDF/X-3 vlen për eksportim PDF-sh për shtypje komerciale dhe është i përzgjedhshëm kur keni veprues administrim ngjyrash.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished">Përcakton lidhjen e faqeve te PDF-ja. Lini parazgjedhjen - Majtas,
veç në e ditshi që keni nevojë ta ndryshoni.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Prodho Artikuj PDF, çka është e dobishme për lundrim artikujsh në një PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="unfinished">Trupëzo në dokumetin tuaj faqerojtësit e krijuar.
Këta janë të dobishëm për të udhëtuar në dokumente të gjatë PDF.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Shfaq paraparje faqeje për secilën nga faqet e listuara më sipër. </translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished">Kohëzgjatja e xhirimit të efektit.
Një kohë më e shkurtër do ta përshpejtojë efektin, një më e gjatë do ta ngadalësojë.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Tip i efektit të ekranit.</translation>
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
        <translation type="unfinished">Zbato efektet e përzgjedhur te të tëra faqet.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished">Aktivizoni veçori sigurie për PDF tuajat të eksportuara.
Nëse përzgjodhët Acrobat 4.0, PDF-ja do të mbrohet me kodim 40 bit.
Nëse përzgjodhët Acrobat 5.0, PDF-ja do të mbrohet me kodim 128 bit.
Disclaimer: Kodimi PDF nuk është aq i besueshëm sa kodimi GPG ose PGP dhe ka ca kufizime.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="unfinished">Zgjidhni fjalëkalim të përgjithshëm i cili aktivizon ose çaktivizon tërë
veçoritë sigurisë për PDF tuajat të eksportuara</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Zgjidhni një fjalëkalim i cili t&apos;u lejojë përdoruesve të lexojnë PDF-të tuaja.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Lejo shtypjen e PDF-së. Nëse nuk shenjohet, parandalohet shtypja.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Lejo ndryshimin e PDF-së. Nëse nuk shenjohet, parandalohet ndryshimi i PDF-së.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="unfinished">Lejo kopjim teksti apo grafike prej PDF-së. 
Nëse nuk shenjohet, nuk mund të kopjohet tekst apo grafikë.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="unfinished">Lejo shtim shënimesh dhe fushash te PDF-ja. 
Nëse nuk shenjohet, parandalohet përpunimi i shënimeve dhe fushave.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">Ky është një rregullim i cili si parazgjedhje nuk është veprues. Do të duhej të aktivizohej
kur kërkohet veçanërisht nga shtypësi juaj dhe ju kanë dhënë saktë hollësitë e nevojshme.
Përndryshe, PDF-ja juaj që eksportuat mund të mos shtypet si pritet dhe të jetë vërtet i kalueshëm nëpër sisteme.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished">Trupëzo profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="unfinished">Trupëzo profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished">Mos përdor profile ngjyrash që genden të trupëzuar në burimin e pamjes</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished">Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">Profil ngjyrash për pamjet</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Vargu i detyrueshëm për PDF/X-3 ose PDF do të dalë
përputhjes me PDF/X-3. Ju këshillojmë të përdorni titullin e dokumentit.</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Ruaj  si</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished">Kartela PDF (*.pdf);;Tërë Kartelat (*)</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation type="unfinished">Paraparje Shtypjeje</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation type="unfinished">Anti-alias &amp;Tekst</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation type="unfinished">Anti-alias &amp;Grafikë</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation type="unfinished">Shfaq Tej&amp;dukshmëri</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation type="unfinished">Heqje Ngjyre &amp;Nën Të</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation type="unfinished">&amp;Shfaq CMYK</translation>
    </message>
    <message>
        <source>&amp;C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Y</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;K</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation type="unfinished">Shfaq tejdukshmëri dhe objekte të tejdukshëm në dokumentin tuaj. Lyp Ghostscript 7.07 ose të mëvonshëm</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation type="unfinished"></translation>
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
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Një rrugë braktisjeje e disa shkallëve të grisë të përbëra nga
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.
UCR prek më tepër pjesë pamjeje  që janë tone neutrale dhe/ose të errët
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje
dhe lipsen ca eksperimente dhe prova për çdo herë.
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tërë</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="unfinished">Kopjo Këtu</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="unfinished">Zhvendos Këtu</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Ngjit</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished">Shfaq &amp;Mënjana</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished">Shfaq &amp;Korniza</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished">Shfaq &amp;Pamje</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished">Shfaq &amp;Udhëzuesa</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
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
        <source>Picture</source>
        <translation type="unfinished">Pamje</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="unfinished">Kartelë:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">PPI Fillestar: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">PPI e Tanishme: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="unfinished">Tekst i Lidhur</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="unfinished">Kornizë Teksti</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Tekst sipas një Shtegu</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished">Paragrafë:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished">Fjalë:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished">Gërma:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="unfinished">Shtyp:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="unfinished">Veprues</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Çaktivizuar</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished">Të dhë&amp;na</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="unfinished">&amp;Merr Pamje...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished">Pa&amp;mje e Dukshme</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="unfinished">&amp;Përditëso Pamje</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="unfinished">&amp;Përpunoni Pamje</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="unfinished">&amp;Përshtatja Kornizën Pamjes</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="unfinished">&amp;Merr Tekst...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="unfinished">&amp;Vini Tekst...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="unfinished">&amp;Përpuno Tekst...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="unfinished">Është &amp;Faqerojtës PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished">Është Sh&amp;ënim PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished">V&amp;eti Shënimesh...</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished">&amp;Veti Fushe</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">Mundësi &amp;PDF</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="unfinished">Përpuno Tekst...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="unfinished">&amp;Blloko</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="unfinished">&amp;Çblloko </translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="unfinished">Kyç &amp;Madhësi Objekti</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="unfinished">Çkyç &amp;Madhësi Objekti</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished">Dërgoje në të Papastër</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Dërgo te Sh&amp;tresë</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="unfinished">&amp;Fut Shembull Teksti</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">&amp;Grup</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="unfinished">Ç&amp;grupo</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished">Ni&amp;vel</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="unfinished">Shpjere &amp;Prapa</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="unfinished">Bjer &amp;Përpara</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="unfinished">&amp;Ul</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished">&amp;Ngri</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="unfinished">Kornizë &amp;Pamjeje</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="unfinished">Shumë&amp;këndësh</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">Kornizë &amp;Teksti</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished">Kurbë &amp;Bezier</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished">Shn&amp;dërro si </translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">P&amp;rij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished">Pa&amp;stro Përmbajtje</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="unfinished">Shfaq &amp;Veti...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="unfinished">Fshih &amp;Veti...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="unfinished">Doni vërtet të pastroni tërë Tekstin tuaj?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="unfinished">Programi</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="unfinished">mungon!</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Kopje e</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation type="unfinished">Pamje</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Rresht</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Shumëkëndësh</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Shumëvijë</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="unfinished">Tekst Shtegu</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="unfinished">Faqe</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="unfinished"> e %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Result</source>
        <translation type="unfinished">Përfundim</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation type="unfinished">Përfundime Kërkimi për:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Paraparje</translation>
    </message>
    <message>
        <source>Select</source>
        <translation type="unfinished">Përzgjidhni</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation type="unfinished">Grafikë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Emër</translation>
    </message>
    <message>
        <source>Path</source>
        <translation type="unfinished">Shteg</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Faqe</translation>
    </message>
    <message>
        <source>Print</source>
        <translation type="unfinished">Shtyp</translation>
    </message>
    <message>
        <source>Status</source>
        <translation type="unfinished">Gjendje</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation type="unfinished">Shko te</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Po</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation type="unfinished">Mungon</translation>
    </message>
    <message>
        <source>Search</source>
        <translation type="unfinished">Kërko</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation type="unfinished">Veti Shumëkëndëshi</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Faktor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Numër kulmesh për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Gradë rrotullimi për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Shembull Shumëkëndëshi</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation type="unfinished">Parapëlqime</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Të përgjithshme</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Udhëzuesa</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Tipografi</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Mjete</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">E papastër</translation>
    </message>
    <message>
        <source>Display</source>
        <translation type="unfinished">Shfaq</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation type="unfinished">Mjete të Jashtëm</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="unfinished">Të ndryshme</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation type="unfinished">GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation type="unfinished">&amp;Temë:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="unfinished">&amp;Madhësi Gërmash:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="unfinished">Njësi</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="unfinished">Pikë (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="unfinished">Milimetra (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Inç (inç)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Pikas (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="unfinished">Rregullime Miu</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation type="unfinished">Hidhu te:</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="unfinished">Caktoni rrezen e lakimit të kulmit</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="unfinished">Menu</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation type="unfinished">Dokumente së &amp;Fundmi:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Shtigje</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation type="unfinished">&amp;Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation type="unfinished">&amp;Ndrysho...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation type="unfinished">Profile &amp;ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation type="unfinished">Kë&amp;mbe...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation type="unfinished">&amp;Programthe:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished">N&amp;dryshoni...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vetiake</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Madhësi:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Së gjeri</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">Orie&amp;ntim:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Gjerësi:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Lartësi:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="unfinished">Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="unfinished">&amp;Faqe Majtas fillimisht</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Djathtas:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Majtas:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Vetëruajtje</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="unfinished">&amp;Veprues</translation>
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
        <source>Grid Layout</source>
        <translation type="unfinished">Skemë Rrjete</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="unfinished">Hapje për Rrjetën e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="unfinished">Hapje për Rrjetën e &amp;Madhe:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="unfinished">Largësi ndërmjet shtyllash</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="unfinished">Ngjyra Rrjete</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="unfinished">Ngjyrë për Rrjetën e &amp;Vogël:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="unfinished">Ngjyrë për Rrjetën e &amp;Madhe:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="unfinished">Ngjyrë Udhëzuesash &amp;Përdoruesi:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="unfinished">&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="unfinished">Vendosje</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="unfinished">Në Sfond:</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="unfinished">Në Ballë</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="unfinished">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="unfinished">O&amp;ff</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Poshtëshkrim</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished">&amp;Zhvendosje:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished">&amp;Ripërmasim:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Sipërshkrim</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished">Zh&amp;vendosje:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished">Ri&amp;përmasim</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished">Ripërm&amp;asim:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="unfinished">Tjetër</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">Degëzim Vertical</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="unfinished">&amp;Kryeradhë dhe Hapësira</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="unfinished">&amp;Gërma Parazgjedhje:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="unfinished">&amp;Madhësi Parazgjedhje:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="unfinished">Ngjyrë &amp;Teksti:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="unfinished">Sht&amp;ylla:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">&amp;Boshllëk</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">W-ja është një shkronjë që i mungon Shqipes </translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished">Ngjyrë &amp;Vije:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Hije:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished">Ngjyrë &amp;Mbushëse:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="unfinished">&amp;Tip Vije:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">&amp;Gjerësi Vije:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="unfinished">Mi&amp;nimum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished">Ma&amp;ksimum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished">&amp;Hapje:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="unfinished">Ripërmasim i &amp;Lirë</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished">Ripërmasim &amp;Horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished">Rpërmasim &amp;Vertical:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished">&amp;Ripërmaso Pamje sa Madhësia e Kornizës</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished">Ruaj &amp;Përpjestim</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished">Ngjyrë sfondi:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rrotullim:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Faktor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="unfinished">Mundësi të Tjera</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Paraparje</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="unfinished">E vogël</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished">Mesatare</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="unfinished">Shfaq Faqet &amp;Krah Njëra Tjetrës</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="unfinished">Ngjyra Faqeje</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="unfinished">&amp;Sfond:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="unfinished">&amp;Mënjana:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Paraqit Zonë të &amp;Pashtypshme në Ngjyrë Mënjanash</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="unfinished">Përdor Karakteristika &amp;Tejdukshmërie PDF 1.4 </translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation type="unfinished">&amp;Ujdis Madhësi Paraqitjeje</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="unfinished">Interpretues PostScript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation type="unfinished">&amp;Emër i të Ekzekutueshmit:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation type="unfinished">Antialias &amp;Tekstesh</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation type="unfinished">Antialias &amp;Grafikash</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation type="unfinished">Mjet Përpunimi Pamjesh</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation type="unfinished">Emër i të &amp;Ekzekutueshmit:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="unfinished">Po shtyp</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="unfinished">Qeth sa &amp;Mënjanat e Faqes</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="unfinished">Zgjidhni pamjet dhe zbukurim dritaresh parazgjedhje.
Scribus-i bën të vetën cilëndo temë të mundshme KDE apo Q</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation type="unfinished">Madhësi parazgjedhje gërmash për menu dhe dritare</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation type="unfinished">Njësi matëse parazgjedhje për përpunime dokumentesh</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation type="unfinished">Drejtori dokumentesh parazgjedhje</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="unfinished">Drejtori profilesh ICC parazgjedhje</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation type="unfinished">Drejtori programthesh Scripter parazgjedhje</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation type="unfinished">Madhësi parazgjedhje e faqes, ose si madhësi standard ose si madhësi vetjake</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation type="unfinished">Orientim parazgjedhje faqesh dokumenti</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Gjerësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation type="unfinished">Lartësi faqesh dokumenti, e përpunueshme nëse keni zgjedhur madhësi vetjake faqeje</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="unfinished">Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="unfinished">Nëse vepruese, Scribus ruan një kopje kopjeruajtjeje të kartelës suaj me zgjatimin .bak
çdo herë që mbaron periudha kohore</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation type="unfinished">Periudhë kohore ndërmjet ruajtjesh të vetvetishme</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="unfinished">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished">Ngjyrë e vijave udhëzuese që fusni</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="unfinished">Vendos rrjetën pas objekteve të faqes</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="unfinished">Vendos rrjetën para objekteve të faqes</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="unfinished">Kopjo fluturimthi</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="unfinished">/Mënyrë Vizualizimi/Pa</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="unfinished">Veti Kornize Teksti</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="unfinished">Veti Kornize Pamjeje</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished">Veti Vizatimi Formash</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished">Parazgjedhje Shkalle Zmadhimi</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="unfinished">Veti Vizatimi Vijash</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished">Veti Vizatimi Shumëkëndëshash</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="unfinished">Gërma për korniza të reja tekstesh</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="unfinished">Ngjyrë gërmash</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">Numër shtyllash në një kornizë teksti</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">Hapësirë midis shtyllash në kornizë teksti</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="unfinished">Shembull i gërmave tuaja</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished">Korniza pamjesh lejojnë pamjet të ripërmasohen në çfarëdo madhësi</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished">Koeficient shkalle gërmash</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="unfinished">Koeficient shkalle gërmash</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished">Pamjet e ripërmasuara vetvetiu ruajnë përpjestimet e veta fillestare</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="unfinished">Ngjyrë mbushjeje për korniza pamjesh</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="unfinished">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="unfinished">Ngjyrë vije formash</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="unfinished">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="unfinished">Ngjyrë mbushjeje për forma</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="unfinished">Stil vije formash</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="unfinished">Gjerësi vije formash</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished">Zmadhimi më i vogël i lejuar</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished">Zmadhimi më i madh i lejuar</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished">Ndryshimi në zmadhim për çdo veprim &quot;zoom&quot;</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="unfinished">Ngjyrë vijash</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished">Ngopje ngjyre:</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="unfinished">Stil vijash</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="unfinished">Gjerësi vijash</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Numër kulmesh për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Gradë rrotullimi për shumëkëndëshat</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Shembull Shumëkëndëshi</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation type="unfinished">Ngjyrë për letrën</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="unfinished">Ngjyrë për vija mënjanash</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Masko zonën jashtë mënjanash me ngjyrën e mënjanave</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="unfinished">Aktivizo karakteristika tejdukshmërie brenda eksportimi PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation type="unfinished">Cakto shkallë zmadhimi parazgjedhje</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="unfinished">Mos shfaq objekte jashtë mënjanave te faqe të shtypura ose kartela të eksportuara</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished">Një rrugë braktisjeje e disa shkallëve të grisë të përbëra nga
cyan, yellow dhe magenta dhe përdorjes së të zezës më mirë.
UCR prek më tepër pjesë pamjeje  që janë tone neutrale dhe/ose të errët
të cilët janë afër të zezës. Përdorimi i kësaj mund të përmirësojë disa pamje
dhe lipsen ca eksperimente dhe prova për çdo herë.
UCR zvogëlon mundësinë e mbingopjes me bojëra CMY.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Zgjidhni një Drejtori</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">&amp;Brenda:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">J&amp;ashtë:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Letër</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished">Tabloid</translation>
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
        <translation type="unfinished">E &amp;kuqe:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation type="unfinished">E &amp;gjelbër:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation type="unfinished">Bl&amp;u:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation type="unfinished">Kanal alfa:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation type="unfinished">Ngjyra &amp;Bazë</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation type="unfinished">Ngjyra &amp;Vetjake</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation type="unfinished">&amp;Përcaktoni Ngjyra Vetjake &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation type="unfinished">&amp;Shto tek Ngjyra Vetjake</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation type="unfinished">Përzgjidhni ngjyrë</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation type="unfinished">Kopjo ose Zhvendos Kartelë</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation type="unfinished">Lexo: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation type="unfinished">Shkruaj: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation type="unfinished">E&amp;mër kartele:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation type="unfinished">T&amp;ip kartele:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation type="unfinished">Një drejtori sipër</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Emër</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">Madhësi</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Tip</translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="unfinished">Datë</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation type="unfinished">Veti</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation type="unfinished">Shih &amp;në:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation type="unfinished">Prapa</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation type="unfinished">Krijo Dosje të Re</translation>
    </message>
    <message>
        <source>List View</source>
        <translation type="unfinished">Parje si Listë</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation type="unfinished">Parje në Hollësi</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation type="unfinished">Paraparje Të dhënash Kartele</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation type="unfinished">Paraparje Përmbajtjeje Kartele</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation type="unfinished">Lexim-shkrim</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation type="unfinished">Vetëm lexim</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation type="unfinished">Vetëm shkrim</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation type="unfinished">I pakapshëm</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation type="unfinished">Simlidhje te Kartelë</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation type="unfinished">Simlidhje te Drejtori</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation type="unfinished">Simlidhje te Special</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Kartelë</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation type="unfinished">Dre</translation>
    </message>
    <message>
        <source>Special</source>
        <translation type="unfinished">Special</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Ruaj Si</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation type="unfinished">&amp;Hap</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation type="unfinished">&amp;Riemërto</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation type="unfinished">R&amp;ingarko</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation type="unfinished">Rendit sipas &amp;Emrit</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation type="unfinished">Rendit sipas &amp;Madhësisë</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation type="unfinished">Rendit sipas &amp;Datës</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation type="unfinished">E &amp;parenditur</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation type="unfinished">Rendit</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation type="unfinished">Shfaq kartela të &amp;fshehura</translation>
    </message>
    <message>
        <source>the file</source>
        <translation type="unfinished">kartela</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation type="unfinished">drejtoria</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation type="unfinished">simlidhja</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation type="unfinished">Fshij %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation type="unfinished">&lt;qt&gt;Jeni të sigurtë se doni të fshihen %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Po</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Jo</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation type="unfinished">Dosje e Re 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation type="unfinished">Dosje e Re</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation type="unfinished">Dosje e Re %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation type="unfinished">Gjej Drejtori</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation type="unfinished">Drejtorira</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="unfinished">Ruaj</translation>
    </message>
    <message>
        <source>Error</source>
        <translation type="unfinished">Gabim</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation type="unfinished">Tërë Kartelat (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation type="unfinished">Përzgjidhni Drejtori</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation type="unfinished">Drejtori:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished">&amp;Gërma</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation type="unfinished">S&amp;til gërmash</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Madhësi</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Efekte</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation type="unfinished">Hequr&amp;vije</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation type="unfinished">&amp;Nënvijë</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished">&amp;Ngjyrë</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation type="unfinished">Shembull</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation type="unfinished">Pro&amp;gramth</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Zbato</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Mbyll</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation type="unfinished">Përzgjidhni Gërma</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation type="unfinished">Fshij</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation type="unfinished">Përzgjidhni Tërë</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Ribëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">P&amp;rij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Ngjit</translation>
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
        <translation type="unfinished">Përshtat...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation type="unfinished">&lt;h3&gt;Rreth Qt-së&lt;/h3&gt;&lt;p&gt;Ky program përdor versionin %1 të Qt-së.&lt;/p&gt;&lt;p&gt;Qt është një toolkit C++ për zhvillim GUI &amp;amp; zbatime shumëplatformëshe.&lt;/p&gt;&lt;p&gt;Qt-ja ofron kalueshmëri prej një burimi të vetëm në MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, dhe tërë variantet e njohur komercialë Unix.&lt;br&gt;Qt është gjithaq i mundshëm për pajisje të trupëzuara.&lt;/p&gt;&lt;p&gt;Qt është një produkt Trolltech. Për më tepër të dhëna shihni &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation type="unfinished">Po importoj tekst</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="unfinished">Tërë Formatet e Mbuluar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Basque</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="unfinished">Katalane</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Çeke</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Daneze</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Holandeze:</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Angleze:</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German</source>
        <translation type="unfinished">Gjermane</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Finlandeze</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Frënge</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Greke</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Hungareze</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Italisht</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Lituaneze</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="unfinished">Polake</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Ruse</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Suedeze</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Spanjolle</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Slovake</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Slovene</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation type="unfinished">Po gatis...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Sfond</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation type="unfinished">Doni vërtet të mbishkruhet Kartela:
%1 ?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="unfinished">&amp;Paraparje Gërmash</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="unfinished">&amp;Fut Special</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="unfinished">Të ri &amp;prej Stampe...</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation type="unfinished">Broshura</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation type="unfinished">Katalogë</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Signs</source>
        <translation type="unfinished">Tabela</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation type="unfinished">Kartolina</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation type="unfinished">Krye Letrash</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation type="unfinished">Zarfa</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation type="unfinished">Karta Biznesi</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation type="unfinished">Kalendarë</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation type="unfinished">Reklama</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation type="unfinished">Etiketa</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="unfinished">Menu</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation type="unfinished">Programe</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation type="unfinished">Formularë PDF:</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation type="unfinished">Paraqitje PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation type="unfinished">Revista</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation type="unfinished">Postera</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation type="unfinished">Lajmërime</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation type="unfinished">Dokumente Tekst</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation type="unfinished">Fletëpalosje</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation type="unfinished">Stampa Vetjake</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished">Ruaj si &amp;Pamje...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation type="unfinished">Ruaj si Pamje</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="unfinished">Gabim në shkrim kartele(ash) përfundim.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation type="unfinished">Gabim në shkrim kartele(ash) përfundim.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation type="unfinished">Eksportim i suksesshëm</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation type="unfinished">Ka një kartelë të tillë. Ta mbishkruaj? </translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation type="unfinished">ka një të tillë. Ta mbishkruaj? </translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Jo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Po</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation type="unfinished">Po, tërë</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="unfinished">Paraparje Shtypjeje</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="unfinished">Para&amp;parje Shtypjeje</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished">Importo &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="unfinished">Ruaj si &amp;Stampë</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="unfinished">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">Programthe &amp;Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">&amp;Ekzekuto Programth...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">Programthe së &amp;Fundmi:</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Shfaq &amp;Konsol</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="unfinished">Referencë Online</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished">Ruaj faqe si &amp;SVG...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Ruaj  si</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation type="unfinished">Pamje SVG (*.svg);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="unfinished">Importo &amp;SVG...</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation type="unfinished">Kartela Vlerash Ndarë me Presje (CSV)</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation type="unfinished">Të dhëna CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation type="unfinished">Titull CSV</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation type="unfinished">Kartela HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation type="unfinished">html</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation type="unfinished">Kartela Teksti</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
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
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found - python error</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation type="unfinished">Fshij</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation type="unfinished">Përzgjidhni Tërë</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Zhbëj</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Ribëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">P&amp;rij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Ngjit</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation type="unfinished">Menu Sistemi</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Hije</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation type="unfinished">Normalizo</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation type="unfinished">Minimizo</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation type="unfinished">Maksimizo</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Mbyll</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation type="unfinished">&amp;Rikthe</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation type="unfinished">&amp;Zhvendos</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Madhësi</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation type="unfinished">Mi&amp;nimizo</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation type="unfinished">Ma&amp;ksimizo</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation type="unfinished">Rri &amp;Sipër</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation type="unfinished">Minimizo</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation type="unfinished">Ul Volumin</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Mbyll</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation type="unfinished">Hi&amp;jezo</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation type="unfinished">%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation type="unfinished">Rregullim Dokumenti</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Madhësi:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Vetiake</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Portret</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Së gjeri</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="unfinished">Gjerësi:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Udhëzuesa Mënjanash</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Krye:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Majtas:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Poshtë:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Djathtas:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="unfinished">Faqe &amp;Përkarshi</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="unfinished">&amp;Faqe Majtas fillimisht</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="unfinished">Numër i Faqes së Parë</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="unfinished">Bëj faqe të parë faqen majtas të një dokumenti</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës së sipërme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës së poshtme dhe skajit të faqes</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës majtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished">Largësi ndërmjet udhëzuesit të mënjanës djathtas dhe skajit të faqes.
Nëse Faqe Përkarshi është përzgjedhur, kjo hapësirë mënjane mund të përdoret për të pasur mënjana të sakta për libërlidhje</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">&amp;Brenda:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="unfinished">&amp;Jashtë:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style Settings</source>
        <translation type="unfinished">Rregullime Stili</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation type="unfinished">Stil për paragrafin e çastit</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation type="unfinished">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="unfinished">Rregullime Ngjyre Mbushjesh</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation type="unfinished">&amp;Jashtëvijëzo</translation>
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
        <translation type="unfinished">Asnjë</translation>
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
        <translation type="unfinished">Rregullime Gërmash</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation type="unfinished">Gërma teksti të përzgjedhur</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation type="unfinished">Gërma (pa hapësira):</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation type="unfinished">Rregullimet Shenjash</translation>
    </message>
    <message>
        <source>Kerning:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="unfinished">&amp;Doracak për Scribus...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Kopjo #%1 nga</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Sfond</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Kartelë</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="unfinished">Krijo një Dokument të ri</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="unfinished">Hap një Dokument</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="unfinished">Ruaj Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="unfinished">Mbyll Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="unfinished">Shtyp Dokumentin e çastit</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="unfinished">Ruaj Dokumentin e çastit si PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Po kërkoj për Gërma</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="unfinished">Po dal tani</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="unfinished">Gabim Fatal</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="unfinished">Vijë e Mençur</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="unfinished">Vendos Majtas</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="unfinished">Vendos Djathtas</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="unfinished">Vendos në Qendër</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="unfinished">Fut Numër Faqeje</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="unfinished">Scrapbooks (*.scs);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="unfinished">Shfaq Shtresa</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="unfinished">Javascripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="unfinished">Zhbëj</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="unfinished">Shfaq Paraparje Faqeje</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="unfinished">Blloko/Çblloko</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="unfinished">Hapësirë pas Shtylle</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="unfinished">Po lexoj Parapëlqimet</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="unfinished">Po marr Profile ICC</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="unfinished">Po rregulloj Shkurtprerje</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="unfinished">Administrimi &amp;Ngjyrash...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="unfinished">Po lexoj nga e Papastër</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="unfinished">Po gatis Shtojca</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Hap...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="unfinished">Hap të &amp;Fundmin</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="unfinished">e Re</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="unfinished">Hap...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="unfinished">Mbyll</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Ruaj</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="unfinished">Ruaj</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Ruaj &amp;Si...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="unfinished">Ruaj  si</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished">Ri&amp;kthehu tek i Ruajtur</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished">&amp;Kërko për Ndihmë</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="unfinished">&amp;Merr Tekst/Pamje...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished">Vini &amp;Tekst...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="unfinished">Importo &amp;Faqe(t)...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;/Importo</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished">Ruaj &amp;Tekst...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished">Ruaj Faqe si $EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished">&amp;Ruaj si P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="unfinished">&amp;Eksporto</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="unfinished">Të dhë&amp;na Dokumenti...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="unfinished">Të dhëna Dokumenti...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished">&amp;Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="unfinished">Rregullim Dokumenti...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">&amp;Shtyp...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="unfinished">Shtyp...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished">&amp;Lër</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="unfinished">Lër</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Zhbëj</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">P&amp;rij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Pa&amp;stro</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">Përzgjidhni &amp;Tërë</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="unfinished">Pri</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="unfinished">Kopjo</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="unfinished">Ngjit</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="unfinished">Fshij</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="unfinished">Përzgjidhni tërë</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Kërko/Zëvendëso...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished">Ngj&amp;yra...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="unfinished">Ngjyra...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished">Stile &amp;Paragrafi...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished">Stile &amp;Vije...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="unfinished">Stile...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="unfinished">&amp;Stampa...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="unfinished">Stampa...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="unfinished">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="unfinished">Përzgjidhni Gërma të Reja</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="unfinished">Dyfisho</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished">Dyfisho CD</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="unfinished">Dyfisho CD</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="unfinished">Fshij</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">&amp;Grup</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="unfinished">Grup</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="unfinished">Shto Grup</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="unfinished">&amp;Blloko</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="unfinished">Shpjere &amp;Prapa</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="unfinished">Shpjere Prapa</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="unfinished">Bjer &amp;Përpara</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="unfinished">Bjere Përpara</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="unfinished">&amp;Ul</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="unfinished">I poshtëm</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished">&amp;Ngri</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="unfinished">Ngri</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="unfinished">Shpërnda/&amp;Drejto...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="unfinished">Shpërnda/Drejto...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="unfinished">&amp;Përpuno Formë</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Formë</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished">&amp;Bashkangjit Tekst tek Shteg</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished">&amp;Shqit Tekst prej Shtegu</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished">&amp;Ndërthur Shumëkëndësha</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished">Ndaj &amp;Shumëkëndësha</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="unfinished">Shto tek stampë</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="unfinished">&amp;Fut...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="unfinished">Fut...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished">&amp;Fshij...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="unfinished">Fshij...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished">&amp;Zhvendos...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="unfinished">Zhvendos...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="unfinished">&amp;Zbato Stampë...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="unfinished">Zbato Stampë...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished">Administro &amp;Udhëzuesa...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="unfinished">Administro Udhëzuesa...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="unfinished">&amp;Sa Dritarja</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="unfinished">Sa Dritarja</translation>
    </message>
    <message>
        <source>50%</source>
        <translation type="unfinished">50%</translation>
    </message>
    <message>
        <source>75%</source>
        <translation type="unfinished">75%</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="unfinished">&amp;100%</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="unfinished">100%</translation>
    </message>
    <message>
        <source>200%</source>
        <translation type="unfinished">200%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished">Shfaq &amp;Mënjana</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="unfinished">Fshih Mënjana</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished">Shfaq &amp;Korniza</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="unfinished">Fshih Korniza</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished">Shfaq &amp;Pamje</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="unfinished">Fshih Pamje</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished">Shfaq &amp;Rrjetë</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="unfinished">Shfaq Rrjetë</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished">Shfaq &amp;Udhëzuesa</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="unfinished">Tekst për tu vizatuar</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="unfinished">&amp;Veti</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="unfinished">Veti</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="unfinished">&amp;Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="unfinished">Jashtëvijëzo</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished">&amp;E papastër</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="unfinished">E papastër</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished">&amp;Shtresa</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="unfinished">Ka paletë</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Faqerojtësa</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished">&amp;Administro Pamje</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Administro Pamje</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished">&amp;Vijëzo Tekst</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="unfinished">Vijëzo Tekst</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished">Ndih&amp;mëza</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="unfinished">&amp;Mjete</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished">Mjete P&amp;DF</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Mjete</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="unfinished">Ndihmëza</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished">Pa&amp;rapëlqime</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="unfinished">&amp;Gërma...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="unfinished">Gërma...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="unfinished">Shkurtprerje &amp;Tastiere...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="unfinished">&amp;Rreth Scribus-it</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="unfinished">Rreth Scribus-it</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished">Rreth &amp;Qt-s</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="unfinished">Rreth Qt-s</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished">&amp;Doracak për Scribus...</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="unfinished">Ndihmë &amp;Online...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="unfinished">St&amp;il</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="unfinished">&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="unfinished">&amp;Faqe</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="unfinished">&amp;Parje</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;kstra</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="unfinished">&amp;Rregullime</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="unfinished">&amp;Dritare</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished">&amp;Ndihmë</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished">&amp;Majtas</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished">&amp;Qendër</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished">&amp;Djathtas</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished">&amp;Blloko</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished">&amp;Tjetër...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished">Nënvijë</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="unfinished">Hequrvije</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Sipërshkrim</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Poshtëshkrim</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="unfinished">X-Poz:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="unfinished">X-Poz:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="unfinished">Gati</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="unfinished">&amp;Ujvarë</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="unfinished">Merr Tekst.Pamje...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="unfinished">Merr Pamje...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished">&amp;Ngjyrë</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="unfinished">&amp;Përmbys</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="unfinished">&amp;Merr Tekst...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished">&amp;Gërma</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Madhësi</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="unfinished">&amp;Efekte</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="unfinished">&amp;Drejtim</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="unfinished">&amp;Hije</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="unfinished">Merr Tekst...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="unfinished">Gërma</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">Madhësi</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="unfinished">Stil</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Ngjyrë</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Hije</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="unfinished">&amp;Çblloko </translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="unfinished">Po importoj Faqe...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="unfinished">Importoni Faqe(t)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished">Krijo</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="unfinished">Importo</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="unfinished">Import i kryer</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="unfinished">Nuk u gjet gjë për ta importuar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="unfinished">Po ngarkoj...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="unfinished">Tërë Formatet e Mbuluar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Tërë kartelat (*)</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="unfinished">S&apos;shkruaj dot Kartelën: 
%1</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Ruaj  si</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="unfinished">Po ruaj...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="unfinished">Po shtyp...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="unfinished">Shtypja dështoi!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="unfinished">Doracak për Scribus</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished">Kartela Tekst (*.txt);;Tërë Kartelat(*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Madhësi:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished">&amp;Hije:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="unfinished">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="unfinished">Pamjet në korniza pamjesh ripërmasohen sa madhësia e kornizës</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="unfinished">Programet vijues mungojnë:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tërë</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="unfinished">Kartela EPS (*.eps);;Tërë Kartelat (*)</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="unfinished">Disa objekte janë të kyçur.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Anulo</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="unfinished">Kyç tërë</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="unfinished">Çkyç tërë</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="unfinished">Çkyç</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="unfinished">Kyç</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="unfinished">Po ngarkoj:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Po ujdis Ngjyra</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="unfinished">&amp;Zhbëj Fshirje Objekti</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="unfinished">&amp;Zhbëj Zhvendosje Objekti</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="unfinished">&amp;Zhbëj Ndryshim Objekti</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="unfinished">Gjermane</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="unfinished">Polake</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="unfinished">Angleze:</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="unfinished">Spanjolle</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="unfinished">Italisht</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="unfinished">Frënge</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="unfinished">Ruse</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="unfinished">Daneze</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="unfinished">Slovake</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="unfinished">Hungareze</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="unfinished">Çeke</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="unfinished">Holandeze:</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="unfinished">Portugeze</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="unfinished">Ukrainase</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="unfinished">Greke</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="unfinished">Katalane</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="unfinished">Finlandeze</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="unfinished">Irlandeze</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="unfinished">Lituaneze</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="unfinished">Suedeze</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="unfinished">Slovene</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Zgjidhni një Drejtori</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="unfinished">Përmbys</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="unfinished">Vithisje Scribus-i</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished">Scribus-i vithiset për shkak të Sinjalit #%1</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation type="unfinished">Shtresë</translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tërë</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="unfinished">&amp;Dil Sidoqoftë</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="unfinished">M&amp;byll Sidoqoftë</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation type="unfinished">Dokument:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation type="unfinished">është ndryshuar që prej ruatjes së fundit.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="unfinished">&amp;Ruaj Tani</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation type="unfinished">Shfaq Paraparje Faqeje</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="unfinished">Shfaq Emra Stampash</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation type="unfinished">Kërko/Zëvendëso</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation type="unfinished">Kërko për:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation type="unfinished">Stil Paragrafi</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="unfinished">Gërma</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation type="unfinished">Madhësi Gërmash</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation type="unfinished">Efekte Gërma</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation type="unfinished">Ngjyrë Mbushëse</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation type="unfinished">Mbush stil:</translation>
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
        <translation type="unfinished">Majtas</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="unfinished">Qendër</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="unfinished">Djathtas</translation>
    </message>
    <message>
        <source>Block</source>
        <translation type="unfinished">Blloko</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished">pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Asnjë</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation type="unfinished">Zëvendëso me:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation type="unfinished">&amp;Tërë Fjalën</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation type="unfinished">&amp;Shpërfill të Mëdhaja/Vogla</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Kërko</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">&amp;Zëvendëso</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation type="unfinished">Zëvendëso &amp;Tërë</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation type="unfinished">Kontroll i përfunduar</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Pa&amp;stro</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation type="unfinished">Sistemo Faqe</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="unfinished">Stampa të Mundshme:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="unfinished">Faqe Dokumenti</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="unfinished">Faqe Përkarshi</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="unfinished">Faqja Majtas e Para</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="unfinished">Parasheh tërë faqet e dokumentit tuaj.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="unfinished">Këtu keni tërë Stampat tuaja, për të krijuar Faqe të re
tërhiqni një Stampë te parja e Faqes më poshtë.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normale</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation type="unfinished">Përzgjidhni Fusha</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation type="unfinished">Fusha të Mundshme</translation>
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
        <source>Selected Fields</source>
        <translation type="unfinished">Fusha të Përzgjedhura</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation type="unfinished">Tjetër...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished">&amp;Hije:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Hije</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Pa Stil</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Pa Stil</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation type="unfinished">Përpunoni Stile</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">&amp;Vini</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">Dy&amp;fisho</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Ruaj</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Kopje e %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation type="unfinished">Stil i Ri</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Jo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Po</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Dokumenta (*.sla *.sla.gz *.scd *.scd.gz);;Tërë Kartelat (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Dokumente (*.sla *.scd);;Tërë kartelat (*)</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation type="unfinished">Përpunues Historie</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;I Ri</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation type="unfinished">&amp;Ringarko Tekst prej Kornize</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation type="unfinished">&amp;Ruaj te Kartelë...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation type="unfinished">&amp;Ngarko prej Kartele...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation type="unfinished">Ruaj &amp;Dokument</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation type="unfinished">&amp;Përditëso Kornizë Teksti dhe Dil</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation type="unfinished">&amp;Dil Pa Përditësuar Kornizë Teksti</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">Përzgjidhni &amp;Tërë</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">P&amp;rij</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Kopjo</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Ngjit</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Kërko/Zëvendëso...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="unfinished">&amp;Fut Special...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation type="unfinished">&amp;Përpunoni Stile...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="unfinished">&amp;Paraparje Gërmash...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation type="unfinished">&amp;Përditëso Kornizë Teksti</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation type="unfinished">&amp;Sfond...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation type="unfinished">&amp;Shfaq Gërma...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Kartelë</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Përpunoni</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="unfinished">&amp;Rregullime</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Kartelë</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="unfinished">Pastro tërë Tekstet</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="unfinished">Ngarko Tekst prej Kartele</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="unfinished">Ruaj Tekst në Kartelë</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="unfinished">Përditëso Kornizë Teksti dhe Dil</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="unfinished">Dil Pa Përditësuar Kornizë Teksti</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="unfinished">Ringarko Tekst prej Kornize</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="unfinished">Përditëso Kornizë Teksti</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="unfinished">Kërko/Zëvendëso</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation type="unfinished">Paragraf i Çastit:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished">Fjalë:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished">Gërma:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation type="unfinished">Gjithsej:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished">Paragrafë:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation type="unfinished">Doni të ruani ndryshimet tuaj?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="unfinished">Doni vërtet të humbasin tërë Ndryshimet tuaj?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="unfinished">Doni vërtet të pastroni tërë Tekstin tuaj?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Hap</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished">Kartela Tekst (*.txt);;Tërë Kartelat(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Ruaj  si</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation type="unfinished">Nënvijë</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Të vogla të Mëdha</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Poshtëshkrim</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Sipërshkrim</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation type="unfinished">Line out</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="unfinished">Jashtëvijëzo Tekst</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation type="unfinished">Administro shtojca</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation type="unfinished">Majtas</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="unfinished">Djathtas</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation type="unfinished">Ndalesë e Plotë</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation type="unfinished">Presje</translation>
    </message>
    <message>
        <source>Center</source>
        <translation type="unfinished">Qendër</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation type="unfinished">&amp;Pozicion:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="unfinished">R&amp;reshti i Parë:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation type="unfinished">Fshij Tërë</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">inç</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation type="unfinished">Jashtëvijëzo</translation>
    </message>
    <message>
        <source>Element</source>
        <translation type="unfinished">Element</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Tip</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Të dhëna</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Kujdes</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="unfinished">Emri &quot;%1&quot; s&apos;është unik.
Ju lutem zgjidhni një tjetër.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>X:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Y:</source>
        <translation type="unfinished">Y:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Gërma:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation type="unfinished">Grup</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="unfinished">Pamje</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Rresht</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="unfinished">Shumëkëndësh</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Shumëvijë</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="unfinished">Tekst Shtegu</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Faqe</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
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
        <source>Send your value to the script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Mjete</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="unfinished">Elementë të përzgjedhur</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="unfinished">Fut Kornizë Teksti</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="unfinished">Fut Pamje</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished">Fut Tabelë</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="unfinished">Veti...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="unfinished">Fut Shumëkëndësha</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="unfinished">Fut Vija</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="unfinished">Fut Kurba Bezier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="unfinished">Fut Vijë me Dorë të Lirë</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="unfinished">Rrotulloni Element</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="unfinished">Zmadho ose zvogëlo</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="unfinished">Përpunoni Përmbajtje Kornize</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="unfinished">Përpuno tekstin me Përpunues Historie</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="unfinished">Lidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="unfinished">Zgidh Korniza Tekstesh</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="unfinished">Kryej matje</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="unfinished">Vizatoni Forma të ndryshme</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation type="unfinished">Mjete PDF</translation>
    </message>
    <message>
        <source>Button</source>
        <translation type="unfinished">Buton</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation type="unfinished">Fushë Tekstesh</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation type="unfinished">Kutizë</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation type="unfinished">Listë Kuti</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation type="unfinished">Kuti Liste</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation type="unfinished">Fusni Fusha PDF</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Tekst</translation>
    </message>
    <message>
        <source>Link</source>
        <translation type="unfinished">Lidhje</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation type="unfinished">Fusni Shënime PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="unfinished">Përzgjidhni Gërma:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Fut</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Mbyll</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished">Fshij përzgjedhjen(t) e çastit.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation type="unfinished">Zgjidhni importuar për t&apos;u përdorur</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Automatike</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="unfinished">Merr vetëm tekst</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation type="unfinished">Importo tekst pa e formatuar</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation type="unfinished">Importues:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation type="unfinished">Kodim:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation type="unfinished">Zgjidhni importuar për t&apos;u përdorur</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation type="unfinished">Të ri Prej Stampe</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation type="unfinished">&amp;Hiq</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation type="unfinished">&amp;Hap</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Tërë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Emër</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Ngjyra</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="unfinished">Përshkrim</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation type="unfinished">Përdorim</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation type="unfinished">Krijuar me</translation>
    </message>
    <message>
        <source>Date</source>
        <translation type="unfinished">Datë</translation>
    </message>
    <message>
        <source>Author</source>
        <translation type="unfinished">Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation type="unfinished">Po shkarkoj Stampa</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation type="unfinished">Po instaloj Stampa</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation type="unfinished">Po përgatis stampë</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation type="unfinished">Sigurohuni që pamjet dhe gërmat që përdorni mund të përdoren lirisht. Nëse gërmat nuk janë të përbashkëta mos i koleksiononi gjatë ruajtjesh stampash.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation type="unfinished">Po heq stampë</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation type="unfinished">po përkthej stampë XML</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation type="unfinished">Ruaj si Stampë</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Emër</translation>
    </message>
    <message>
        <source>Category</source>
        <translation type="unfinished">Kategori</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation type="unfinished">Madhësi Faqeje</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Ngjyra</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="unfinished">Përshkrim</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation type="unfinished">Përdorim</translation>
    </message>
    <message>
        <source>Author</source>
        <translation type="unfinished">Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation type="unfinished">Email</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation type="unfinished">Më tepër Hollësi</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="unfinished">OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation type="unfinished">Më pak Hollësi</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="unfinished">Letër</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="unfinished">Tabloid</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation type="unfinished">së gjeri</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation type="unfinished">portret</translation>
    </message>
    <message>
        <source>custom</source>
        <translation type="unfinished">vetiak</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">Pa&amp;stro</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Fshij</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Anulo</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>and</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>words</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished">Hiq</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply</source>
        <translation type="unfinished">Zbato</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
