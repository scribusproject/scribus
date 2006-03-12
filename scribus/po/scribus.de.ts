<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Gibt eine Liste zurück, die alle vorhandenen Farben des aktuellen Dokuments enthält. 
Wenn kein Dokument geöffnet ist, werden die Standardfarben zurückgegeben.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Zeigt den Dialog \&quot;Neues Dokument\&quot; an. Damit können Sie ein neues Dokument erstellen, wenn der Benutzer die standardmäßigen Einstellung aktzeptiert. Erzeugt kein neues Dokument, wenn der Benutzer auf \&quot;Abbrechen\&quot; klickt. 
Gibt den Wert \&quot;true\&quot; zurück, wenn ein neues Dokument erstellt wurde.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Gibt den Wert der Füllfarbe des Objekts &quot;name&quot; zurück.
Wenn &quot;name&quot; nicht angegeben ist, wird das aktuell ausgewählt Objekt verwendet.</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;])

Verschiebt das Objekt &quot;name&quot; um dx und dy relativ zur Position. Die 
Entfernungsangaben werden in der Maßeinheit des Dokuments 
angegeben (siehe Konstanten UNIT*). Wenn &quot;name&quot; nicht vorhanden ist, 
wird das ausgewählte Objekt verschoben und wenn &quot;name&quot; zu einer Gruppe
gehört, wird die gesamte Gruppe verschoben.</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Wenn bool=False, dann wird die Seite nicht neu neu aufgebaut (redraw ist deaktiviert).
Standard ist bool=True. Diese Einstellung bleibt bestehen, auch wenn das Script beendet ist.
Stellen Sie also sicher, dass setRedraw(True) aufgerufen wird, bevor das Script beendet wird.</translation>
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

Erzeugt ein neues Rechteck auf der aktuellen Seite und gibt seinen Namen zurück. Die
Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe UNIT-Konstanten).
&quot;name&quot; sollte eine eindeutige Identifikation des Rechtecks ermöglichen, weil Sie den Objekt-
namen immer wieder benötigen. Ist &quot;name&quot; nicht vorhanden, wählt Scribus einen Namen für Sie.

Wenn Sie einen schon verwendeten Namen benutzen, tritt der Fehler NameExistsError auf.</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Füllt das Objekt &quot;name&quot; mit einem Farbverlauf. Farbbezeichnungen sind die
gleichen wie für setFillColor() und setFillShade(). Siehe Konstanten
für mögliche Typen (FILL_&lt;type&gt;).</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;name&quot;]) -&gt; float

Gibt die Schriftgröße in Punkten für den Textrahmen &quot;name&quot; zurück. Wenn innerhalb
des Rahmens Text markiert ist, wird die Schriftgröße des ersten Zeichens verwendet.
Wenn &quot;name&quot; nicht vergeben ist, wird der selektierte Textrahmen verwendet.</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Schreibt &quot;string&quot; in die Statuszeile von Scribus. Der Text muss
UTF8-kodiert oder ein &apos;unicode&apos;-String sein (empfohlen).</translation>
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
        <translation type="obsolete">newDoc(size, margins, orientation, firstPageNumber, unit facingPages, firstSideLeft) -&gt; bool

Erstellt eine neue Datei und gibt, falls erfolgreich, true zurück. Die Parameter haben
folgende Bedeutung:

size = Ein Zahlenpaar (Breite, Höhe), das die Größe des Dokuments beschreibt. Sie können
vordefinierte Konstanten wie PAPER_&lt;paper_type&gt;, z. B.  PAPER_A4 etc. verwenden.

margins=margins = Eine Zahlenkombination, welche die Seitenränder beschreibt (links, rechts, oben, unten).

orientation = Ausrichtung der Seite. Konstanten sind PORTRAIT und LANDSCAPE.

firstPageNumer = Ist die Nummer der ersten Seite, auf welcher die Seitennumerierung beginnt. Während Sie
normalerweise mit &quot;1&quot; beginnen werden, ist es nützlich mit anderen Seitennummern zu beginnen, wenn Sie ein 
Dokument in mehrere Dateien aufteilen.

unit: Dieser Wert legt die im Dokument verwendete Maßeinheit fest. Wählen Sie eine der folgenden 
vordefinierten Konstanten aus: UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.

facingPages = FACINGPAGES, NOFACINGPAGES

firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

Die Werte für Breite, Höhe und die Ränder werden in der für das Dokument gewählten Maßeinheit angegeben,
die Werte für PAPER_*-Konstanten jedoch immer in Punkt. Wenn die gewählte Maßeinheit nicht Punkt ist, sollten
Sie dies beachten.

Beispiel: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                              FACINGPAGES, FIRSTPAGERIGHT)</translation>
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

Erzeugt eine neue Seite. Falls &quot;where&quot; -1 ist, wird die neue Seite an das Dokument angefügt,
ansonsten wird die neue Seite vor &quot;where&quot; eingefügt. Seitennummern werden von 1 ab hoch-
gezählt, egal welches die angezeigte erste Seitennummer Ihres Dokumentes ist. Der optionale
Parameter &quot;masterpage&quot; legt den Namen der Musterseite für die neue Seite fest</translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>&quot;string&quot; muss ein gültiger Dateiname einer SVG-Datei sein. Der Text
muss UTF8-kodiert oder ein Unicode-String (empfohlen) sein.</translation>
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
        <translation>newDocument(size, margins, orientation, firstPageNumber,
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
        <translation>getColor(&quot;name&quot;) -&gt; tuple

Gibt eine Liste mit den vier Farbkomponenten der Farbe &quot;name&quot; im aktuellen 
Dokument wieder. Wenn kein Dokument geöffnet ist, werden die Werte der 
angegebenen Standardfarbe zurückgegeben.

Wenn die Farbe nicht gefunden wird, tritt der Fehler NotFoundError auf.
Wenn der Name der Farbe ungültig ist, tritt der Fehler ValueError auf.</translation>
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

Ändert die Farbe &quot;name&quot; auf die angegeben CMYK-Werte. Die Abkürzungen
stehen für die vier Farben c=Cyan, m=Magenta, y=Yellow, b=Black.
Die Werte müssen zwischen 0 und 255 liegen.

Wenn die Farbe nicht gefunden wird, tritt der Fehler NotFoundError auf.
Wenn ein ungültiger Farbname angegeben wird, tritt der Fehler ValueError auf.</translation>
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

Löscht die Farbe &quot;name&quot;. An allen Stellen, wo &quot;name&quot; im Dokument auftritt,
wird sie durch &quot;replace&quot; ersetzt. Standardmäßig ist &quot;replace&quot; gar keine Farbe -
Transparenz.

Sie können deleteColor auch verwenden, wenn kein Dokument geöffnet ist, dann
bleibt die Angabe von &quot;replace&quot; aber ohne Auswirkung.

Wenn die angegebene Farbe nicht gefunden wird, tritt der Fehler NotFoundError auf.
Wenn der Farbname ungültig ist, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

An allen Stellen, wo die Farbe &quot;name&quot; vorkommt, wird sie duch die Farbe
&quot;replace&quot; ersetzt.

Wenn eine der Farben nicht vorhanden ist, tritt der Fehler NotFoundError auf.
Wenn der Name der Farbe ungültig ist, tritt der Fehler ValueError auf.</translation>
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
        <translation>messageBox(&quot;caption&quot;, &quot;message&quot;,icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
button2=BUTTON_NONE, button3=BUTTON_NONE) -&gt; integer
Zeigt einen Dialog mit dem Titel &quot;caption&quot;, der Botschaft &quot;message&quot; und dem Symbol &quot;icon&quot; sowie
bis zu 3 Buttons an. Standard ist kein Symbol und ein OK-Button. Nur Titel und Botschaft sind
erforderliche Felder, aber Symbol und Buttons werden empfohlen.
Der Text der Botschaft kann auch einfachen HTML-Code enthalten.
Gibt die Nummer des Buttons wieder, der gedrückt wurde, beginnend mit 1.
Für Buttons und Symbole gibt es vordefinierte Konstanten, die gleichen wie in
der Qt-Dokumentation.
Die sind im Modul definiert und heißen BUTTON_* und ICON_* . Es gibt auch zwei
spezielle Konstanten, die auch den Wert 0 annehmen können: BUTTONOPT_DEFAULT aktiviert den 
jeweiligen Button per Druck auf Enter.
BUTTONOPT_ESCAPE drückt die Escape-Taste falls dieser Button gedrückt wird.
Beispiel
result = messageBox(&apos;Script failed&apos;,&apos;This script only works when you have a text frame selected.&apos;,
ICON_ERROR)
result = messageBox(&apos;Monkeys!&apos;, &apos;Something went ook! &lt;i&gt;Was it a monkey?&lt;/i&gt;&apos;,ICON_WARNING,
BUTTON_YES|BUTTONOPT_DEFAULT, BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)
Konstanten für Buttons und Symbole:
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL.</translation>
    </message>
    <message>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation>valueDialog(caption, message [,defaultvalue]) -&gt; string

Zeigt einen Dialog an, der einen String verlangt und zurückgibt.
Parameter: Fenstertitel, Text im Fenster und optionaler &apos;default&apos;-Text.

Beispiel: valueDialog(&apos;TItel&apos;, &apos;Text im Fenster&apos;, &apos;optional&apos;)</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Schließt das aktuelle Dokument, ohne Änderungen zu sichern.

Wenn kein Dokument offen ist, tritt der Fehler NoDocOpenError auf</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Gibt True zurück, wenn ein Dokument geöffnet ist.</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;)

Öffnet das Dokument &quot;name&quot;.

Schlägt das fehl, tritt der Fehler ScribusError auf.</translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation>saveDoc()

Speichert das Dokument unter dem aktuellen Namen und gibt True zurück, wenn das
erfolgreich war. Wurde das Dokument noch nicht gesichert, öffnet sich der &quot;Speichern&quot;-
Dialog.

Wenn das Sichern fehlschlägt, erscheint derzeit keine Fehlermeldung.</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;name&quot;)

Speichert das aktuelle Dokument unter dem neuen Namen &quot;name&quot;. Die Pfadangabe kann dabei
relativ oder absolut sein.

Schlägt das Sichern fehl, tritt der Fehler ScribusError auf.</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;Autor&quot;, &quot;Info&quot;, &quot;Beschreibung&quot;) -&gt; bool

Speichert die Dokumenteigenschaften Autor, Informationen und Beschreibung.
Angabe als Strings.</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Verändert die Ränder des Dokuments. Links=lr, Rechts=rr, Oben=tr, Unten=br.
Die Einheit wird durch die Maßeinheit des Dokuments vorgegeben, siehe die
Konstanten UNIT_&lt;type&gt;.</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type)

Ändert die Maßeinheit des Dokuments auf &quot;type&quot;. Mögliche Einheiten geben Sie durch die 
Konstanten UNIT_&lt;type&gt; an.

Ist die Einheit ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (Scribus unit constant)

Gibt die Maßeinheit des Dokuments zurück. Der Rückgabewert ist eine
der möglichen UNIT_*-Konstanten:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filename&quot;)

Lädt die Absatzstile aus der Datei &quot;filename&quot; in das aktuelle Dokument.</translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(facingPages, firstPageLeft)

Setzt den Dokumenttyp. Gegenüberliegende Seiten erreichen Sie mit FACINGPAGES, normale
Seiten mit NOFACINGPAGES. Wenn die erste Seite links sein soll, schreiben Sie FIRSTPAGELEFT, 
ansonsten FIRSTPAGERIGHT.</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Gibt den Namen der Linienfarbe des Objekts &quot;name&quot; zurück.
Wenn &quot;name&quot; nicht angegeben ist, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer

Gibt die Linienbreite des Objekts &quot;name&quot; zurück.
Wenn &quot;name&quot; nicht angegeben ist, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; integer

Gibt den Tonwert des Objekts &quot;name&quot; zurück.
Wenn &quot;name&quot; nicht angegeben ist, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; integer (siehe Konstanten)

Gibt den Stil der Ecken des Objekts &quot;name&quot; zurück. Ist kein &quot;name&quot; angegeben, wird
das aktuelle Objekt benutzt. Die Linientypen sind JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;name&quot;]) -&gt; integer (siehe Konstanten)

Gibt den Stil der Linienrundungen des Objekts &quot;name&quot; an. Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt benutzt.
Die unterstützten Stile sind CAP_FLAT, CAP_ROUND, CAP_SQUARE</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;name&quot;]) -&gt; integer (siehe Konstanten)

Gibt den Linienstil des Objekts &quot;name&quot; wieder. Ist &quot;name&quot; nicht angegeben,
wird das aktuelle Objekt benutzt. Linienstile sind LINE_DASH, LINE_DASHDOT,
LINE_DASHDOTDOT, LINE_SOLID</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Gibt den Tonwert der Füllfarbe von &quot;name&quot; zurück. Ist &quot;name&quot; nicht angegeben, wird
das aktuelle Objekt verwendet.</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y)(

Gibt ein Zahlenpaar (x,y) zurück mit den Skalierungswerten des Bildes im Rahmen
&quot;name&quot;. Ist &quot;name&quot; nicht angegeben, wird der aktuelle Rahmen verwendet.</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; string

Gibt den Dateinamen des Bildes im Rahmen &quot;name&quot; zurück. Ist &quot;name&quot; nicht
angegeben, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;name&quot;]) -&gt; (Breite,Höhe)

Gibt das Zahlenpaar (Breite,Höhe) des Objekts &quot;name&quot; zurück. Ist &quot;name&quot; nicht
angegeben, wird das aktuelle Objekt verwendet. Die Größe wird in der Maßeinheit
des Dokuments angegeben - siehe die UNIT_*-Konstanten.</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;name&quot;]) -&gt; integer

Gibt die Drehung des Objekts &quot;name&quot; in Grad zurück und im Uhrzeigersinn zurück.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Gibt eine Liste zurück mit allen auf der aktuellen Seite verwendeten Objekte.</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;name&quot;])

Bewegt das Objekt &quot;name&quot; an die neue Stelle. Die Koordinaten werden in der Maßeinheit
des Dokuments angegeben (siehe UNIT_*-Konstanten). Ist &quot;name&quot; nicht angegeben, wird das
aktuelle Objekt benutzt. Gehört &quot;name&quot; zu einer Gruppe, wird die Gruppe verschoben.</translation>
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

Dreht das Objekt &quot;name&quot; relativ um &quot;rot&quot; Grad. Beim Drehen wird der Ursprung verwendet,
der gerade aktiv ist - normalerweise der Punkt links oben. Positive Werte für &quot;rot&quot; bedeuten
Drehung in Uhrzeigersinn, negative Werte Drehung gegen den Uhrzeigersinn. Ist &quot;name&quot; nicht
angegeben, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(width, height [, &quot;name&quot;])

Ändert die Größe von &quot;name&quot; auf die Breite width und die Höhe height.
ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt benutzt.</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; string

Gibt den Namen des ausgewählten Objekts zurück. &quot;nr&quot; gibt die Zahl des gewählten
Objekts an. 0 bedeutet das erste ausgewählte Objekt, 1 das zweite usw.</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Gibt die Anzahl der ausgewählten Objekte zurück.</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;name&quot;)

Wählt das Objekt &quot;name&quot; aus.</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Setzt alle Objekte im Dokument auf nicht ausgewählt.</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(list)

Gruppiert die Elemente in &quot;list&quot;. &quot;list&quot; muss die Namen der zu gruppierenden 
Objekte enthalten. Ist &quot;list&quot; nicht angegeben, werden die aktuell gewählten 
Objekte verwendet.</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;name&quot;)

Zerstört die Gruppe &quot;name&quot;. Ist &quot;name&quot; nicht angegeben, wird die aktuelle Gruppe verwendet.</translation>
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

Skaliert die Gruppe, zu dem das Objekt &quot;name&quot; gehört. Werte größer als 1 vergrößern das Objekt, 
Werte kleiner als 1 verkleinert das Objekt. Zum Beispiel bedeutet 0.5 = 50 % oder 1.5=150% der 
Originalgröße. &quot;factor&quot; muss größer als 0 sein. Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt
benutzt.</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Lädt das Bild &quot;picture&quot; in den Bildrahmen &quot;name&quot;. Ist &quot;name&quot; nicht angegeben, 
wird der aktuelle Rahmen benutzt.

Ist das Ziel kein Bildrahmen, tritt der Fehler WrongFrameTypeError auf</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;name&quot;])

Skaliert das Bild &quot;name&quot; auf die angegeben Werte. 1 bedeutet 100 %. Ist &quot;name&quot;
nicht angegeben, wird der aktuelle Bildrahmen verwendet.

Ist das Ziel kein Bildrahmen, tritt der Fehler WrongFrameTypeError auf,</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;name&quot;]) -&gt; bool

Sperrt das Objekt &quot;name&quot;, wenn es freigegeben ist und entsperrt es, wenn es
gesperrt ist. Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.
Gibt True zurück, wenn das Objekt gesperrt ist.</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

Gibt True zurück, wenn &quot;name&quot; gesperrt ist. Ist &quot;name&quot; nicht angegeben, wird das aktuelle
Objekt verwendet.</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Gibt eine Liste mit allen verfügbaren Schriftarten zurück.</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; list

GIbt genaue Informationen zu einer Schriftart zurück mit den Werten:
[ (Name in Scribus, Familie, wirklicher Name, Subset (1|0), embed PS (1|0), Fontdatei), (...), ... ]</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list

Gibt eine Liste mit allen Ebenen zurück.</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;name&quot;)

Macht die Ebene &quot;name&quot; zur aktiven Ebene.

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf.
Ist der Name der Ebene ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

Gibt die aktive Ebene zurück.</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Verschiebt das Objekt &quot;name&quot; auf die Ebene &quot;layer&quot;. Die Ebene muss
vorhanden sein. Ist &quot;name&quot; nicht angegeben, wird das aktive Objekt benutzt.

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf.
Ist der Name der Ebene ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;layer&quot;, visible)

Zeigt die Ebene &quot;layer&quot; an oder versteckt sie. Ist &quot;visible&quot; auf False gesetzt,
ist die Ebene unsichtbar.

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf.
Ist der Name der Ebene ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;layer&quot;, printable)

Legt fest, ob die Ebene &quot;layer&quot; gedruckt werden soll oder nicht. False bedeutet, dass
die Ebene nicht gedruckt wird.

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf.
Ist der Name Name ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;layer&quot;)

Löscht die Ebene &quot;layer&quot;. Ist die Ebene nicht vorhanden oder ist nur eine
Ebene vorhanden, passiert gar nichts.

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf,
Ist der Name der Ebene ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(layer)

Erzeugt einen Ebene mit dem Namen &quot;layer&quot;.

Ist der Name ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

Gibt den Wert der Variable -lang zurück.</translation>
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

Erzeugt eine Ellipse auf der aktuellen Seite und gibt ihren Namen zurück. Die Koordinaten
werden in der Maßeinheit des Dokuments angegeben (siehe UNIT-Konstanten). &quot;name&quot; sollte
das Objekt eindeutig identifizieren, weil Sie den Namen für spätere Zwecke brauchen. Ist &quot;name&quot;
nicht angegeben, erzeugt Scribus einen Namen für das Objekt.

Ist der Name schon vorhanden, tritt der Fehler NameExistsError auf.</translation>
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

Erzeugt einen Bildrahmen auf der aktuellen Seite und gibt seinen Namen zurück.
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.</translation>
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

Erzeugt einen neuen Textrahmen auf der aktuellen Seite und gibt seinen Namen zurück.
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.</translation>
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

Erzeugt eine Linie von P(x1,y1) zu P(x2,y2) und gibt ihren Namen zurück.
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.</translation>
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

Erzeugt eine Mehrfachlinie und und gibt ihren Namen zurück. Die Punkte werden in
der folgenden Reihenfolge gespeichert: [x1, y1, x2,...xn, yn].
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.
Ist die Anzahl der Punkte nicht genügend, tritt der Fehler ValueError auf.</translation>
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

Erzeugt ein Vieleck und gibt seinen Namen zurück. Die Punkte werden in
der folgenden Reihenfolge gespeichert: [x1, y1, x2,...xn, yn]. Sie müssen
mindestens 3 Punkte angeben, aber Sie müssen den ersten Punkt nicht zum
Schließen des Polygons erneut angeben - das geschieht automatisch.
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.
Ist die Anzahl der Punkte nicht ausreichend, tritt der Fehler ValueError auf.</translation>
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

Erzeugt eine Bézierkurve und gibt ihren Namen zurück. Die Punkte werden in
der folgenden Reihenfolge gespeichert: 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]. 
x und y steht dabei für die X- und Y-Koordinaten und kx und ky steht für den
Kontrollpunkt der Kurve. 
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.
Ist die Anzahl der Punkte nicht ausreichend, tritt der Fehler ValueError auf.</translation>
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

Erzeugt Text auf einem Pfad, indem die Objekte &quot;textbox&quot; und &quot;beziercurve&quot;
zusammengefügt werden.
Die Koordinaten werden in der Maßeinheit des Dokuments angegeben (siehe
UNIT-Konstanten). &quot;name&quot; sollte das Objekt eindeutig identifizieren, weil Sie den
Namen für spätere Zwecke benötigen. Wenn Sie &quot;name&quot; nicht angeben, legt Scribus
einen Namen für das Objekt fest.

Ist das Objekt schon vorhanden, tritt der Fehler NameExistsError auf.
Sind ein oder beide Objekte nicht vorhanden, tritt der Fehler NotFoundError auf.</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;name&quot;])

Löscht das Objekt &quot;name&quot;. Ist &quot;name&quot; nicht angegeben, wird das aktuelle
Objekt gelöscht.</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;name&quot; [, state])

Aktiviert/Deaktiviert &quot;Text umfließt den Rahmen&quot; für das Objekt &quot;name.
&quot;state&quot; ist optional, wenn es nicht angegeben ist, wird der Status der
Option jeweils geändert.</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;name&quot;]) -&gt; bool

Gibt an, ob das Objekt mit dem Name &quot;name&quot; auch wirklich im aktuellen Dokument
existiert. &quot;name&quot; ist optional, ist &quot;name&quot; nicht angegeben, wird True ausgegeben, wenn
gerade ein Objekt selektiert ist.</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Setzt den Absatzstil &quot;style&quot; für das Objekt &quot;name&quot;. Ist kein Objekt angegeben,
wird der Stil auf das aktuelle Objekt angewendet.</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Gibt eine Listen mit allen Absatzstilen im aktuellen Dokument zurück.</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Gibt die Nummer der aktuellen Seite zurück. Seitenzahlen werden ab 1 gezählt, egal welche
Seitenzahl auf der aktuellen Seite angezeigt wird.</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Baut alle Seiten neu auf.</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;name&quot;)

Speichert die aktuelle Seite als EPS-Datei mit dem Namen &quot;name&quot;.

Schlägt das Speichern fehl, tritt der Fehler ScribusError auf.</translation>
    </message>
    <message>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation>deletePage(nr)

Löscht die Seite mit der Nummer &quot;nr&quot;. Besteht das Dokument nur aus einer Seite, passiert nichts.
Seitenzahlen werden von 1 aufwärts gezählt, egal, welche Seitenzahl Sie auf der aktuellen Seite
im Dokument sehen.

Ist die Seitenzahl ungültig, tritt der Fehler IndexError auf</translation>
    </message>
    <message>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation>gotoPage(nr)

Geht zur Seite &quot;nr&quot;, das heißt, die Seite &quot;nr&quot; wird zur aktuellen Seite. Zur Zeit ändert
sich allerdings nicht die Seite, die der User sieht, es wird nur intern die aktuelle
Seite geändert.

Ist die Seitenzahl ungültig, tritt der Fehler IndexError auf.</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; integer

Gibt die Anzahl der Seiten im Dokument zurück.</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

Gibt eine Liste zurück, die die Position der horizontalen Hilfslinien enthält. Die Werte
werden in der Maßeinheit des Dokuments angegeben, siehe UNIT-Konstanten.</translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(list)

Legt horizontale Hilfslinien fest, list muss eine Liste der Linien sein, angegeben in der Maßeinheit
des Dokuments, siehe UNIT-Konstanten.

Beispiel:
setHGuides(getHGuides() + [200.0, 210.0] # fügt eine neue Hilfslinie hinzu
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Siehe getHGuides.</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Siehe setHGuides.</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Gibt eine Liste mit der Seitengröße in der aktuellen Maßeinheit zurück, siehe
UNIT-Konstanten und getPageMargins()</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; list

Gibt eine Liste zurück mit den Objekten auf der aktuelle Seite. Die Reihenfolge
der Werte ist: (Name, Typ des Objekts, Anordnung).

Beispiel: [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)] bedeutet, dass das Objekt Text1 heißt, 
ein Textrahmen ist  (type 4) und sich auf der ersten Seite befindet...</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Setzt die Füllfarbe &quot;color&quot; für das Objekt &quot;name&quot;. &quot;color&quot; ist der Name einer
vorhandenen Farbe. Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt
verwendet.</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Setzt die Linienfarbe &quot;color&quot; für das Objekt &quot;name&quot;. Ist &quot;name&quot; nicht angegeben, 
wird das aktuelle Objekt verwendet.</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(width, [&quot;name&quot;])

Setzt die Linienbreite für das Objekt &quot;name&quot; auf &quot;width&quot;. &quot;width&quot; muss zwischen 
0.0 und 12.0 groß sein und wird in Punkt angegeben. Ist &quot;name&quot; nicht angegeben,
wird das aktuelle Objekt verwendet.

Ist die Linienbreite ungültig, tritt der Fehler ValueError auf.</translation>
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

Setzt den Tonwert der Linie des Objekts &quot;name&quot; auf &quot;shade&quot;. &quot;shade&quot; ist eine
ganze Zahl zwischen 0 und 100. Ist &quot;name&quot; nicht angegeben, wird das aktuelle 
Objekt verwendet.

Ist der Tonwert ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;name&quot;])

Setzt die Ecken der Linien des Objekts &quot;name&quot; auf den Stil join.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Obekt verwendet.
Benutzen Sie die vorgegeben Konstaten des Typs JOIN_*.</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>SetLineEnd(endtype, [&quot;name&quot;])

Setzt die Linienenden des Objekts &quot;name&quot; auf den Stil &quot;endtype&quot;.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.
Benutzen Sie die vorgegebenen Konstanten des Typs CAP_*.</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(style, [&quot;name&quot;])

Setzt den Linienstil des Objekts &quot;name&quot; auf den Stil &quot;style&quot;. Ist &quot;name&quot; nicht
angegeben, wird das aktuelle Objekt verwendet. Benutzen Sie die
vorgegebenen Konstanten des Typs LINE_*.</translation>
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

Setzt den Tonwert der Füllfarbe für das Objekt &quot;name&quot; auf &quot;shade&quot;.
&quot;shade&quot; muss eine ganze Zahl zwischen 0 und 100 sein. Ist &quot;name&quot;
nicht angegeben, wird das aktuelle Objekt benutzt.

Ist der Tonwert ungültig, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(radius, [&quot;name&quot;])

Setzt den Eckradius für das Objekt &quot;name&quot; auf radius. Der Radius wird
in Punkt angegeben. Ist &quot;name&quot; nicht angegeben, wird das aktuelle
Objekt verwendet.

Ist der Eckradius negativ, tritt der Fehler ValueError auf.</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Setzt den Linienstil für das Objekt &quot;name&quot; auf den angegebenen Stil.
Ist &quot;name&quot; nicht angegeben, wird der aktuelle Stil benutzt.

Ist der Stil nicht vorhanden, tritt der Fehler NotFoundError auf.</translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;name&quot;]) -&gt; string

Gibt den Namen der Schriftart für den Textrahmen &quot;name&quot; zurück. Ist in
dem Textfeld Text ausgewählt, wird die Schriftart des ersten gewählten
Zeichens angegeben. Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt
verwendet.</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer

Gibt die Länge des Textes im Textrahmen &quot;name&quot; zurück.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;name&quot;]) -&gt; string

Gibt den Text aus dem Textrahmen &quot;name&quot; zurück. Ist in dem Textfeld Text markiert,
wird der selektierte Text zurückgegeben. Der gesamte Text wird zurückgegeben, nicht
nur der sichtbare Teil des Textes, Ist &quot;name&quot; nicht angegeben, wird das aktuelle
Objekt verwendet.</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>getAllText([&quot;name&quot;]) -&gt; string

Gibt den Text aus dem Textrahmen &quot;name&quot; zurück und aus allen Rahmen, die mit
dem Rahmen &quot;name&quot; verbunden sind. Ist in dem Textfeld Text markiert,
wird der selektierte Text zurückgegeben. Ist &quot;name&quot; nicht angegeben, wird das aktuelle
Objekt verwendet.</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float

Gibt den Zeilenabstand für den Textrahmen &quot;name&quot; in Punkten zurück.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float

Gibt den Zeilenabstand für den Text im Rahmen &quot;name&quot; in Punkten zurück. Ist
&quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer

Gibt die Anzahl der Spalten im Textrahmen &quot;name&quot; zurück. Ist &quot;name&quot; nicht
angegeben, wird das aktuelle Objekt verwendet.</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;text&quot;, [&quot;name&quot;])

Ändert den Text im Rahmen &quot;name&quot; auf den String &quot;text&quot;.
Der Text muss URF8-kodiert sein - verwenden Sie z.B. unicode(text, &apos;iso-8859-2&apos;).
Lesen Sie in der FAQ nach für weitere Informationen. Ist &quot;name&quot; nicht angegeben, 
wird der aktuelle Textrahmen verwendet.</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation>setFont(&quot;font&quot;, [&quot;name&quot;])

Setzt die Schriftart für den Rahmen &quot;name&quot; auf &quot;font&quot;. Ist Text ausgewählt,
wird nur die Markierung verändert. Ist &quot;name&quot; nicht angegeben, wird der
aktuelle Textrahmen verwendet.

Ist die Schriftart ungültig, wird der Fehler ValueError ausgegeben.</translation>
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

Setzt die Schriftgröße im Textrahmen &quot;name&quot; auf den Wert &quot;size&quot;. &quot;size&quot; ist
ein Wert in Punkten zwischen 1 und 512. Ist eine Markierung vorhanden, wird
nur die die Markiertung verändert. Ist &quot;name&quot; nicht angegeben, wird der aktuelle
Textrahmen verwendet.

Ist die Schriftgröße ungültig, wird der Fehler ValueError ausgegeben.</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(size, [&quot;name&quot;])

Setzt den Zeilenabstand im Rahmen &quot;name&quot; auf den Wert &quot;size&quot;.
&quot;size&quot; ist ein Wert in Punkten. Ist &quot;name&quot; nicht angegeben, wird der
aktuelle Textrahmen verwendet.

Ist der Wert für Zeilenabstand ungültig, wird der Fehler ValueError
ausgegeben.</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(size, [&quot;name&quot;])

Setzt den Spaltenabstand im Rahmen &quot;name&quot; auf den Wert &quot;size&quot;.
Ist &quot;name&quot; nicht angegeben, wird der aktuelle Textrahmen verwendet.

Ist der Abstand nicht positiv, wird der Fehler ValueError ausgegeben.</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(nr, [&quot;name&quot;])

Legt die Anzahl der Spalten im Textrahmen &quot;name&quot; fest auf &quot;nr&quot; fest.
Wenn &quot;name&quot; nicht angegeben ist, wird der aktuelle Textrahmen verwendet.

Ist die Anzahl der Spalten kleiner als 1, wird der Fehler ValueError ausgegeben.</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(align, [&quot;name&quot;])

Richtet den Text in dem Objekt &quot;name&quot; nach der Art &quot;align&quot; aus. Ist &quot;name&quot;
nicht angegeben, wird der aktuelle Textrahmen verwendet. &quot;align&quot; ist eine der
ALIGN_*-Konstanten. Siehe dir(scribus).

Bei einem falschen Argument wird der Fehler ValueError ausgegeben.</translation>
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

Markiert &quot;count&quot; Zeichen des Textrahmens &quot;name&quot; beginnend mit dem Zeichen &quot;start&quot;. 
Die Zählung beginnt bei 0. Wenn &quot;count&quot;=0 ist, wird die Markierung gelöscht. Wenn &quot;name&quot;
nicht angegeben ist, wird der aktuelle Textrahmen verwendet.

Wenn die Werte ungültig sind, wird der Fehler IndexError ausgegeben.</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;name&quot;])

Löscht den Textrahmen &quot;name&quot;. Wenn Text ausgewählt ist, wird nur die Markierung
gelöscht. Ist &quot;name&quot; nicht angegeben, wird der aktuelle Textrahmen gelöscht.</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Ändert die Farbe des Textes auf &quot;color&quot;. Wenn Text ausgewählt ist, wird die
Markierung auf &quot;color&quot; geändert. Ist &quot;name&quot; nicht angegeben, wird der aktuelle
Textrahmen verwendet.</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Setzt die Farbe &quot;color&quot; für die Umrandung des Textes. Ist &quot;name&quot; nicht angegeben, wird der aktuelle 
Textrahmen verwendet.</translation>
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

Setzt den Tonwert für den Textrahmen &quot;name&quot; auf &quot;shade&quot;.
Ist Text in dem Rahmen ausgewählt, wird nur der ausgewählt Text
verändert. &quot;shade&quot; muss eine ganze Zahl zwischen 0 und 100 sein.
Ist &quot;name&quot; nicht vorhanden oder angegeben, wird der aktuelle Rahmen verwendet.</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Zwei Textrahmen verketten. &quot;Fromname&quot; wird mit &quot;Toname&quot; verkettet. Der Zielrahmen
muss leer sein und darf nicht mit anderen Rahmen verkettet sein.

Werden die Regeln verletzt, erscheint der Fehler ScribusException.</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;name&quot;)

Entfernt den Textrahmen &quot;name&quot; aus der Reihe der verketteten Rahmen. Ist der
zu entfernende Rahmen einer aus der Mitte der Reihe, wird der vorherige und nächste
Rahmen miteinander verkettet, z.B. wird aus a-&gt;b-&gt;c dann a-&gt;c, wenn Sie Rahmen b
entfernen.

Wenn die Verkettungsregeln gebrochen werden, erscheint der Fehler ScribusException.</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;name&quot;])

Konvertiert den Textrahmen &quot;name&quot; in Umrisse. Ist &quot;name&quot; nicht vorhanden oder 
vorgegeben, wird der aktuelle Textrahmen verwendet.</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Löscht die Fortschrittsanzeige. Wird aufgerufen, bevor eine neue Verlaufsanzeige benutzt wird.
Siehe progressSet</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Setzt die Gesamtzahl der Schritte für die Fortschrittsanzeige. Siehe progressSet.</translation>
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

Die Fortschrittsanzeigen auf &quot;nr&quot; setzen, ein Wert relativ zum vorher festgelegten Wert
für progressTotal. Die Fortschrittsanzeige verwendet Schritte zum Anzeigen des Verlaufs:
Sie geben die Gesamtzahl der Schritte an und dann immer die fertigen Schritte - die Verlaufs-
anzeige zeigt immer den Prozentsatz der fertigen Schritte an. Die Gesamtzahl der Schritte können
Sie mit progressTotal() festlegen, die aktuelle Zahl der Schritte mit progressSet().
Die Fortschrittsanzeige kann mit progressReset() zurückgesetzt werden. [Infos sind aus den 
Dokumenten von Trolltech Qt]</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[nicht unterstützt!] Bitte verwenden Sie diese Funktion zur Zeit noch nicht.</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Aktiviert/Deaktivert das Icon zum Speichern und den entsprechenden Eintrag im Datei-Menü.
Wenn Sie ein Dokument verändert ist es hilfreich, diese Funktion aufzurufen, weil Scribus im Script-
Modus nicht automatisch erkennt, wann ein Dokument verändert wurde.</translation>
    </message>
    <message>
        <source>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Sets the scale to frame on the selected or specified image frame to `scaletoframe&apos;.
If `proportional&apos; is specified, set fixed aspect ratio scaling to `proportional&apos;.
Both `scaletoframe&apos; and `proportional&apos; are boolean.

May raise WrongFrameTypeError.
</source>
        <translation>setScaleImageToFrame(scaletoframe, proportional=None, name=&lt;selection&gt;)

Setzt die Eigenschaft An Rahmen anpassen für den gewählten Bildrahmen oder angegebenen
Bildrahmen.
&apos;proportional&apos; verkleinert das Bild proportional zur ursprünglichen Größe.
Sowohl &apos;scaletoframe&apos; als auch &apos;proportional&apos; sind boolsche Werte.

Kann den Fehler WrongFrameTypeError verursachen.</translation>
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

Gibt zurück, ob die Ebene gedruckt wird oder nicht. True bedeutet, dass die Ebene
gedruckt wird, False bedeutet, dass das Drucken deaktiviert ist.

Der Fehler NotFoundError tritt auf, wenn die Ebene nicht existiert.
Der Fehler ValueError tritt auf, wenn der Ebenenname ungültig ist.</translation>
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

Erzeugt ein Bild mit dem Text &quot;sample&quot; in der Schriftart &quot;font&quot; und der Größe &quot;size&quot;.
Ist &quot;filename&quot; angegeben, wird das Bild unter &quot;filename&quot; abgespeichert. Ansonsten
werden die Bilddaten als String zurückgegeben. Das optionale Argument &quot;format&quot; gibt 
an, welches Bildformat generiert werden soll. Unterstützt werden die Formate, die auch 
QPixmap.save() kennt, zum Beispiel PPM, JPEG, PNG und XPM.

Der Fehler NotFoundError tritt auf, wenn die Schriftart nicht gefunden wird.
Der Fehler ValueError tritt auf, wenn &quot;sample&quot; leer ist oder der Dateiname ungültig ist.</translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Legt fest (toggle=1), ob der Textrahmen &quot;name&quot; ein Lesezeichen ist oder nicht.
Ist &quot;name&quot; nicht angegeben, wird der aktuelle Textrahmen verwendet.

Der Fehler WrongFrameTypeError tritt auf, wenn der Zielrahmen kein Textrahmen ist</translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Gibt zurück, ob der Textrahmen &quot;name&quot; ein PDF-Lesezeichen ist.
Ist &quot;name&quot; nicht angegeben, wird der aktuelle Textrahmen verwendet.

Der Fehler WrongFrameTypeError tritt auf, wenn der Zielrahmen kein Textrahmen ist</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Gibt die Seitenränder als ein Tupel in der gerade aktiven Einheit zurück (oben, unten, links, rechts).
Mögliche Einheiten finden Sie bei UNIT_&lt;tye&gt; und getPageSize().
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
        <translation>defineColor(&quot;name&quot;, c, m, y, k)

Definiert die neue Farbe &quot;name&quot;. Der Farbwert besteht aus vier Farbkomponenten:
c = Cyan, m = Magenta, y = Yello and k = Schwarz. Farbkomponenten sollten
im Bereich zwischen 0 und 255 liegen.

Könnte einen ValueError hervorrufen, wenn ein ungültiger Name definiert wird.</translation>
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
        <translation>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot;, haspreview, issave, isdir]) -&gt; string with filename

Zeigt einen Datei öffnen-Dialog mit der Beschriftung &quot;caption&quot;. Dateien werden
gefiltert mit dem String &quot;filter&quot;. Ein Standard-Dateiname oder -pfad kann auch
angegeben werden. Lassen Sie diesen String leer, wenn Sie ihn nicht verwenden
möchten. Der Wert &quot;True&quot; für &quot;haspreview&quot; aktiviert die Dateivorschau im FileSelect-
Dialog. Wenn der &quot;issave&quot;-Parameter den Wert &quot;True&quot; hat, verhält sich der Dialog wie
der &quot;Speichern unter&quot;-Dialog, ansonsten wie der &quot;Date öffnen&quot;-Dialog. Wenn der &quot;isdir&quot;-
Parameter den Wert &quot;True&quot; hat, zeigt der Dialog lediglich Verzeichnisse an. Der Standard-
wert aller optionalen Parameter ist &quot;False&quot;</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius isexpressed in points. If &quot;name&quot; is not given the currentlyselected item is used.
</source>
        <translation>getCornerRadius([&quot;name&quot;]) -&gt; integer

Gibt den Eckenradius des Objekts &quot;name&quot; zurück. Der Radius wird in Punkten ausgedrückt. 
Falls &quot;name&quot; nicht definiert ist, wird das momentan ausgewählte Objekt verwendet.</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
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
        <source>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.
</source>
        <translation>getPropertyCType(object, property, includesuper=True)

Returns the name of the C type of `property&apos; of `object&apos;. See getProperty()
for details of arguments.

If `includesuper&apos; is true, search inherited properties too.</translation>
    </message>
    <message>
        <source>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.
</source>
        <translation>getPropertyNames(object, includesuper=True)

Return a list of property names supported by `object&apos;.
If `includesuper&apos; is true, return properties supported
by parent classes as well.</translation>
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
        <translation>getProperty(object, property)

Return the value of the property `property&apos; of the passed `object&apos;.

The `object&apos; argument may be a string, in which case the named PageItem
is searched for. It may also be a PyCObject, which may point to any
C++ QObject instance.

The `property&apos; argument must be a string, and is the name of the property
to look up on `object&apos;.

The return value varies depending on the type of the property.</translation>
    </message>
    <message>
        <source>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.
</source>
        <translation>setProperty(object, property, value)

Set `property&apos; of `object&apos; to `value&apos;. If `value&apos; cannot be converted to a type
compatible with the type of `property&apos;, an exception is raised. An exception may
also be raised if the underlying setter fails.

See getProperty() for more information.</translation>
    </message>
    <message>
        <source>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.
</source>
        <translation>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.</translation>
    </message>
    <message>
        <source>getChild(object, childname, ofclass=None, recursive=True)

Return the first child of `object&apos; named `childname&apos;, possibly restricting
the search to children of type name `ofclass&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.
</source>
        <translation>getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)

Return a list of children of `object&apos;, possibly restricted to children
of class named `ofclass&apos; or children named `ofname&apos;. If `recursive&apos; is true,
search recursively through children, grandchildren, etc.

See QObject::children() in the Qt docs for more information.</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positve values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.</translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.</translation>
    </message>
    <message>
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference) The first character has an
index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot; is
not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.</translation>
    </message>
    <message>
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable</translation>
    </message>
    <message>
        <source>newStyleDialog() -&gt; string

Shows &apos;Create new paragraph style&apos; dialog. Function returns real
style name or None when user cancels the dialog.
</source>
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.</translation>
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
        <translation>newDoc(size, margins, orientation, firstPageNumber,
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
</translation>
    </message>
    <message>
        <source>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().
</source>
        <translation>closeMasterPage()

Closes the currently active master page, if any, and returns editing
to normal. Begin editing with editMasterPage().</translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation>masterPageNames()(new line)
(new line)
Returns a list of the names of all master pages in the document.</translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation>editMasterPage(pageName)(new line)
(new line)
Enables master page editing and opens the named master page(new line)
for editing. Finish editing with closeMasterPage().</translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation>createMasterPage(pageName)(new line)
(new line)
Creates a new master page named pageName and opens it for(new line)
editing.</translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation>deleteMasterPage(pageName)(new line)
(new line)
Delete the named master page.</translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation>zoomDocument(double)(new line)
(new line)
Zoom the document in main GUI window. Actions have whole number(new line)
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.</translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Build-ID:</source>
        <translation type="obsolete">Build-ID:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Beiträge von:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>Ü&amp;ber</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utoren</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>Über&amp;setzungen</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Online</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Entwicklerteam:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Offizielle Dokumentation:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Andere Dokumentationen:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Homepage</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Onlinereferenz</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Fehler und Featurewünsche</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Mailingliste</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Offizielle Übersetzungen und Übersetzer:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Ehemalige Übersetzer:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Über Scribus %1</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="obsolete">%1 %2 %3</translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation type="obsolete">Windows-Portierung:</translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation type="obsolete">Portierung auf Mac OS/X Aqua:</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="obsolete">Ghostscript-Version %1</translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="obsolete">Keine Ghostscript-Version verfügbar</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="obsolete">Scribus-Version %1
%2 %3 (%4)</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="obsolete">Diese Leiste zeigt die Version, das Build-Datum und die kompilierte Unterstützung für Programmbibliotheken. C-C-T-F bedeutet im einzelnen: 
C= littleCMS C=CUPS, T=TIFF-Unterstützung, F=Fontconfig-Unterstützung. Der zusätzliche Buchstabe am Ende bezeichnet den Renderer, nämlich entweder C (Cairo) oder A (libart). Wird eine der Bibliotheken nicht unterstützt, steht statt dessen ein *. Hier wird auch die von Scribus gefundene Ghostscript-Version angezeigt.</translation>
    </message>
    <message>
        <source>November</source>
        <translation type="obsolete">November</translation>
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
        <translation>Ghostscript-Version: %1</translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation>Ghostscript ist nicht vorhanden</translation>
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
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation>Dieser Dialog zeigt die Version, das Datum der Kompilierung und die vorhandenen Bibliotheken in Scribus an. C-C-T-F steht für C=CUPS-Support, T=TIFF-Support, F=Fontconfig-Support. Der letzte Buchstabe steht für die Rendering Engine, C=Cairo oder A=libart. Fehlende Bibliotheken sind mit * markiert. Auch die Ghostscript-Version wird angezeigt.</translation>
    </message>
    <message>
        <source>December</source>
        <translation type="obsolete">Dezember</translation>
    </message>
    <message>
        <source>January</source>
        <translation type="obsolete">Januar</translation>
    </message>
    <message>
        <source>February</source>
        <translation type="obsolete">Februar</translation>
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
        <source>March</source>
        <translation>März</translation>
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
        <translation>Nein</translation>
    </message>
    <message>
        <source>Filename:</source>
        <translation>Dateiname:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation>Version:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation>Aktiviert:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation>Datum der Veröffentlichung:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beschreibung:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation>Autor(en):</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Urheberrecht:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation>Lizenz:</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation>Scribus: Über die Plug-Ins</translation>
    </message>
    <message>
        <source>File Name:</source>
        <translation type="obsolete">Dateiname:</translation>
    </message>
    <message>
        <source>Version:</source>
        <translation type="obsolete">Version:</translation>
    </message>
    <message>
        <source>Enabled:</source>
        <translation type="obsolete">Aktiviert:</translation>
    </message>
    <message>
        <source>Release Date:</source>
        <translation type="obsolete">Datum der Veröffentlichung:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="obsolete">Urheberrecht:</translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="obsolete">Autor(en):</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="obsolete">Beschreibung:</translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="obsolete">Lizenz:</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
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
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>Ö&amp;ffnen...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Neu la&amp;den</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Für Aus&amp;gabe sammeln...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Text laden...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Text anfügen...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation>Bild laden...</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>&amp;Text speichern...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Seite als &amp;EPS speichern...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Als &amp;PDF speichern...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokument&amp;einstellungen...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Drucken...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>&amp;Beenden</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation>&amp;Objektbezogener Modus</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>Inhalt lösc&amp;hen</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Alles aus&amp;wählen</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation>Alles auswählen &amp;rückgängig machen</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Suchen &amp;&amp; Ersetzen...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation>Bild bearbeiten...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>&amp;Farben...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>&amp;Absatzstile...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Linienstile...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation>&amp;Musterseiten...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;Javascripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>S&amp;cribus-Einstellungen...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation>%1 pt</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Andere...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Links</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Zentriert</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>&amp;Rechts</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Blocksatz</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Erzwungener Blocksatz</translation>
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
        <translation>&amp;Unterstrichen</translation>
    </message>
    <message>
        <source>Underline &amp;Words</source>
        <translation>Wörter unterst&amp;richen</translation>
    </message>
    <message>
        <source>&amp;Strike Through</source>
        <translation>&amp;Durchgestrichen</translation>
    </message>
    <message>
        <source>&amp;All Caps</source>
        <translation>&amp;Großbuchstaben</translation>
    </message>
    <message>
        <source>Small &amp;Caps</source>
        <translation>&amp;Kapitälchen</translation>
    </message>
    <message>
        <source>Su&amp;perscript</source>
        <translation>&amp;Hochgestellt</translation>
    </message>
    <message>
        <source>Su&amp;bscript</source>
        <translation>&amp;Tiefgestellt</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">D&amp;okumentstruktur</translation>
    </message>
    <message>
        <source>S&amp;hadow</source>
        <translation>&amp;Schatten</translation>
    </message>
    <message>
        <source>&amp;Image Effects</source>
        <translation>&amp;Bild-Effekte</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatoren...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>&amp;Mehrfach Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Gruppieren</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Gruppe auf&amp;lösen</translation>
    </message>
    <message>
        <source>Is &amp;Locked</source>
        <translation>Ge&amp;sperrt</translation>
    </message>
    <message>
        <source>Si&amp;ze is Locked</source>
        <translation>Größen&amp;änderung gesperrt</translation>
    </message>
    <message>
        <source>Lower to &amp;Bottom</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>Raise to &amp;Top</source>
        <translation>In den &amp;Vordergrund</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>Weiter nach hin&amp;ten</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>Weiter nach vor&amp;n</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>In &amp;Bibliothek speichern</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation>&amp;Attribute...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Bild an&amp;zeigen</translation>
    </message>
    <message>
        <source>&amp;Update Image</source>
        <translation>Bild &amp;aktualisieren</translation>
    </message>
    <message>
        <source>Adjust Frame to Image</source>
        <translation>Rahmen an Bild anpassen</translation>
    </message>
    <message>
        <source>Extended Image Properties</source>
        <translation>Erweiterte Bildeigenschaften</translation>
    </message>
    <message>
        <source>&amp;Low Resolution</source>
        <translation>&amp;Niedrige Auflösung</translation>
    </message>
    <message>
        <source>&amp;Normal Resolution</source>
        <translation>Nor&amp;male Auflösung</translation>
    </message>
    <message>
        <source>&amp;Full Resolution</source>
        <translation>&amp;Hohe Auflösung</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Als PDF-&amp;Lesezeichen verwenden</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Als PDF-&amp;Anmerkung verwenden</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Eigenschaften der An&amp;merkung</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>&amp;Eigenschaften des Feldes</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Form bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>Text an Pfad aus&amp;richten</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>Text von Pfad l&amp;ösen</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>Polygone &amp;kombinieren</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Polygone &amp;teilen</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Bézier&amp;kurve</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation>&amp;Bildrahmen</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Umrisse</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation>&amp;Polygon</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textrahmen</translation>
    </message>
    <message>
        <source>&amp;Glyph...</source>
        <translation>&amp;Zeichen...</translation>
    </message>
    <message>
        <source>Sample Text</source>
        <translation>Beispieltext</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>Ein&amp;fügen...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation>&amp;Importieren...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Löschen...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation>&amp;Kopieren...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>Ver&amp;schieben...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation>Musterseite an&amp;wenden...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Hilfslinien bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Fit in window</source>
        <translation>An &amp;Fenster anpassen</translation>
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
        <translation>&amp;Vorschaubilder</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;Ränder anzeigen</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Ra&amp;hmen anzeigen</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>&amp;Bilder anzeigen</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Ra&amp;ster anzeigen</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;Hilfslinien anzeigen</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Grundlinienraster anzeigen</translation>
    </message>
    <message>
        <source>Show &amp;Text Chain</source>
        <translation>Ver&amp;kettete Textrahmen anzeigen</translation>
    </message>
    <message>
        <source>Show Control Characters</source>
        <translation>Kontrollzeichen anzeigen</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Am &amp;Raster ausrichten</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>An &amp;Hilfslinien ausrichten</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Eigenschaftenpalette</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>Biblio&amp;thek</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>E&amp;benen</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>&amp;Measurements</source>
        <translation>&amp;Abstandsmesser</translation>
    </message>
    <message>
        <source>Action &amp;History</source>
        <translation>Aktions&amp;verlauf</translation>
    </message>
    <message>
        <source>Preflight &amp;Verifier</source>
        <translation>&amp;Druckvorstufenüberprüfung</translation>
    </message>
    <message>
        <source>&amp;Align and Distribute</source>
        <translation>&amp;Ausrichten und Verteilen</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Werkzeuge</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>&amp;PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Select Item</source>
        <translation>Eintrag auswählen</translation>
    </message>
    <message>
        <source>T&amp;able</source>
        <translation>Ta&amp;belle</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linie</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation>Frei&amp;handlinie</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Objekt drehen</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Vergrößern oder verkleinern</translation>
    </message>
    <message>
        <source>Zoom in</source>
        <translation>Vergrößern</translation>
    </message>
    <message>
        <source>Zoom out</source>
        <translation>Verkleinern</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Rahmeninhalt bearbeiten</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Text bearbeiten...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Textrahmen verketten</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Verkettete Textrahmen trennen</translation>
    </message>
    <message>
        <source>&amp;Eye Dropper</source>
        <translation>Farb&amp;wähler</translation>
    </message>
    <message>
        <source>Copy Item Properties</source>
        <translation>Eigenschaften übertragen</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Text im Story Editor bearbeiten</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Textrahmen einfügen</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation>Bildrahmen einfügen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Tabelle einfügen</translation>
    </message>
    <message>
        <source>Insert Shape</source>
        <translation>Form einfügen</translation>
    </message>
    <message>
        <source>Insert Polygon</source>
        <translation>Polygon einfügen</translation>
    </message>
    <message>
        <source>Insert Line</source>
        <translation>Linie einfügen</translation>
    </message>
    <message>
        <source>Insert Bezier Curve</source>
        <translation>Bézierkurve einfügen</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Freihandlinie einfügen</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Bilder &amp;verwalten</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>Silben&amp;trennung anwenden</translation>
    </message>
    <message>
        <source>&amp;Generate Table Of Contents</source>
        <translation>&amp;Inhaltsverzeichnis erstellen</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Über Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Über &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Tool&amp;tipps</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;Handbuch für Scribus...</translation>
    </message>
    <message>
        <source>Smart &amp;Hyphen</source>
        <translation>Bedingtes &amp;Trennzeichen</translation>
    </message>
    <message>
        <source>Non Breaking Dash</source>
        <translation>Geschützter Bindestrich</translation>
    </message>
    <message>
        <source>Non Breaking &amp;Space</source>
        <translation>Geschütztes &amp;Leerzeichen</translation>
    </message>
    <message>
        <source>Page &amp;Number</source>
        <translation>Seiten&amp;zahl</translation>
    </message>
    <message>
        <source>New Line</source>
        <translation>Zeilenumbruch</translation>
    </message>
    <message>
        <source>Frame Break</source>
        <translation>Rahmenumbruch</translation>
    </message>
    <message>
        <source>Column Break</source>
        <translation>Spaltenumbruch</translation>
    </message>
    <message>
        <source>Copyright</source>
        <translation>Copyright</translation>
    </message>
    <message>
        <source>Registered Trademark</source>
        <translation>Registriertes Warenzeichen</translation>
    </message>
    <message>
        <source>Trademark</source>
        <translation>Warenzeichen</translation>
    </message>
    <message>
        <source>Bullet</source>
        <translation>Aufzählung</translation>
    </message>
    <message>
        <source>Em Dash</source>
        <translation>Geviertstrich</translation>
    </message>
    <message>
        <source>En Dash</source>
        <translation>Halbgeviertstrich</translation>
    </message>
    <message>
        <source>Figure Dash</source>
        <translation>Zahlenstrich</translation>
    </message>
    <message>
        <source>Quotation Dash</source>
        <translation>Zitatstrich</translation>
    </message>
    <message>
        <source>Apostrophe</source>
        <translation>Apostroph</translation>
    </message>
    <message>
        <source>Straight Double</source>
        <translation>Zollzeichen</translation>
    </message>
    <message>
        <source>Single Left</source>
        <translation>Einfaches Anführungszeichen rechts</translation>
    </message>
    <message>
        <source>Single Right</source>
        <translation>Einfaches Anführungszeichen rechts (Englisch)</translation>
    </message>
    <message>
        <source>Double Left</source>
        <translation>Doppeltes Anführungszeichen rechts</translation>
    </message>
    <message>
        <source>Double Right</source>
        <translation>Doppeltes Anführungszeichen rechts (Englisch)</translation>
    </message>
    <message>
        <source>Single Reversed</source>
        <translation>Anführungszeichen links (Englisch) gespiegelt</translation>
    </message>
    <message>
        <source>Double Reversed</source>
        <translation>Doppeltes Anführungszeichen links (Englisch) gespiegelt</translation>
    </message>
    <message>
        <source>Single Left Guillemet</source>
        <translation>&lt;</translation>
    </message>
    <message>
        <source>Single Right Guillemet</source>
        <translation>&gt;</translation>
    </message>
    <message>
        <source>Double Left Guillemet</source>
        <translation>&lt;&lt;</translation>
    </message>
    <message>
        <source>Double Right Guillemet</source>
        <translation>&gt;&gt;</translation>
    </message>
    <message>
        <source>Low Single Comma</source>
        <translation>Einfaches Anführungszeichen links</translation>
    </message>
    <message>
        <source>Low Double Comma</source>
        <translation>Doppeltes Anführungszeichen links</translation>
    </message>
    <message>
        <source>Double Turned Comma</source>
        <translation type="obsolete">Double Turned Comma</translation>
    </message>
    <message>
        <source>CJK Single Left</source>
        <translation>Einfaches Anführungszeichen links (CJK)</translation>
    </message>
    <message>
        <source>CJK Single Right</source>
        <translation>Einfaches Anführungszeichen rechts (CJK)</translation>
    </message>
    <message>
        <source>CJK Double Left</source>
        <translation>Doppeltes Anführungszeichen links (CJK)</translation>
    </message>
    <message>
        <source>CJK Double Right</source>
        <translation>Doppeltes Anführungszeichen rechts (CJK)</translation>
    </message>
    <message>
        <source>Toggle Palettes</source>
        <translation>Paletten ändern</translation>
    </message>
    <message>
        <source>Toggle Guides</source>
        <translation>Hilfslinien ändern</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation>&amp;Seiten anordnen</translation>
    </message>
    <message>
        <source>Dehyphenate Text</source>
        <translation>Silbentrennung rückgängig machen</translation>
    </message>
    <message>
        <source>Manage Page Properties...</source>
        <translation>Eigenschaften der Seite bearbeiten...</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Lineal relativ zur Seite</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>D&amp;ruckvorschau</translation>
    </message>
    <message>
        <source>&amp;JavaScripts...</source>
        <translation>&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Convert to Master Page...</source>
        <translation>In Musterseite umwandeln...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Hintereinander</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Nebeneinander</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation>&amp;Über Plug-Ins</translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation>Erweitert...</translation>
    </message>
    <message>
        <source>Copy Contents</source>
        <translation>Inhalt kopieren</translation>
    </message>
    <message>
        <source>Paste Contents</source>
        <translation>Inhalt einfügen</translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation>&amp;Drucken aktiviert</translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation>Horizontal &amp;spiegeln</translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation>Vertikal &amp;spiegeln</translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation>Lineale anzeigen</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation>D&amp;okumentstruktur</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation>Schrägstrich</translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation>Mittepunkt</translation>
    </message>
    <message>
        <source>En Space</source>
        <translation>En-Abstand</translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation>Em-Abstand</translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation>Sehr enge Grundschriftweite</translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation>Weite Grundschriftweite</translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation>Enge Grundschriftweite</translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation>Extrem enge Grundschriftweite</translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation>Bedingten Trennstrich einfügen</translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation>Geschützten Bindestrich einfügen</translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation>Geschütztes Leerzeichen einfügen</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Seitenzahl einfügen</translation>
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
        <source>Paste Contents (Absolute)</source>
        <translation>Inhalt einfügen (absolut)</translation>
    </message>
    <message>
        <source>S&amp;tyles...</source>
        <translation>&amp;Stile...</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>type effect</comment>
        <translation>&amp;Umriss</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <comment>Convert to oulines</comment>
        <translation>&amp;Umrisse</translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Weitere Optionen</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Erstellt eine PostScript-Level 3 Datei</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Erstellt eine PostScript-Level 2 Datei. Vorsicht:
hierbei können sehr große Dateien entstehen</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Erstellt eine PostScript-Level 1 Datei. Vorsicht:
hierbei können sehr große Dateien entstehen</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Seiten &amp;horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Seiten &amp;vertikal spiegeln</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">&amp;ICC-Profile anwenden</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">PostScript-Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">PostScript-Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">PostScript-Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Unterfarben&amp;reduktion durchführen</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Mit dieser Option werden ein paar Grautöne, die entstehen,
wenn Black aus Cyan, Magenta und Yellow gemischt wird, durch Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr nahe stehen. Diese Option kann zu besseren Bildern führen,
allerdings müssen Sie von Fall zu Fall entscheiden, wie Sie bessere Ergebnisse
bekommen. Außerdem reduziert UFR die Gefahr einer Übersättigung mit CMY.
</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="obsolete">Mediengröße festlegen</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Abstand/Ausrichtung</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Ausrichten</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Waagrecht</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Linken Kanten</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Mitten</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Rechten Kanten</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertikal</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Oberen Kanten</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Unteren Kanten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">An&amp;wenden</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;zwischen:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">&amp;Ausrichten</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">&amp;Versatz</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">&amp;Gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">zwi&amp;schen:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">&amp;Nicht verändern</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">A&amp;usrichten</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">Ve&amp;rsatz</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">Gleich&amp;mäßig verteilen</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">N&amp;icht verändern</translation>
    </message>
</context>
<context>
    <name>AlignDistributePalette</name>
    <message>
        <source>Align and Distribute</source>
        <translation>Ausrichten und Verteilen</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Ausrichten</translation>
    </message>
    <message>
        <source>&amp;Relative to:</source>
        <translation>&amp;Relativ zu:</translation>
    </message>
    <message>
        <source>First Selected</source>
        <translation>Zuerst markiertes</translation>
    </message>
    <message>
        <source>Last Selected</source>
        <translation>Zuletzt markiertes</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hilfslinien</translation>
    </message>
    <message>
        <source>Selection</source>
        <translation>Markierung</translation>
    </message>
    <message>
        <source>Align right sides of objects to left side of anchor</source>
        <translation>Rechte Seiten von Objekten an der linken Seite des gewählten Ankers ausrichten</translation>
    </message>
    <message>
        <source>Align left sides of objects to right side of anchor</source>
        <translation>Linke Seiten von Objekten an der rechten Seite des gewählten Ankers ausrichten</translation>
    </message>
    <message>
        <source>Align bottoms</source>
        <translation>Untere Seiten ausrichten</translation>
    </message>
    <message>
        <source>Align right sides</source>
        <translation>Rechte Seiten ausrichten</translation>
    </message>
    <message>
        <source>Align tops of objects to bottom of anchor</source>
        <translation>Oberseite von Objekten an der Unterseite des gewählten Ankers ausrichten</translation>
    </message>
    <message>
        <source>Center on vertical axis</source>
        <translation>Auf vertikaler Achse zentrieren</translation>
    </message>
    <message>
        <source>Align left sides</source>
        <translation>Linke Seiten ausrichten</translation>
    </message>
    <message>
        <source>Center on horizontal axis</source>
        <translation>Auf horizontaler Achse zentrieren</translation>
    </message>
    <message>
        <source>Align bottoms of objects to top of anchor</source>
        <translation>Unterseite von Objekten an der Oberseite des gewählten Ankers ausrichten</translation>
    </message>
    <message>
        <source>Align tops</source>
        <translation>Obere Seiten ausrichten</translation>
    </message>
    <message>
        <source>&amp;Selected Guide:</source>
        <translation>&amp;Ausgewählte Hilfslinie:</translation>
    </message>
    <message>
        <source>Distribute</source>
        <translation>Verteilen</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal</source>
        <translation>Gleicher horizontaler Abstand zwischen Objekten</translation>
    </message>
    <message>
        <source>Make horizontal gaps between objects equal to the value specified</source>
        <translation>Horizontaler Abstand zwischen Objekten wie angegeben</translation>
    </message>
    <message>
        <source>Distribute right sides equidistantly</source>
        <translation>Rechte Seiten gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Distribute bottoms equidistantly</source>
        <translation>Unterseiten gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly horizontally</source>
        <translation>Objektmitten gleichmäßig horizontal verteilen</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal</source>
        <translation>Gleicher vertikaler Abstand zwischen Objekten</translation>
    </message>
    <message>
        <source>Make vertical gaps between objects equal to the value specified</source>
        <translation>Vertikaler Abstand zwischen Objekten wie angegeben</translation>
    </message>
    <message>
        <source>Distribute left sides equidistantly</source>
        <translation>Linke Seiten gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Distribute centers equidistantly vertically</source>
        <translation>Objektmitten gleichmäßig vertikal verteilen</translation>
    </message>
    <message>
        <source>Distribute tops equidistantly</source>
        <translation>Oberseiten gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>&amp;Distance:</source>
        <translation>Ab&amp;stand:</translation>
    </message>
    <message>
        <source>Distribute the items with the distance specified</source>
        <translation>Verteilung der Objekte wie angegeben</translation>
    </message>
    <message>
        <source>None Selected</source>
        <translation>Nichts ausgewählt</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Einige Objekte sind gesperrt.</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Alle &amp;entsperren</translation>
    </message>
    <message>
        <source>Y: %1%2</source>
        <translation>Y: %1 %2</translation>
    </message>
    <message>
        <source>X: %1%2</source>
        <translation>X: %1 %2</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Linksbündig</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Rechtsbündig</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Zentriert</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Blocksatz</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Erzwungener Blocksatz</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Feldeigenschaften</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Hilfstext:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Rand</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farbe:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Breite:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Schmal</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Breit</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Stil:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Durchgehend</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Gestrichelt</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Hervorgehoben</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Vertieft</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andere</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Nur Lesen</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Erforderlich</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Wert nicht exportieren</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Darstellung:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Sichtbar</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Versteckt</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Nicht Drucken</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Nicht Sichtbar</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Aussehen</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Text für Button Down</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Text für Roll Over</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Icons</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Icons benutzen</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Gedrückt</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Roll Over</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Anordnung...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Hervorhebung</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertieren</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Umrandung</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Gedrückt</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Mehrzeilig</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Passwort</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Maximum von</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Nicht scrollen</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Nicht in Rechtschreibprüfung einbeziehen</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Art des Häkchens:</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Standardeinstellung ist angekreuzt</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Änderbar</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Ereignis:</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Bearbeiten...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Sende an URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Sende Daten als HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importiere Daten von:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Ziel</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>In Datei:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Ändern...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Seite:</translation>
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
        <source>Action</source>
        <translation>Aktion</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Feld ist formatiert als:</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Zahlenformat</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Dezimalstellen:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Währungssymbol benutzen</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Währungssymbol voranstellen</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatierung</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Prozent-Format</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Datums-Format</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Zeit-Format</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Tastendruck:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Format</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Wert wird nicht überprüft</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Wert muss größer oder gleich sein als:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>und kleiner oder gleich als:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Eigenes Überprüfungsscript:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Überprüfen</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Wert wird nicht berechnet</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Wert ist</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>die Summe</translation>
    </message>
    <message>
        <source>product</source>
        <translation>das Produkt</translation>
    </message>
    <message>
        <source>average</source>
        <translation>der Durchschnitt</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>das Minimum</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>das Maximum</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>der folgenden Felder:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Auswählen...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Eigenes Berechnungs-Script:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Berechnen</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Hier eine kommaseparierte Liste der Felder eintragen</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Sie benötigen mindestens das Icon für Normal um Icons zu benutzen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Bilder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Beispiel:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Auswahl geändert</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">JavaScript</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Schaltfläche</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textfeld</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kontrollkästchen</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinationsfeld</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listenfeld</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Haken</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Kreuz</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Raute</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Kreis</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Stern</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Quadrat</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Gehe zu</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Formular senden</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Formular zurücksetzen</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Daten importieren</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Maustaste loslassen</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Maustaste drücken</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mauszeiger berührt Feld</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mauszeiger verlässt Feld</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Feld hat Fokus</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Feld verliert Fokus</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Nichts</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Zahl</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Prozent</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Zeit</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Schriftart für PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Wird von PDF 1.3 ignoriert</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation>JavaScript</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation>Bilder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation>Keine</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation>Keine</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Eigenschaften der Anmerkung</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Verknüpfung</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Externe Verknüpfung</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Externe Web-Verknüpfung</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Ziel</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Seite:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>&amp;X-Position</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
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
        <translation>Musterseite anwenden</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Musterseite:</translation>
    </message>
    <message>
        <source>Apply To</source>
        <translation>Anwenden auf</translation>
    </message>
    <message>
        <source>Current &amp;page</source>
        <translation>Aktuelle &amp;Seite</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+P</translation>
    </message>
    <message>
        <source>&amp;Even pages</source>
        <translation>&amp;Gerade Seiten</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation>&amp;Ungerade Seiten</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Alle &amp;Seiten</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Within range</source>
        <translation>&amp;Bereich</translation>
    </message>
    <message>
        <source>Alt+W</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Wendet die ausgewählte Vorlage auf die geraden, ungeraden oder alle Seiten innerhalb des definierten Bereichs an&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>to</source>
        <translation>bis</translation>
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
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Apply the selected template to even, odd or all pages within the following range</source>
        <translation type="obsolete">Vorlage auf gerade, ungerade oder alle Seiten im definierten Bereich anwenden</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation>Musterseite auf gerade, ungerade oder alle Seiten im definierten Bereich anwenden</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation>Nichts</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation>&amp;Strichcode-Generator...</translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation>Scribus-Frontend für den &quot;Pure Postscript Barcode Writer&quot;</translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation>Fehler beim Öffnen der Datei: %1</translation>
    </message>
    <message>
        <source>Error creating preview</source>
        <translation type="obsolete">Fehler beim Erstellen der Vorschau</translation>
    </message>
    <message>
        <source>12 or 13 digits</source>
        <translation>12 oder 13 Ziffern</translation>
    </message>
    <message>
        <source>8 digits</source>
        <translation>8 Ziffern</translation>
    </message>
    <message>
        <source>11 or 12 digits</source>
        <translation>11 oder 12 Ziffern</translation>
    </message>
    <message>
        <source>7 or 8 digits</source>
        <translation>7 oder 8 Ziffern</translation>
    </message>
    <message>
        <source>5 digits</source>
        <translation>5 Ziffern</translation>
    </message>
    <message>
        <source>2 digits</source>
        <translation>2 Ziffern</translation>
    </message>
    <message>
        <source>9 or 10 digits separated appropriately with dashes</source>
        <translation>9 oder 10 Ziffern, mit Bindestrichen korrekt getrennt</translation>
    </message>
    <message>
        <source>Variable number of characters, digits and any of the symbols -. *$/+%.</source>
        <translation>Kombination aus Buchstaben, ZIffern und den Symbolen -.*$/+%.</translation>
    </message>
    <message>
        <source>Variable number of ASCII characters and special function symbols, starting with the appropriate start character for the initial character set. UCC/EAN-128s must have a mandatory FNC 1 symbol immediately following the start character.</source>
        <translation>Kombination aus ASCII-Zeichen und Nichtdatenzeichen, die mit dem richtigen Startzeichen des Zeichensatzes beginnt. UCC/EAN-128-Codes benötigen in jedem Fall ein FNC1-Symbol, das dem Startzeichen folgt, damit ein Scanner zwischen UCC/EAN-128 und anderen Strichcodes unterscheiden kann.</translation>
    </message>
    <message>
        <source>Variable number of digits and any of the symbols -$:/.+ABCD.</source>
        <translation>Kombination aus Ziffern und den Zeichen -$:/.+ABCD.</translation>
    </message>
    <message>
        <source>Variable number of digits. An ITF-14 is 14 characters and does not have a check digit</source>
        <translation>Kombination aus Ziffern. Ein ITF-14-Code besteht aus 14 Zeichen und hat keine Prüfziffer</translation>
    </message>
    <message>
        <source>Variable number of digits</source>
        <translation>Kombinationen aus Ziffern</translation>
    </message>
    <message>
        <source>Variable number of digits and capital letters</source>
        <translation>Kombinationen aus Ziffern und Großbuchstaben</translation>
    </message>
    <message>
        <source>Variable number of hexadecimal characters</source>
        <translation>Kombinationen aus Hexadezimalzeichen</translation>
    </message>
    <message>
        <source>Barcode incomplete</source>
        <translation>Strichcode ist unvollständig</translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation>Strichcode-Generator</translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation>Strichcode</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Typ:</translation>
    </message>
    <message>
        <source>C&amp;ode:</source>
        <translation type="obsolete">Co&amp;de:</translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation>Wählen Sie bitte einen der verfügbaren Typen aus</translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation>Die Zahlen, die in dem Strichcode vorkommen. Siehe Hilfe unten</translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation>Einstellungen zurücksetzen</translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation>&amp;Text in Strichcode einfügen</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+T</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation>Zahlen im Strichcode darstellen, wenn aktiviert</translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation>&amp;Sicherheitsabstand</translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation>Zeichne Pfeile, um einen Sicherheitsabstand zum Code herzustellen</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation>&amp;Hintergrund</translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation>Farbe hinter dem Strichcode</translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation>&amp;Linien</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation>Farbe der Strichcodelinien</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Text</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation></translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation>Farbe des Textes und der Zahlen</translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation>Hier sehen Sie Tipps und Tricks</translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation>Vorschau des Ergebnisses in 72dpi.</translation>
    </message>
    <message>
        <source>Make check digit visible in text</source>
        <translation>Prüfsumme sichtbar machen</translation>
    </message>
    <message>
        <source>Co&amp;de:</source>
        <translation>&amp;Code:</translation>
    </message>
    <message>
        <source>I&amp;nclude checksum</source>
        <translation>Prüfsumme &amp;integrieren</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Generate and include a checksum in barcode</source>
        <translation>Prüfsumme erzeugen und mit in den Barcode einfügen</translation>
    </message>
    <message>
        <source>Incl&amp;ude checksum digit</source>
        <translation>Prüfsummen&amp;zeichen integrieren</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+Z</translation>
    </message>
    <message>
        <source>Include the checksum digit in the barcode text</source>
        <translation>Prüfnummernziffer in den Barcode integrieren</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Bibliothek</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Bibliotheken (*.scs);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Neuer Eintrag</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Umbenennen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Laden...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Speichern</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="obsolete">&amp;Klein</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="obsolete">&amp;Mittel</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="obsolete">&amp;Groß</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>Vor&amp;schau</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation>Die Bezeichnung &quot;%1&quot; ist nicht eindeutig.
Bitte wählen Sie eine andere.</translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation>Wählen Sie ein Verzeichnis für die Bibliothek</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Lesezeichen</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Lesezeichen verschieben</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Lesezeichen einfügen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Lesezeichen</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Anordnung:</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Skaliere:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Immer</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>bei zu kleinem Icon</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>bei zu großem Icon</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nie</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Skalierungsart:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proportional</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Unproportional</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Icon</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Zurücksetzen</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Nur Text</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Nur Icon</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Text unter dem Icon</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Text über dem Icon</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Text rechts vom Icon</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Text links vom Icon</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Text überlagert Icon</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>System Profiles</source>
        <translation>Systemprofile</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Render-Prioritäten</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ farbmetrisch</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Farbsättigung</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut farbmetrisch</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Farbprofil für Bilder</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Farbprofil für Füllfarben</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Das Farbprofil für Ihren Monitor, das Sie entweder selbst erstellt oder vom 
Hersteller erhalten haben. Dieses Farbprofil sollte die Eigenschaften Ihres 
Monitors berücksichtigen und kein generisches Profil (z. B. sRGB) sein.</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Das Farbprofil für Ihren Drucker, das Sie vom Hersteller erhalten haben. 
Dieses Farbprofil sollte die Eigenschaften Ihres Druckers berücksichtigen 
und kein generisches Profil (z. B. sRGB) sein.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Tiefenkompensierung ist eine Methode zur Verbesserung des Kontrasts.
Diese Option sollte aktiviert sein, wenn Sie Fotos im Dokument haben.</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard-Rendermethode für Ihren Monitor. 
Falls keine besonderen Gründe vorliegen, sollte 
entweder relativ farbmetrisch oder Wahrnehmung 
gewählt werden.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard-Rendermethode für den Drucker. 
Falls keine besonderen Gründe vorliegen,
sollte entweder relativ farbmetrisch oder 
Wahrnehmung gewählt werden.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Aktiviert die Druckvorschau unter Verwendung des
gewählten Druckerprofils.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Aktiviert die Gamut-Warnung, d. h. die Anzeige von Farben, welche 
auf Grund der Eigenschaften des gewählten Ausgabegerätes für den 
Druck nicht korrekt wiedergegeben werden können. Bitte beachten Sie, 
dass dies sehr genaue Geräteprofile erfordert.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Farbmanagement &amp;aktivieren</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Bilder:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Füllfarben:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>Moni&amp;tor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Drucker:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>Moni&amp;tor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;Drucker:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Druckerfarben auf dem Bildschirm &amp;simulieren</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Farben außerhalb des Farbbereichs &amp;markieren</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>&amp;Tiefenkompensierung benutzen</translation>
    </message>
    <message>
        <source>&amp;RGB Pictures:</source>
        <translation>&amp;RGB-Bilder:</translation>
    </message>
    <message>
        <source>&amp;CMYK Pictures:</source>
        <translation>&amp;CMYK-Bilder:</translation>
    </message>
    <message>
        <source>Default color profile for imported CMYK images</source>
        <translation>Standard-Farbprofil für importierte CMYK-Bilder</translation>
    </message>
    <message>
        <source>Default color profile for imported RGB images</source>
        <translation>Standard-Farbprofil für importierte RGB-Bilder</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
    <message>
        <source>Edit Color</source>
        <translation>Farbe bearbeiten</translation>
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
        <translation>Webfarben</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Neu</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Alt</translation>
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
        <translation>Dynamische Farbregler</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Statische Farbregler</translation>
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
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">Der Name der Farbe ist nicht eindeutig</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV-Farbwähler</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Farb&amp;modell</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">Sie können keine Farbe mit dem Namen &quot;%1&quot; erstellen.
Dieser Name ist für die transparente Farbe reserviert</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation>Schmuckfarbe</translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation>Registrierungsfarbe</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation>Sie können keine Farbe mit der Bezeichnung &quot;%1&quot; erstellen.
%1 ist reserviert für Transparenz</translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation>Der Name der Farbe ist nicht eindeutig</translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
        <translation type="obsolete">Wenn Sie die rechte Maustaste drücken und gedrückt halten,
sehen Sie ein Vorschaubild.

Die Taste Einfügen fügt ein Zeichen in die Markierung unten ein
und die Taste Löschen entfernt das letzte Zeichen wieder</translation>
    </message>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation>Wenn Sie die rechte Maustaste drücken und gedrückt halten, sehen Sie ein Vorschaubild. Die Taste Einfügen fügt ein Zeichen in die Markierung unten ein, und die Taste Löschen entfernt das letzte Zeichen wieder</translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation>Zeichen auswählen:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Schrift:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation>Zeichensatz:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Ein&amp;fügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Zeichen an Cursorposition einfügen</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Markierung(en) löschen.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Diesen Dialog schließen und den Text weiter bearbeiten.</translation>
    </message>
    <message>
        <source>Full Character Set</source>
        <translation>Kompletter Zeichensatz</translation>
    </message>
    <message>
        <source>Basic Latin</source>
        <translation>Basis Latein</translation>
    </message>
    <message>
        <source>Latin-1 Supplement</source>
        <translation>Latein-1</translation>
    </message>
    <message>
        <source>Latin Extended-A</source>
        <translation>Latein erweitert A</translation>
    </message>
    <message>
        <source>Latin Extended-B</source>
        <translation>Latein erweitert B</translation>
    </message>
    <message>
        <source>General Punctuation</source>
        <translation>Allgemeine Interpunktion</translation>
    </message>
    <message>
        <source>Super- and Subscripts</source>
        <translation>Hoch- und Tiefstellungen</translation>
    </message>
    <message>
        <source>Currency Symbols</source>
        <translation>Währungssymbole</translation>
    </message>
    <message>
        <source>Letterlike Symbols</source>
        <translation>Buchstabensymbole</translation>
    </message>
    <message>
        <source>Number Forms</source>
        <translation>Zahlensysmbole</translation>
    </message>
    <message>
        <source>Arrows</source>
        <translation>Pfeilsymbole</translation>
    </message>
    <message>
        <source>Mathematical Operators</source>
        <translation>Mathematische Operatoren</translation>
    </message>
    <message>
        <source>Box Drawing</source>
        <translation>Rahmensymbole</translation>
    </message>
    <message>
        <source>Block Elements</source>
        <translation>Blocksymbole</translation>
    </message>
    <message>
        <source>Geometric Shapes</source>
        <translation>Geometrische Formen</translation>
    </message>
    <message>
        <source>Miscellaneous Symbols</source>
        <translation>Sonstige Symbole</translation>
    </message>
    <message>
        <source>Dingbats</source>
        <translation>Dingbats</translation>
    </message>
    <message>
        <source>Small Form Variants</source>
        <translation>Kleine Symbole</translation>
    </message>
    <message>
        <source>Ligatures</source>
        <translation>Ligaturen</translation>
    </message>
    <message>
        <source>Specials</source>
        <translation>Spezielle Zeichen</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griechisch</translation>
    </message>
    <message>
        <source>Greek Extended</source>
        <translation>Griechisch erweitert</translation>
    </message>
    <message>
        <source>Cyrillic</source>
        <translation>Kyrillisch</translation>
    </message>
    <message>
        <source>Cyrillic Supplement</source>
        <translation>Kyrillisch Zusätze</translation>
    </message>
    <message>
        <source>Arabic</source>
        <translation>Arabisch</translation>
    </message>
    <message>
        <source>Arabic Extended A</source>
        <translation>Arabisch erweitert A</translation>
    </message>
    <message>
        <source>Arabic Extended B</source>
        <translation>Arabisch erweitert B</translation>
    </message>
    <message>
        <source>Hebrew</source>
        <translation>Hebräisch</translation>
    </message>
    <message>
        <source>&amp;Insert Code:</source>
        <translation>Code ein&amp;fügen:</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation>Diesen Dialog schließen und zum Text zurückkehren</translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
        <translation>Geben Sie hier den vierstelligen Unicodewert direkt ein</translation>
    </message>
</context>
<context>
    <name>CheckDocument</name>
    <message>
        <source>Glyphs missing</source>
        <translation>Fehlende Zeichen</translation>
    </message>
    <message>
        <source>Text overflow</source>
        <translation>Überfließender Text</translation>
    </message>
    <message>
        <source>Object is not on a Page</source>
        <translation>Objekt außerhalb einer Seite</translation>
    </message>
    <message>
        <source>Missing Image</source>
        <translation>Fehlendes Bild</translation>
    </message>
    <message>
        <source>Image has a DPI-Value less than %1 DPI</source>
        <translation>Der dpi-Wert des Bildes ist kleiner als %1 dpi</translation>
    </message>
    <message>
        <source>Object has transparency</source>
        <translation>Transparenz wird verwendet</translation>
    </message>
    <message>
        <source>Object is a PDF Annotation or Field</source>
        <translation>Objekt ist eine PDF-Anmerkung oder ein PDF-Feld</translation>
    </message>
    <message>
        <source>Object is a placed PDF</source>
        <translation>Objekt ist eine importierte PDF-Datei</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation>Keine Probleme gefunden</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Seite </translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Freie Objekte</translation>
    </message>
    <message>
        <source>Problems found</source>
        <translation>Es sind Probleme aufgetaucht</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Druckvorstufenüberprüfung</translation>
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
        <source>Current Profile:</source>
        <translation>Aktuelles Profil:</translation>
    </message>
    <message>
        <source>&amp;Ignore Errors</source>
        <translation>Fehler ig&amp;norieren</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Stil wählen</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Verfügbare Stile</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation>Stelle die notwendigen Dateien zusammen...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation>Es können nicht alle Dateien für die Datei %1 
zusammengestellt werden</translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation>Fehler beim Zusammenstellen der Datei
%1</translation>
    </message>
</context>
<context>
    <name>ColorListBox</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Unbenutzte &amp;entfernen</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Farbpaletten</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Aktive Palette:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Farbset &amp;speichern</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Farbpalette auswählen</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Farbpalette speichern</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Unbenutzte Farben löschen</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation>Importiert Farben von einem existierenden Dokument</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Neue Farbe anlegen</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Farbe bearbeiten</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Kopie der ausgewählen Farbe anlegen</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Ausgewählte Farbe löschen</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Aktuelle Farbpalette zur Voreinstellung machen</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Namen wählen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Neue Farbe</translation>
    </message>
</context>
<context>
    <name>ColorWheel</name>
    <message>
        <source>Monochromatic</source>
        <translation>Monochromatische Farben</translation>
    </message>
    <message>
        <source>Analogous</source>
        <translation>Analoge Farben</translation>
    </message>
    <message>
        <source>Complementary</source>
        <translation>Komplementärfarben</translation>
    </message>
    <message>
        <source>Split Complementary</source>
        <translation>Gespaltene Komplementärfarben</translation>
    </message>
    <message>
        <source>Triadic</source>
        <translation>Triadisch</translation>
    </message>
    <message>
        <source>Tetradic (Double Complementary)</source>
        <translation>Triadisch (Doppelte Komplementärfarben)</translation>
    </message>
    <message>
        <source>Base Color</source>
        <translation>Grundfarbe</translation>
    </message>
    <message>
        <source>Monochromatic Light</source>
        <translation>Monochromatisch Hell</translation>
    </message>
    <message>
        <source>Monochromatic Dark</source>
        <translation>Monochromatisch Dunkel</translation>
    </message>
    <message>
        <source>1st. Analogous</source>
        <translation>1. Analoge</translation>
    </message>
    <message>
        <source>2nd. Analogous</source>
        <translation>2. Analoge</translation>
    </message>
    <message>
        <source>1st. Split</source>
        <translation>1. Teilung</translation>
    </message>
    <message>
        <source>2nd. Split</source>
        <translation>2. Teilung</translation>
    </message>
    <message>
        <source>3rd. Split</source>
        <translation>3. Teilung</translation>
    </message>
    <message>
        <source>4th. Split</source>
        <translation>4. Teilung</translation>
    </message>
    <message>
        <source>1st. Triadic</source>
        <translation>1. Triadisch</translation>
    </message>
    <message>
        <source>2nd. Triadic</source>
        <translation>2. Triadisch</translation>
    </message>
    <message>
        <source>1st. Tetradic (base opposite)</source>
        <translation>1. Tetradisch</translation>
    </message>
    <message>
        <source>2nd. Tetradic (angle)</source>
        <translation>2. Tetradisch (Winkel)</translation>
    </message>
    <message>
        <source>3rd. Tetradic (angle opposite)</source>
        <translation>3. Tetradisch (gegenüberliegender Winkel)</translation>
    </message>
</context>
<context>
    <name>ColorWheelDialog</name>
    <message>
        <source>Color Wheel</source>
        <translation>Farbkreis</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
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
        <translation>Gewünschte Methode:</translation>
    </message>
    <message>
        <source>Angle (0 - 90 degrees):</source>
        <translation>Radius (0-90 Grad):</translation>
    </message>
    <message>
        <source>&amp;Merge Colors</source>
        <translation>Farben &amp;hinzufügen</translation>
    </message>
    <message>
        <source>&amp;Replace Colors</source>
        <translation>Farben er&amp;setzen</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Merge created colors into the document colors</source>
        <translation>Farben zu den Dokumentfarben hinzufügen</translation>
    </message>
    <message>
        <source>Replace created colors in the document colors</source>
        <translation>Vorhandene Dokumentfarben mit den neuen Farben ersetzen</translation>
    </message>
    <message>
        <source>Leave colors untouched</source>
        <translation>Farben unverändert lassen</translation>
    </message>
    <message>
        <source>Difference between selected value and counted ones. See documentation for more info</source>
        <translation type="obsolete">Differenz zwischen der gewählten und berechneten Farbe. Mehr Infos gibts in der Hilfe</translation>
    </message>
    <message>
        <source>Click the wheel to get base color</source>
        <translation type="obsolete">Klicken Sie auf das Farbrad, um die Ausgangsfarbe zu wählen</translation>
    </message>
    <message>
        <source>Here you have the sample color schema</source>
        <translation type="obsolete">Ein Beispielschema, basierend auf den aktuellen Einstellungen</translation>
    </message>
    <message>
        <source>Select one of the method to create color schema. See documentation for more info</source>
        <translation type="obsolete">Wählen Sie eine Methode, nach der Sie das Schema erstellen wollen. Weitere Informationen im Handbuch</translation>
    </message>
    <message>
        <source>Here you have the color of your chosen color schema</source>
        <translation type="obsolete">Hier sehen Sie alle Farben für Ihr Farbschema</translation>
    </message>
    <message>
        <source>Merging colors</source>
        <translation>Farben werden zusammengeführt</translation>
    </message>
    <message>
        <source>Error: </source>
        <translation>Fehler:</translation>
    </message>
    <message>
        <source>Now opening the color manager.</source>
        <translation>Farben-Dialog wird jetzt geöffnet.</translation>
    </message>
    <message>
        <source>Color Merging</source>
        <translation>Farben hinzufügen</translation>
    </message>
    <message>
        <source>Normal Vision</source>
        <translation>Normales Sehvermögen</translation>
    </message>
    <message>
        <source>Protanopy</source>
        <translation type="obsolete">Rotblindheit</translation>
    </message>
    <message>
        <source>Deuteranopy</source>
        <translation type="obsolete">Rotgrünblindheit</translation>
    </message>
    <message>
        <source>Full Color Blindness</source>
        <translation>Komplett Farbenblind</translation>
    </message>
    <message>
        <source>Vision Defect:</source>
        <translation>Sehschwäche:</translation>
    </message>
    <message>
        <source>You can simulate common vision defects here. Just select type of the defect</source>
        <translation type="obsolete">Sie können hier bekannte Sehschwächen simulieren. Wählen Sie einfach die Krankheit aus</translation>
    </message>
    <message>
        <source>Protanopia (red)</source>
        <translation type="obsolete">Protanopie (Rotblindheit)</translation>
    </message>
    <message>
        <source>Deuteranopia (green)</source>
        <translation type="obsolete">Deuteranopie (Grünblindheit)</translation>
    </message>
    <message>
        <source>Tritanopia (blue)</source>
        <translation type="obsolete">Tritanopie (Blaublindheit)</translation>
    </message>
    <message>
        <source>Cr&amp;eate color...</source>
        <translation>Neue &amp;Farbe erstellen...</translation>
    </message>
    <message>
        <source>&amp;Import existing color...</source>
        <translation>Bestehende Farben &amp;importieren...</translation>
    </message>
    <message>
        <source>&amp;Merge colors</source>
        <translation>F&amp;arben zusammenfügen</translation>
    </message>
    <message>
        <source>&amp;Replace colors</source>
        <translation>Fa&amp;rben ersetzen</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>Ver&amp;lassen</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>Saturation:</source>
        <translation type="obsolete">Sättigung:</translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="obsolete">Wert:</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation>Difference between the selected value and the counted ones. Refer to documentation for more information.</translation>
    </message>
    <message>
        <source>Saturation component in HSV mode</source>
        <translation type="obsolete">Sättigungskomponente im HSV-Modus</translation>
    </message>
    <message>
        <source>Value component in HSV mode</source>
        <translation type="obsolete">Wertkomponente im HSV-Modus</translation>
    </message>
    <message>
        <source>Click the wheel to get the base color. It is hue in HSV mode.</source>
        <translation>Klicken Sie in das Farbrad, um die Ausgangsfarbe zu erhalten. 
Diese ist Farbe (Hue) im HSV-Modus.</translation>
    </message>
    <message>
        <source>Sample color scheme</source>
        <translation>Beispiel-Farbmuster</translation>
    </message>
    <message>
        <source>Select one of the methods to create a color scheme. Refer to documentation for more information.</source>
        <translation>Wählen Sie eine der Methoden zur Erstellung eines Farbschemas aus.
Die Dokumentation enthält nähere Informationen hierzu.</translation>
    </message>
    <message>
        <source>Colors of your chosen color scheme</source>
        <translation>Die Farben des gewählten Farbschemas</translation>
    </message>
    <message>
        <source>Simulate common vision defects here. Select type of the defect.</source>
        <translation>Hier können Sie die Art der Sehbehinderung auswählen, welche Sie simulieren wollen.</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Neue Farbe</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation>Scribus konnte die gewünschte Farbe nicht finden. Sie haben wahrscheinlich Schwarz, Grau oder Weiß ausgewählt.
Es ist nicht möglich, diese Farben zu verarbeiten.</translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation>&amp;Farbkomponenten...</translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation>Rotgrünblindheit</translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation>Grünblindheit</translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation>Blaublindheit</translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation>Die Farbe %1 ist schon vorhanden!</translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
        <translation>Farbe %1 wurde hinzugefügt.</translation>
    </message>
</context>
<context>
    <name>ColorWheelPlugin</name>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation>Farb&amp;rad...</translation>
    </message>
    <message>
        <source>Color setting helper</source>
        <translation>Hilfsmittel zur Erstellung von Farbschemata</translation>
    </message>
    <message>
        <source>Color selector with color theory included.</source>
        <translation>Farbwähler basierend auf Farbenlehren.</translation>
    </message>
</context>
<context>
    <name>CommonStrings</name>
    <message>
        <source>&amp;Apply</source>
        <translation>An&amp;wenden</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>A&amp;bbrechen</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Einzelne Seite</translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation>Doppelseite</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation>3fach gefaltet</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation>4fach gefaltet</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation>Linke Seite</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation>Mitte</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation>Mitte links</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Mitte rechts</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Rechte Seite</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Waagrechter Verlauf</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Senkrechter Verlauf</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Diagonaler Verlauf</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Umgekehrt diagonaler Verlauf</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Kreisförmiger Verlauf</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Deckkraft:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Tonwert:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Linienfarbe bearbeiten</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Füllfarbe bearbeiten</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Tonwert der Farbe</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Füllmethode</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Transparenz für Farbe auswählen</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Farbe des selektierten Objekts</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Benutzerdefinierter linearer Verlauf</translation>
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
        <source>Free radial Gradient</source>
        <translation>Benutzerdefinierter radialer Verlauf</translation>
    </message>
    <message>
        <source>Move Vector</source>
        <translation>Vektor verschieben</translation>
    </message>
    <message>
        <source>Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed</source>
        <translation>Verschieben Sie den Anfang des Verlaufsvektors, indem Sie die linke Maustaste gedrückt halten, und den Endwert des Vektors mit der rechten Maustaste</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Optionen für CSV-Import</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Feldtrenner:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Werttrenner:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keiner</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Erste Zeile ist Kopfzeile</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation>Keine</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Druckereinstellungen (CUPS)</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Einstellung</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Wert</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Welche Seiten</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Alle Seiten</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Nur gerade Seiten</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Nur ungerade Seiten</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Spiegeln</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Ausrichtung</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Seiten zusammenfassen</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Seite pro Blatt</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Seiten pro Blatt</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Dieser Dialog zeigt verschiedene CUPS-Optionen zum Drucken an.
Die verfügbaren Einstellungen sind von Ihrem Drucker anhängig.
Hlife -&gt; Über zeigt Ihnen an, ob Ihre Scribus-Version CUPS unterstützt.
Dabei stehen die Zeichen für C=CUPS, C=LittleCMS und T=TIFF-Unterstützung.
Fehlende Bibliotheken werden durch ein Sternchen angezeigt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation>Dieser Dialog enthält verschiedene Optionen für das Drucken mit CUPS. Die exakten Optionen hängen von Ihrem Drucker ab. Sie können überprüfen, ob CUPS aktiviert ist, wenn Sie Hilfe &gt; Über aufrufen. Suchen Sie nach den Zeichen C-C-T-F. Diese bedeuten C=CUPS C=littlecms T=TIFF. Fehlende Bibliotheken werden durch * angezeigt</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Kodierung:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Führt zum Dokumentverzeichnis.
Diese Option können Sie in den Einstellungen ändern.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>Datei &amp;komprimieren</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>Schriftarten ein&amp;betten</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation>Farbkomponenten festlegen</translation>
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
        <translation>&amp;RGB auswählen</translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation>C&amp;MYK auswählen</translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation>&amp;HSV auswählen</translation>
    </message>
</context>
<context>
    <name>DeferredTask</name>
    <message>
        <source>Cancelled by user</source>
        <translation>Vom Anwender beendet</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Farbe löschen</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Delete Color:</source>
        <translation>Farbe löschen:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Ersetzen mit:</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Seiten löschen</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation>Löschen von:</translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation>Stil löschen</translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation>Lösche Stil:</translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation>Ersetzen mit:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation>Import fehlgeschlagen</translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation>Fehler beim Import des Word-Dokuments
%1</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Dokumentinformationen</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Titel:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Stichwörter:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Beschrei&amp;bung:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Herausgeber:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Mitarbeiter:</translation>
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
        <translation>&amp;Format:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Identifikation:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Quelle:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Beziehung:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Gültigkeitsbereich:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>&amp;Rechte:</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Weitere &amp;Informationen</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Eine Person oder Organisation, die für die Veröffentlichung des Dokuments verantwortlich ist</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Eine Person oder Organisation, die an der Erstellung des Dokuments beteiligt war</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Ein Datum, das mit der Entstehung des Dokuments verbunden ist,
nach ISO 8601 im Format YYYY-MM-DD </translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Die Gattung oder der Typ des Dokuments, z. B. Kategorie, Funktion, Genre etc</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Eine eindeutige Referenz zu dem Dokument in einem gegebenen Kontext wie ISBN oder URI</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Eine Referenz zu einem ähnlichen Dokument. Hier kann eine formale 
Identifikation wie ISBN oder ein URI verwendet werden</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Der Geltungsbereich des Dokuments, wenn möglich mit Ort, Zeit und Gerichtsstand</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informationen über rechtliche Aspekte des Dokumenteninhalts, 
etwa Urheberrechte, Patente oder Warenzeichen</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Eine Referenz zu einem Dokument, von dem sich das aktuelle Dokument ableitet, z.B. ISBN oder URI</translation>
    </message>
    <message>
        <source>Documen&amp;t</source>
        <translation>Dokume&amp;nt</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Die Person oder Organisation, die hauptsächlich für den Inhalt des Dokuments verantwortlich ist. Dieses Feld kann sowohl in das Scribus-Dokument als auch in die Metadaten einer PDF-Datei eingebettet werden</translation>
    </message>
    <message>
        <source>A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Der Titel oder die Bezeichnung des Dokuments. 
Dieses Feld kann sowohl in das Scribus-Dokument als auch 
in die Meta-Daten einer PDF-Datei eingebettet werden</translation>
    </message>
    <message>
        <source>An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Ein Abriss des Inhalts.
Hier kann eine kurze Beschreibung oder Zusammenfassung stehen. 
Sie wird in die PDF-Datei exportiert</translation>
    </message>
    <message>
        <source>The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Kurze Beschreibung des Inhalts.
Der Inhalt dieses Feldes wird in der PDF-Datei gespeichert und hilft Ihnen, 
PDF-Dateien zu indexieren und wiederzufinden</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Die physische oder digitale Veröffentlichungsform des Dokuments. 
Abmessung und Medientyp können hier notiert werden.
Die Verwendung der RFCs 2045 und 2046 für MIME-Typen kann hier nützlich sein</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Die Sprache, in welcher der Inhalt des Dokuments geschrieben ist, 
normalerweise ein ISO-639-Sprachcode, optional ergänzt durch 
einen Bindestrich und den ISO-3166 Ländercode, wie z. B. en-GB oder fr-CH</translation>
    </message>
</context>
<context>
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation>Fügt einen Abschnitt für die Seitenzahl in das Dokument ein. Der neue Abschnitt wird unter dem aktuellen hinzugefügt.</translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation>Markierten Abschnitt löschen.</translation>
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
        <source>Name: Optional name for section eg. Index&lt;br/&gt;Shown: Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;From: The page index for this section to start at.&lt;br/&gt;To: The page index for this section to stop at.&lt;br/&gt;Style: Select the page number style to be used.&lt;br/&gt;Start: The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b.</source>
        <translation type="obsolete">Name: Optionaler Name für den Abschnitt, z. B. Index&lt;br/&gt;angezeigt: Seitenzahlen anzeigen für diesen Abschnitt, wenn diese Einstellung für einen oder mehrere Textrahmen gewählt ist.&lt;br/&gt;Von: Die Seitenzahl für diesen Abschnitt endet auf dieser Seite.&lt;br/&gt;Bis: Die Seitenzahl für diesen Abschnitt endet auf dieser Seite.&lt;br/&gt;Stil: Stil für die Seitenzahlen wählen.&lt;br/&gt;Start: Die Seitenzahlen beginnen bei der Zahl, entsprechend den möglichen Werten für den Stil, bei Style=a,b,c beginnt die Nummerierung bei b.</translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation>Seitenzahl außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation>Der Wert, den Sie eingegeben haben, liegt außerhalb des gültigen Bereichs (%1-%2).</translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation>Dokumentabschnitte</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="obsolete">Abschnitte</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Active</source>
        <translation type="obsolete">aktiv</translation>
    </message>
    <message>
        <source>From</source>
        <translation>von</translation>
    </message>
    <message>
        <source>To</source>
        <translation>bis</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <source>Start</source>
        <translation>Beginn</translation>
    </message>
    <message>
        <source>End</source>
        <translation type="obsolete">Ende</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation>Sichtbar</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Verweist auf</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Ist Elternobjekt von</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Ist Kindobjekt von</translation>
    </message>
    <message>
        <source>Text Frames</source>
        <translation>Textrahmen</translation>
    </message>
    <message>
        <source>Image Frames</source>
        <translation>Bildrahmen</translation>
    </message>
    <message>
        <source>Boolean</source>
        <translation>Boolsch</translation>
    </message>
    <message>
        <source>Integer</source>
        <translation>Integer</translation>
    </message>
    <message>
        <source>String</source>
        <translation>String</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation type="obsolete">Eigenschaften des Dokumentobjekts</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Name</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Wert</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="obsolete">Beziehung</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="obsolete">Beziehung zu</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation type="obsolete">Automatisch hinzufügen zu</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+H</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopieren</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Inhalt löschen</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Keine</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation>Keine</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation>Reale Zahl</translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation>Eigenschaften des Dokumentobjekts</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Wert</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Beziehung</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Beziehung zu</translation>
    </message>
    <message>
        <source>Auto Add To</source>
        <translation>Automatisch hinzufügen zu</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
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
        <translation>Drucker einrichten</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
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
        <translation>Gelb</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Schwarz</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Geben sie durch Kommata getrennt ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Drucker</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Optionen...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Datei:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Al&amp;ternativer Druckbefehl</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Befehl:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Bereich</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>&amp;Alles drucken</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Aktue&amp;lle Seite drucken</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Be&amp;reich drucken</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Anzahl der &amp;Kopien:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">&amp;Normal drucken</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">&amp;Farbauszüge drucken</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">In Fa&amp;rbe drucken, falls verfügbar</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">In &amp;Graustufen drucken</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">Er&amp;weiterte Optionen...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Drucken</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Einen anderen Druckmanager benutzen, zum Beispiel kprinter oder gtklp,
um zusätzliche Optionen einstellen zu können</translation>
    </message>
    <message>
        <source>Postscript Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Postscript-Dateien (*.ps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Print Normal</source>
        <translation>Normal drucken</translation>
    </message>
    <message>
        <source>Print Separations</source>
        <translation>Farbseparationen drucken</translation>
    </message>
    <message>
        <source>Print in Color if Available</source>
        <translation>In Farbe drucken, falls möglich</translation>
    </message>
    <message>
        <source>Print in Grayscale</source>
        <translation>In Graustufen drucken</translation>
    </message>
    <message>
        <source>PostScript Level 1</source>
        <translation>Postscript Level 1</translation>
    </message>
    <message>
        <source>PostScript Level 2</source>
        <translation>Postscript Level 2</translation>
    </message>
    <message>
        <source>PostScript Level 3</source>
        <translation>Postscript Level 3</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Mirror Page(s) Horizontal</source>
        <translation>Seite(n) horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) Vertical</source>
        <translation>Seite(n) vertikal spiegeln</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Mediengröße festlegen</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <source>Apply Under Color Removal</source>
        <translation>Unterfarbenreduktion anwenden</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Schmuckfarben in Prozessfarben umwandeln</translation>
    </message>
    <message>
        <source>Apply ICC Profiles</source>
        <translation>ICC-Profile anwenden</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Weitere Optionen</translation>
    </message>
    <message>
        <source>Preview...</source>
        <translation>Vorschau...</translation>
    </message>
    <message>
        <source>Sets the PostScript Level.
 Setting to Level 1 or 2 can create huge files</source>
        <translation>Legt die Postscript-Version fest. 
Postscript Level 1 und 2 haben häufig sehr große Dateien zur Folge</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Mit dieser Option werden Grautöne, die aus Cyan, Magenta und Gelb gemischt würden, 
durch reine Abstufungen von Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr ähnlich sind. Diese Option kann zu besseren Bildern führen.
Sie müssen jedoch von Fall zu Fall entscheiden, ob Sie bessere Ergebnisse
erzielen. Zudem reduziert UFR die Gefahr der Übersättigung.</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation>Postscript-Dateien (*.ps);;All Files (*)</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation>Einen alternativen Druckmanager, etwa kprinter oder gtklp, für weitere Druckoptionen verwenden</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Verwendet echtes Schwarz anstelle von Farbmischungen aus Magenta, Gelb und Cyan. UFR hat den größten Effekt auf Bilder, die neutrale oder dunkle Farbtöne haben, die Grau ähneln. Diese Option kann die Druckqualität verbessern. Testen Sie bitte von Fall zu Fall, wie Sie bessere Ergebnisse erhalten. UFR verringert außerdem die Gefahr einer Übersättigung der CMYK-Farben.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Konvertiert Schmuckfarben in normale Farben. Sie sollten diese Option nur deaktivieren, wenn Sie Schmuckfarben wirklich benötigen.</translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation>Wenn das Farbmanagement aktiviert ist, werden die ICC-Profile mit in den Druckstream eingebettet</translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation>Legen Sie die Druckgröße der Postscript-Datei explizit fest. Nur aktivieren, wenn ihre Druckerei das verlangt.</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Auf Seitenränder beschneiden</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Objekte außerhalb der Seitenränder auf den gedruckten oder exportierten Seiten nicht zeigen</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation>Abfragen der Druckereinstellungen fehlgeschlagen</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page</source>
        <translation>Objekte außerhalb des Seitenrandes nicht drucken</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Fehler beim Importieren
der Datei
%1!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fataler Fehler</translation>
    </message>
    <message>
        <source>Converting Image:
%1
failed!</source>
        <translation>Fehler beim Konvertieren von:
%1!</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Stilvorlage bearbeiten</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Zeilenabstand</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">Der Name der Stilvorlage ist nicht eindeutig</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Name des Absatz-Stils</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Schriftart des gewählten Textes oder des Objekts</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textfarbe</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Textumrissfarbe</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Legt die gesamte Höhe der Initiale in Zeilennummern fest</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Abstand über dem Absatz</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Abstand unter dem Absatz</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabulatoren und Einzüge</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Zeilen:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation>Abstände</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fester Zeilenabstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatischer Zeilenabstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Am Grundlinienraster ausrichten</translation>
    </message>
    <message>
        <source>Drop Caps</source>
        <translation>Initialen</translation>
    </message>
    <message>
        <source>Distance from Text:</source>
        <translation>Abstand vom Text:</translation>
    </message>
    <message>
        <source>Preview of the Paragraph Style</source>
        <translation>Vorschau des Absatzstils</translation>
    </message>
    <message>
        <source>Determines the gap between the DropCaps and the Text</source>
        <translation>Legt den Abstand zwischen der Initiale und dem Text fest</translation>
    </message>
    <message>
        <source>Toggles sample text of this paragraph style</source>
        <translation>Schaltet die Absatzvorschau an oder aus</translation>
    </message>
    <message>
        <source>Name of the style is not unique</source>
        <translation>Der Name des Stils ist nicht eindeutig</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
        <translation>Aktivieren Sie diese Option, um hellen Text besser lesen zu können</translation>
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
        <translation type="obsolete">Javascripte (*.js);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>Ö&amp;ffnen...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>Speicher&amp;n und beenden</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>Nicht speichern und &amp;beenden</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Feldnamen auslesen</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>JavaScripts (*.js);;All Files (*)</source>
        <translation>JavaScripts (*.js);;Alle Dateien (*)</translation>
    </message>
</context>
<context>
    <name>EffectsDialog</name>
    <message>
        <source>Image Effects</source>
        <translation>Bildeffekte</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Optionen:</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farbe:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Tonwert:</translation>
    </message>
    <message>
        <source>Brightness:</source>
        <translation>Helligkeit:</translation>
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
        <translation>Wert:</translation>
    </message>
    <message>
        <source>Posterize:</source>
        <translation>Posterisieren:</translation>
    </message>
    <message>
        <source>Available Effects</source>
        <translation>Verfügbare Effekte</translation>
    </message>
    <message>
        <source>Blur</source>
        <translation>Verwischen</translation>
    </message>
    <message>
        <source>Brightness</source>
        <translation>Helligkeit</translation>
    </message>
    <message>
        <source>Colorize</source>
        <translation>Farben ändern</translation>
    </message>
    <message>
        <source>Contrast</source>
        <translation>Kontrast</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Graustufen</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertieren</translation>
    </message>
    <message>
        <source>Posterize</source>
        <translation>Posterisieren</translation>
    </message>
    <message>
        <source>Sharpen</source>
        <translation>Schärfen</translation>
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
        <translation>Verwendete Effekte</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>Alle &amp;Seiten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Ausgabeverzeichnis wechseln</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Verfügbare Exportformate</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Wählen Sie ein Ausgabeverzeichnis</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Das Ausgabeverzeichnis - dort werden Ihre Bilder gespeichert.
Dateinamen der Bilder haben das Format &quot;NamedesDokuments-Seite.Dateiformat&quot;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Nur die aktuelle Seite exportieren</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>In &amp;Verzeichnis exportieren:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Dateityp:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualität:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Als Bild speichern</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Auf&amp;lösung:</translation>
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
        <translation>Bereich</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>A&amp;ktuelle Seite</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Bereich</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Einen Seitenbereich exportieren</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Geben Sie mit Hilfe von Kommata oder Bindestrichen ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Alle Seiten exportieren</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Gibt die Auflösung der Bilder an.
72 dpi sind optimal, wenn Sie die Seiten nur auf dem 
Bildschirm betrachten wollen</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Gibt die Qualität der Bilder an - von 100% = beste Qualität bis 1% = schlechteste Qualität</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Größe der Bilder. 100% verändert nichts, 200% für doppelt so große Bilder etc.</translation>
    </message>
</context>
<context>
    <name>ExtImageProps</name>
    <message>
        <source>Extended Image Properties</source>
        <translation>Erweiterte Bildeigenschaften</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Darken</source>
        <translation>Abdunkeln</translation>
    </message>
    <message>
        <source>Lighten</source>
        <translation>Aufhellen</translation>
    </message>
    <message>
        <source>Hue</source>
        <translation>Farbton</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Sättigung</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <source>Luminosity</source>
        <translation>Luminanz</translation>
    </message>
    <message>
        <source>Multiply</source>
        <translation>Multiplizieren</translation>
    </message>
    <message>
        <source>Screen</source>
        <translation>Negativ Multiplizieren</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Auflösen</translation>
    </message>
    <message>
        <source>Overlay</source>
        <translation>Ineinander kopieren</translation>
    </message>
    <message>
        <source>Hard Light</source>
        <translation>Hartes Licht</translation>
    </message>
    <message>
        <source>Soft Light</source>
        <translation>Weiches Licht</translation>
    </message>
    <message>
        <source>Difference</source>
        <translation>Differenz</translation>
    </message>
    <message>
        <source>Exclusion</source>
        <translation>Ausschluss</translation>
    </message>
    <message>
        <source>Color Dodge</source>
        <translation>Farbig abwedeln</translation>
    </message>
    <message>
        <source>Color Burn</source>
        <translation>Farbig nachbelichten</translation>
    </message>
    <message>
        <source>Exlusion</source>
        <translation>Ausschluss</translation>
    </message>
    <message>
        <source>Blend Mode:</source>
        <translation>Modus:</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Deckkraft:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Ebenen</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation>Keinen Pfad benutzen</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Pfade</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Größe:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Titel:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Kein Titel</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus-Dokument</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Auflösung:</translation>
    </message>
    <message>
        <source>DPI</source>
        <translation>dpi</translation>
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
        <translation>Graustufen</translation>
    </message>
    <message>
        <source>Colorspace:</source>
        <translation>Farbraum:</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation type="obsolete">Farben</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="obsolete">Farbpaletten</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="obsolete">Aktive Palette:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="obsolete">Namen wählen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Neue Farbe</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Kopie von %1</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="obsolete">Farbpalette auswählen</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="obsolete">Farbpalette speichern</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Unbenutzte Farben löschen</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Farben aus anderem Dokument nachladen</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Neue Farbe anlegen</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Farbe bearbeiten</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Kopie der ausgewählen Farbe anlegen</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">Ausgewählte Farbe löschen</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">Aktuelle Palette zur Voreinstellung machen</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">An&amp;fügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">Unbenutzte &amp;entfernen</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">Farbset &amp;speichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Name:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">setHGuides(list)

Legt horizontale Hilfslinien fest, list muss eine Liste der Linien sein, angegeben in der Maßeinheit
des Dokuments, siehe UNIT-Konstanten.

Beispiel:
setHGuides(getHGuides() + [200.0, 210.0] # fügt eine neue Hilfslinie hinzu
setHGuides([90,250]) # ersetzt alle vorhandenen Hilfslinien</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importieren</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="obsolete">Importiert Farben in die Liste eines existierenden Dokumentes</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation>Einige Schriftarten in diesem Dokument wurden ersetzt:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> wurde ersetzt durch: </translation>
    </message>
</context>
<context>
    <name>FileSearch</name>
    <message>
        <source>Search cancelled</source>
        <translation type="obsolete">Suche abgebrochen</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Available Fonts</source>
        <translation>Verfügbare Schriften</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Schriftersetzungen</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Zusätzliche Pfade</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Schriftname</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Ersatzschrift</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Verfügbare &amp;Schriftarten</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Schriftarten&amp;ersetzung</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Zusätzliche &amp;Pfade</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Hinzufügen...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation>Name der Schriftart</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation>Schrift benutzen</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation>Einbetten in:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation>Unterteilen</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation>Pfad zur Schriftdatei</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Pfade, die in die Suche nach Schriftarten einbezogen werden sollen, können nur in den Voreinstellungen festgelegt werden, wenn kein Dokument geöffnet ist. Schließen Sie alle Dokumente und ändern Sie die Pfad-Einstellungen über Bearbeiten-&gt;Voreinstellungen.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation>Der Pfad, in dem nach Fonts gesucht wird, kann nur in den Grundeinstellungen geändert werden, wenn gerade kein Dokument geöffnet ist. Schließen Sie alle Dokumenten und ändern Sie den Pfad in Bearbeiten &gt; Einstellungen.</translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="obsolete">Name der Schriftart</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation type="obsolete">Doc</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="obsolete">Unterteilen</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="obsolete">Zugriff</translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="obsolete">Zeichengröße:</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="obsolete">Schriftartenvorschau</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="obsolete">Suche:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="obsolete">&amp;Suchen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="obsolete">Sch&amp;ließen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation>Ausgewählte Schriftart dem Stil zuweisen</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation>Vorschau verlassen</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. E.g. &apos;bold&apos; shows all fonts with Bold in name. Searching is case insensitive.</source>
        <translation type="obsolete">Hier können Sie den Namen einer Schriftart oder auch nur einen Teil eingeben. Wenn Sie z.B. &quot;bold&quot; eingeben, erscheinen alle Schriftarten, in denen diese Zeichenfolge vorkommt. Die Suche berücksichtigt Groß- und Kleinschreibung.</translation>
    </message>
    <message>
        <source>Start searching</source>
        <translation>Suche starten</translation>
    </message>
    <message>
        <source>Size of the selected font</source>
        <translation>Größe der gewählten Schrift</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <comment>font preview</comment>
        <translation>Falsches Üben von Xylophonmusik quält jeden größeren Zwerg</translation>
    </message>
    <message>
        <source>User</source>
        <comment>font preview</comment>
        <translation>Benutzer</translation>
    </message>
    <message>
        <source>System</source>
        <comment>font preview</comment>
        <translation>System</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation>Die Vorschau wird nach Loslassen der Taste angezeigt</translation>
    </message>
    <message>
        <source>Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.</source>
        <translation>Wenn Sie hier Text eingeben, starten Sie eine schnelle Suche. Groß- und Kleinschreibung wird ignoriert. Sie können auch Platzhalter (*, ?, [...]) verwenden. Beispiele: t* listet alle Schriftarten auf, die mit t oder T beginnen. *bold* listet alle Schriftenarten mit fetten Schnitten auf.</translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation>Schriftartenvorschau</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation>&amp;Schnellsuche:</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Suchen</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Name der Schriftart</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation>Doc</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Unterteilen</translation>
    </message>
    <message>
        <source>Access</source>
        <translation>Zugriff</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Schrift&amp;größe:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation>Beispieltext</translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation>Än&amp;dern</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation>Text zurücksetzen</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Verwenden</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+L</translation>
    </message>
</context>
<context>
    <name>FontPreviewPlugin</name>
    <message>
        <source>&amp;Font Preview...</source>
        <translation>&amp;Schriftartenvorschau...</translation>
    </message>
    <message>
        <source>Font Preview dialog</source>
        <translation>Schriftartenvorschau-Dialog</translation>
    </message>
    <message>
        <source>Sorting, searching and browsing available fonts.</source>
        <translation>Verfügbare Schriftarten sortieren und durchsuchen.</translation>
    </message>
</context>
<context>
    <name>FontReplaceDialog</name>
    <message>
        <source>Font Substitution</source>
        <translation>Ersetzung von Schriftarten</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system,
please choose a suitable replacement for them.</source>
        <translation type="obsolete">In diesem Dokument werden einige Schriftarten verwendet, die auf Ihrem System nicht installiert sind,
bitte wählen Sie dafür geeignete Ersatz-Schriften.</translation>
    </message>
    <message>
        <source>Original Font</source>
        <translation>Originalschriftart</translation>
    </message>
    <message>
        <source>Substitution Font</source>
        <translation>Ersatzschriftart</translation>
    </message>
    <message>
        <source>Make these substitutions permanent</source>
        <translation>Schriftart dauerhaft ersetzen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>This document contains some fonts that are not installed on your system, please choose a suitable replacement for them. Cancel will stop the document from loading.</source>
        <translation>Dieses Dokument enthält einige Schriftarten, welche auf Ihrem System nicht installiert sind. 
Bitte entscheiden Sie sich für einen passenden Ersatz. &quot;Abbrechen&quot; wird das Laden des Dokuments
beenden.</translation>
    </message>
    <message>
        <source>Cancels these font substitutions and stops loading the document.</source>
        <translation>Bricht die Schriftartenersetzung ab und beendet das Laden des
Dokumentes.</translation>
    </message>
    <message>
        <source>Enabling this tells Scribus to use these replacements for missing fonts permanently in all future layouts. This can be reverted or changed in Edit &gt; Preferences &gt; Fonts.</source>
        <translation>Falls Sie diese Funktion wählen, wird Scribus diese Ersetzungen dauerhaft verwenden. Ihre Entscheidnung können Sie jedoch jederzeit im Menü Bearbeiten &gt; Einstellungen &gt; Schriftarten rückgängig machen.</translation>
    </message>
    <message>
        <source>If you select OK, then save, these substitutions are made permanent in the document.</source>
        <translation>Wenn Sie &quot;OK&quot; klicken und speichern, werden diese Schriftartenersetzungen dauerhaft
im Dokument gespeichert.</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Position:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Hier können Sie Farben hinzufügen, ändern oder löschen.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Hilfslinien bearbeiten</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Waagerechte Linien</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Senkrechte Linien</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Position:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Hin&amp;zufügen</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>Lö&amp;schen</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>Hilfslinien &amp;sperren</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Rows and Columns - Automatic Guides</source>
        <translation>Zeilen und Spalten - Automatische Hilfslinien</translation>
    </message>
    <message>
        <source>&amp;Rows:</source>
        <translation>&amp;Zeilen:</translation>
    </message>
    <message>
        <source>C&amp;olumns:</source>
        <translation>&amp;Spalten:</translation>
    </message>
    <message>
        <source>Row &amp;Gap</source>
        <translation>&amp;Zwischenraum</translation>
    </message>
    <message>
        <source>Colum&amp;n Gap</source>
        <translation>Z&amp;wischenraum</translation>
    </message>
    <message>
        <source>Refer to:</source>
        <translation>Bezieht sich auf:</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Seite</translation>
    </message>
    <message>
        <source>&amp;Margins</source>
        <translation>&amp;Ränder</translation>
    </message>
    <message>
        <source>&amp;Selection</source>
        <translation>&amp;Markierung</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Update</source>
        <translation>Ak&amp;tualisieren</translation>
    </message>
    <message>
        <source>Set the guides in document. Guide manager is still opened but the changes are persistant</source>
        <comment>guide manager</comment>
        <translation>Hilfslinien für das Dokument übernehmen. Der Hilfslinien-Manager ist noch geöffnet, aber die Änderungen sind schon sichtbar</translation>
    </message>
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation>Auf &amp;alle Seiten anwenden</translation>
    </message>
    <message>
        <source>Guide</source>
        <translation>Hilfslinien</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation>Einheit</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation>Es gibt bereits eine unsichtbare Hilfslinie (0,0)</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Leider ist kein Handbuch verfügbar! Auf http://docs.scribus.net finden Sie aktualisierte Handbücher, und auf http://www.scribus.net finden Sie weitere Downloads.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Inhalt</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Verknüpfung</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Online-Hilfe für Scribus</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Inhalt</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Suchen</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation type="obsolete">unbekannt</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Suchbegriff:</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation>Suc&amp;hen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>De&amp;lete All</source>
        <translation>&amp;Alle löschen</translation>
    </message>
    <message>
        <source>Book&amp;marks</source>
        <translation>&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>&amp;Drucken...</translation>
    </message>
    <message>
        <source>E&amp;xit</source>
        <translation>Ver&amp;lassen</translation>
    </message>
    <message>
        <source>Searching is case unsensitive</source>
        <translation>Die Suche berücksichtigt Groß- und Kleinschreibung nicht</translation>
    </message>
    <message>
        <source>New Bookmark</source>
        <translation>Neues Lesezeichen</translation>
    </message>
    <message>
        <source>New Bookmark&apos;s Title:</source>
        <translation>Titel des Lesezeichens:</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Find...</source>
        <translation>&amp;Suchen...</translation>
    </message>
    <message>
        <source>Find &amp;Next</source>
        <translation>&amp;Nächstes suchen</translation>
    </message>
    <message>
        <source>Find &amp;Previous</source>
        <translation>&amp;Vorheriges Suchen</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Add Bookmark</source>
        <translation>Lesezeichen &amp;hinzufügen</translation>
    </message>
    <message>
        <source>D&amp;elete All</source>
        <translation>All&amp;es löschen</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation>Relevanz</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Trennvorschlag</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Überspringen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Länge des kürzesten Wortes, das getrennt werden soll.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Maximale Anzahl aufeinanderfolgender Trennungen.
Null bedeutet unbegrenzt.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Kürzestes Wort:</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Trenn&amp;vorschläge</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Text automatisch während der Ein&amp;gabe trennen</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Wenn Sie die Funktion Extras &gt; Silbentrennung anwenden benutzen, können Sie in einem Dialog aus allen möglichen Trennungen für jedes Wort wählen.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Aktiviert die automatische Trennung während der Eingabe.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Aufeinanderfolgende &amp;Trennungen:</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation>Bildinformationen</translation>
    </message>
    <message>
        <source>General Info</source>
        <translation>Allgemeine Informationen</translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation>Datum/Zeit:</translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation>eingebettetes Profil:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation>Name des Profils:</translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation>eingebettete Pfade:</translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation>Ebenen:</translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation>EXIF-Informationen</translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation>Künstler:</translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation>Kommentar:</translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation>Kommentar des Benutzers:</translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation>Kamera:</translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation>Kamerahersteller:</translation>
    </message>
    <message>
        <source>Description:</source>
        <translation>Beschreibung:</translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation>Urheberrecht:</translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation>Scanner:</translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation>Scannerhersteller:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>&amp;EPS/PS-Dateien importieren...</translation>
    </message>
    <message>
        <source>Imports EPS Files</source>
        <translation>EPS-Dateien importieren</translation>
    </message>
    <message>
        <source>Imports most EPS files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importiert die meisten EPS-Dateien in das aktuelle Dokument
und wandelt die Vektordaten in Scribus-Objekte um.</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
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
        <translation>Seite einfügen</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>vor Seite</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>nach Seite</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>am Dokumentende</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Seite(n)</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Ein&amp;fügen</translation>
    </message>
    <message>
        <source>Master Page (&amp;Left Page):</source>
        <translation type="obsolete">Musterseite (&amp;Linke Seite):</translation>
    </message>
    <message>
        <source>&amp;Master Page:</source>
        <translation>&amp;Musterseite:</translation>
    </message>
    <message>
        <source>Master Page (&amp;Right Page):</source>
        <translation type="obsolete">Musterseite (&amp;Rechte Seite):</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Au&amp;srichtung:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Objekte mit der Seite verschieben</translation>
    </message>
    <message>
        <source>Master Pages</source>
        <translation>Musterseite</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Tabelle einfügen</translation>
    </message>
    <message>
        <source>Number of rows:</source>
        <translation>Anzahl der Zeilen:</translation>
    </message>
    <message>
        <source>Number of columns:</source>
        <translation>Anzahl der Spalten:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Neues Script</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>JavaScripts bearbeiten</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Hinzufügen...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Neues Script:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">Wollen Sie dieses Script wirklich löschen?</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation>Wollen Sie dieses Script wirklich löschen?</translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation>Fügt ein neues Script hinzu und erstellt eine Funktion mit demselben Namen. Wenn Sie das Script als ein &quot;Open Action&quot;-Script verwenden wollen, verändern Sie bitte nicht den Namen der Funktion.</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Action</source>
        <translation>Aktion</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Aktuelle Taste</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Tastenkombination für die ausgewählte Aktion</translation>
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
        <translation>Strg</translation>
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
        <translation>Strg+</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Keine Taste</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>&amp;Benutzerdefinierte Taste</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Taste &amp;definieren</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Diese Tastenkombination wird bereits verwendet</translation>
    </message>
    <message>
        <source>Loadable Shortcut Sets</source>
        <translation>Benutzerdefinierten Satz laden</translation>
    </message>
    <message>
        <source>&amp;Load</source>
        <translation>&amp;Laden</translation>
    </message>
    <message>
        <source>&amp;Import...</source>
        <translation>&amp;Importieren...</translation>
    </message>
    <message>
        <source>&amp;Export...</source>
        <translation>E&amp;xportieren...</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Zurücksetzen</translation>
    </message>
    <message>
        <source>Keyboard shortcut sets available to load</source>
        <translation>Verfügbare Tastenkürzel-Sätze</translation>
    </message>
    <message>
        <source>Load the selected shortcut set</source>
        <translation>Gewählten Satz laden</translation>
    </message>
    <message>
        <source>Import a shortcut set into the current configuration</source>
        <translation>Tastenkürzel-Satz in die aktuelle Konfiguration importieren</translation>
    </message>
    <message>
        <source>Export the current shortcuts into an importable file</source>
        <translation>Tastenkürzel-Satz exportieren</translation>
    </message>
    <message>
        <source>Reload the default Scribus shortcuts</source>
        <translation>Standard-Tastenkürzel von Scribus erneut laden</translation>
    </message>
    <message>
        <source>Key Set XML Files (*.ksxml)</source>
        <translation>XML-Tastenkürzel-Satz (*.ksxml)</translation>
    </message>
    <message>
        <source>This key sequence is already in use</source>
        <translation>Diese Tastenkombination ist schon belegt</translation>
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
        <translation>Ebenen</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Neue Ebene hinzufügen</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Ebene löschen</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Ebene nach oben verschieben</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Ebene nach unten verschieben</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Neue Ebene</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation>Wollen Sie alle Objekte auf dieser Ebene löschen?</translation>
    </message>
    <message>
        <source>Add a new layer</source>
        <translation>Neue Ebene hinzufügen</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Ebene löschen</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Ebene nach oben verschieben</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Ebene nach unten verschieben</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Linienstile bearbeiten</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Neuer Stil</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Anfügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Wollen Sie diesen Absatzstil wirklich löschen?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation>Möchten Sie diesen Stil wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>LineStyleWBase</name>
    <message>
        <source>LineStyleWBase</source>
        <translation>LinienStileWBase</translation>
    </message>
    <message>
        <source>%</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linienbreite:</translation>
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
        <translation>Flach</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Quadratisch</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Abgerundet</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spitz</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Gefasst</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Durchgehend</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Gestrichelt</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Gepunktet</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Strich-Punkt</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Strich-Punkt-Punkt</translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
</context>
<context>
    <name>LoremManager</name>
    <message>
        <source>Select Lorem Ipsum</source>
        <translation>Beispieltext wählen</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Get More:</source>
        <translation>Mehr:</translation>
    </message>
    <message>
        <source>XML File:</source>
        <translation>XML-Datei:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Beispieltext</translation>
    </message>
    <message>
        <source>Paragraphs:</source>
        <translation>Absätze:</translation>
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
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
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
        <translation>Seiteneigenschaften bearbeiten</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Aus&amp;richtung:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation>Objekte zusammen mit der Seite verschieben</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Typ:</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Rand des Satzspiegels und dem Seitenrand (Kopfsteg)</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Rand des Satzspiegels und dem Seitenrand (Fußsteg)</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Abstand zwischen dem linken Rand des Satzspiegels und dem Seitenrand.
Wenn Doppelseiten aktiviert sind, kann hier der für die Bindung notwendige 
Abstand (Bundsteg) eingestellt werden</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Abstand zwischen dem rechten Rand des Satzspiegels und dem Seitenrand. 
Wenn Doppelseiten aktiviert sind, kann hier der für die Bindung notwendige 
Abstand eingestellt werden</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Auße&amp;n:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation>Standard-Layouts:</translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation>Seitenränder auf alle Seiten anwenden</translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation>Seitenränder auf alle vorhandenen Seiten im Dokument anwenden</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante. Wenn Doppelseiten aktiviert sind, können Sie diese Einstellung verwenden, um korrekte Bindungen zu erreichen</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante. Wenn Doppelseiten aktiviert sind, können Sie diese Einstellung verwenden, um korrekte Bindungen zu erreichen</translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation>Druckerränder...</translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
        <translation>Randeinstellung von einem vorhandenen Drucker importieren.</translation>
    </message>
</context>
<context>
    <name>MasterPagesPalette</name>
    <message>
        <source>Edit Master Pages</source>
        <translation>Musterseiten bearbeiten</translation>
    </message>
    <message>
        <source>Duplicates the selected master page</source>
        <translation type="obsolete">Dupliziert die aktuelle Musterseite</translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="obsolete">Löscht die aktuelle Musterseite</translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="obsolete">Fügt eine neue Musterseite hinzu</translation>
    </message>
    <message>
        <source>Imports master pages from another document</source>
        <translation type="obsolete">Importiert eine Musterseite aus einem anderen Dokument</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation>Wollen Sie diese Musterseite wirklich löschen?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation>Neue Musterseite</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation>Neue Musterseite</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie #%1 von</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation>Markierte Musterseite duplizieren</translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation>Markierte Musterseite löschen</translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation>Neue Musterseite hinzufügen</translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation>Musterseite von einem anderen Dokument importieren</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Neue Musterseite %1</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Mehrfach Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>An&amp;zahl der Kopien:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Horizontaler Versatz:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Vertikaler Versatz:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Abstände</translation>
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
        <translation>Winkel:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Länge:</translation>
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
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Seiten importieren</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>von 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Seiten erstellen</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>von %1</translation>
    </message>
    <message>
        <source>Import Master Page</source>
        <translation>Musterseite importieren</translation>
    </message>
    <message>
        <source>&amp;From Document:</source>
        <translation>von &amp;Dokument:</translation>
    </message>
    <message>
        <source>Chan&amp;ge...</source>
        <translation>Än&amp;dern...</translation>
    </message>
    <message>
        <source>&amp;Import Page(s):</source>
        <translation>Seite(n) &amp;importieren:</translation>
    </message>
    <message>
        <source>&amp;Import Master Page</source>
        <translation>&amp;Musterseite importieren</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens import where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Geben Sie mit Hilfe von Kommata oder Bindestrichen ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Vor Seite</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Nach Seite</translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Am Ende</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation>Fehlende Schriftart</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>Die Schrift %1 ist nicht installiert.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Benutze</translation>
    </message>
    <message>
        <source>instead</source>
        <translation>anstatt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Seiten verschieben</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Seite kopieren</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Seite(n) verschieben von:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">bis:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Move Page(s)</source>
        <translation>Seite(n) verschieben</translation>
    </message>
    <message>
        <source>Before Page</source>
        <translation>Vor Seite</translation>
    </message>
    <message>
        <source>After Page</source>
        <translation>Nach Seite </translation>
    </message>
    <message>
        <source>At End</source>
        <translation>Ans Ende</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation>Anzahl der Kopien:</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometrie</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Ursprung:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Form:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Abstand des Textes</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Kurve zeigen</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Anfangsposition:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Abstand zur Kurve:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Farbprofil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Renderpriorität:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ farbmetrisch</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Farbsättigung</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut farbmetrisch</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Linker Punkt</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Endpunkte</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spitz</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Gefasst</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flach</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Quadratisch</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Abgerundet</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Zeilenabstand</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Name des markierten Objekts</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Horizontale Position des aktuellen Ursprungs</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Vertikale Position des aktuellen Ursprungs</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Breite</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Höhe</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Drehung des Objekts um den aktuellen Ursprung</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Ursprungspunkt</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Links oben als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Rechts oben als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Links unten als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Rechts unten als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Mittelpunkt als Ursprung setzen</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Horizontal spiegeln</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Vertikal spiegeln</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Weiter nach vorn verschieben</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Weiter nach hinten verschieben</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>In den Vordergrund bringen</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>In den Hintergrund bringen</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Objekt sperren oder entsperren</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Größenänderung sperren oder entsperren</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Drucken aktivieren oder deaktivieren</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Schriftart des Objekts</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Zeichenbreiten skalieren</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Textumrissfarbe</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Textfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Tonwert der Textumrissfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Tonwert der Textfarbe</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilvorlage für aktuellen Absatz</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Ändert die Einstellungen für die linken und die Endpunkte</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Muster der Line</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Linienbreite</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Art der Ecken</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Art der Enden</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Linienstil des Objekts</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Rahmenform auswählen...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Rahmenform bearbeiten...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Radius der Eckrundung setzen</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Anzahl der Spalten im Textrahmen</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Spaltenabstand</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Abstand des Textes vom oberen Rahmenrand</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Abstand des Textes vom unteren Rahmenrand</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Abstand des Textes vom linken Rahmenrand</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Abstand des Textes vom rechten Rahmenrand</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Tabulatoren setzen/bearbeiten...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Erlaube verschiedene Größen für Bild und Rahmen</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Horizontaler Versatz des Bildes innerhalb des Rahmens</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Vertikaler Versatz des Bildes innerhalb des Rahmens</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Bild horizontal skalieren</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Bild vertikal skalieren</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Seitenverhältnis beibehalten</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Bild füllt den Rahmen aus</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Benutzt die Bildproportionen anstelle der Rahmenproportionen</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Zellumrandungen</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Oben</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Unten</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Seitenverhältnisse beibehalten</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Farbprofil des Bildes</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Render-Methode für das Bild</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Wechsel zwischen der Breite des Zwischenraums und der Spalten</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Spaltenbreite</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Eigenschaften des Pfadtextes</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Text in untergeordneten Textrahmen um die Objektform fließen lassen</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Zeigt an, auf welcher Ebene sich das Objekt befindet. 0 bedeutet im Vordergrund</translation>
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
        <translation>&amp;Text</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Bild</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linien</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>Fa&amp;rben</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Position:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Text umfließt Ra&amp;hmen</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>&amp;Bounding Box benutzen</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Objektumriss benutzen</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Form bearbeiten...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Ecken
ab&amp;runden:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Spalten:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Abstand:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Tabulatoren...</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>S&amp;til:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Freie Skalierung</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>&amp;X-Größe:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Y-Größe:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>An Rahmen an&amp;passen</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proportional</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>&amp;Ursprung:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Linien&amp;art:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linien&amp;breite:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Ecken:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>En&amp;den:</translation>
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
        <translation>Für den Textumfluss wird das umgebende Rechteck statt der Form genutzt</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Für den Textumfluss wird eine zweite Linie benutzt, die auf der ursprünglichen Form basiert</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Spracheinstellung für die Silbentrennung</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Rechtsläufiger Text</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manueller Zeichenabstand</translation>
    </message>
    <message>
        <source>Fixed Linespacing</source>
        <translation>Fester Zeilenabstand</translation>
    </message>
    <message>
        <source>Automatic Linespacing</source>
        <translation>Automatischer Zeilenabstand</translation>
    </message>
    <message>
        <source>Align to Baseline Grid</source>
        <translation>Am Grundlinienraster ausrichten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Actual X-DPI:</source>
        <translation>X-dpi aktuell:</translation>
    </message>
    <message>
        <source>Actual Y-DPI:</source>
        <translation>Y-dpi aktuell:</translation>
    </message>
    <message>
        <source>Start Arrow:</source>
        <translation>Anfangspfeil:</translation>
    </message>
    <message>
        <source>End Arrow:</source>
        <translation>Endpfeil:</translation>
    </message>
    <message>
        <source>Offset to baseline of characters</source>
        <translation>Abstand der Zeichen von der Grundlinie</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Höhe der Zeichen</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Der Name &quot;%1&quot; ist nicht eindeutig.&lt;br/&gt; Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation>Farbe der Textumrandung. Nur möglich, wenn als Textoption &quot;Umriss&quot; gewählt ist.</translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation>Farbe der Textfüllung. Nur möglich, wenn als Textoption &quot;Umriss&quot; gewählt ist.</translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation>Fülloption</translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation>Gerade-Ungerade</translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation>Nicht Null</translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Stil bearbeiten</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Flach</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Quadratisch</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Abgerundet</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Spitz</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Gefasst</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Rund</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linienbreite:</translation>
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
        <translation> pt</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Durchgehend</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Gestrichelt</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Gepunktet</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Strich-Punkt</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Strich-Punkt-Punkt</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation>Der Name &quot;%1&quot; ist nicht eindeutig.&lt;br/&gt; Bitte wählen Sie einen anderen.</translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation>Fortschritt</translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation>Gesamter Fortschritt:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>MyPlugin</name>
    <message>
        <source>My &amp;Plugin</source>
        <translation>Mein &amp;PlugIn</translation>
    </message>
</context>
<context>
    <name>MyPluginImpl</name>
    <message>
        <source>Scribus - My Plugin</source>
        <translation>Scribus - Mein PlugIn</translation>
    </message>
    <message>
        <source>The plugin worked!</source>
        <translation>Das PlugIn funktioniert!</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>New Document</source>
        <translation>Neues Dokument</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Seitenränder</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Seitengröße des Dokuments, entweder eine Standardgröße oder benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Ausrichtung der Seiten</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Breite der Seite</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Höhe der Seite</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktiviert doppelseitiges Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Macht die erste Seite zu der linken Seite des Dokuments</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="obsolete">Nummer der ersten Seite des Dokuments</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standardmaßeinheit für das Dokument</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Textrahmen automatisch erstellen, wenn neue Seiten hinzugefügt werden</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Anzahl der Spalten in den automatisch erzeugten Textrahmen</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Abstand zwischen den Spalten</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Au&amp;srichtung:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Nummer der &amp;ersten Seite:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Standard&amp;maßeinheit:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Automatische &amp;Textrahmen</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Ab&amp;stand:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Spa&amp;lten:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Open Document</source>
        <translation type="obsolete">Dokument öffnen</translation>
    </message>
    <message>
        <source>Initial number of pages of the document</source>
        <translation>Anzahl der Seiten, die erstellt werden sollen</translation>
    </message>
    <message>
        <source>N&amp;umber of Pages:</source>
        <translation>&amp;Anzahl der Seiten:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumente (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="obsolete">Zuletzt verwendete Dokumente</translation>
    </message>
    <message>
        <source>Do not show this dialog again</source>
        <translation>Diesen Dialog nicht mehr zeigen</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript-Dateien (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG-Bilder (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG-Bilder (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Zeichnung (*.sxd);;</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation>&amp;Neues Dokument</translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation>Vor&amp;handenes Dokument öffnen</translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation>Dokument aus dem &amp;Verlauf öffnen</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Neu von &amp;Vorlage...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation>Dokument mit vordefiniertem Layout laden</translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation>Beginnt ein neues Dokument aus einer Vorlage, die Sie oder andere erstellt haben (z. B. für Dokumente mit gleichbleibendem Stil).</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Linke Seite</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Rechte Seite</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Pfade</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Punkte verschieben</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Kontrollpunkte verschieben</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Punkte hinzufügen</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Punkte löschen</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Kontrollpunkte zurücksetzen</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Diesen Kontrollpunkt zurücksetzen</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Absolute Koordinaten</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Position:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Position:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>&amp;Objektumriss bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Objektumriss &amp;zurücksetzen</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Bearbeitung &amp;beenden</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Kontrollpunkte unabhängig verschieben</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Kontrollpunkte symmetrisch verschieben</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Bézierkurve oder Polygon öffnen</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Bézierkurve schließen</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Pfad horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Pfad vertikal spiegeln</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Pfad nach links scheren</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Pfad nach oben scheren</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Pfad nach unten scheren</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Pfad gegen den Uhrzeigersinn drehen</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Pfad mit dem Uhrzeigersinn drehen</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="obsolete">Pfad verkleinern um x %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Pfad vergrößern um x %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Grad der Drehung</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="obsolete">% vergrößern oder verkleinern</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Konturmodus aktivieren</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Kontur auf die originale Form zurücksetzen</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Pfad nach rechts scheren</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation>Wenn aktiviert, werden die Koordinaten relativ zur Seite verwendet, andernfalls die Koordinaten absolut zum Objekt.</translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation>Größe des Pfads verringern um x %</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation>Größe des Pfads um den angezeigten Wert verringern</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation>Größe des Pfads um den angezeigten Wert vergrößern</translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation>Prozent vergrößern/verkleinern</translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation>Um den Wert vergrößern/verkleinern</translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation>Diese Datei scheint keine OpenOffice.org-Zeichnung zu sein.</translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>&amp;OpenOffice.org-Zeichnung importieren...</translation>
    </message>
    <message>
        <source>Imports OpenOffice Draw Files</source>
        <translation type="obsolete">Importiert OpenOffice.org Draw-Dateien</translation>
    </message>
    <message>
        <source>Imports most OpenOffice Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="obsolete">Importiert die meisten OpenOffice.org Draw-Dateien in das aktuelle Dokument und wandelt deren Vektordaten in Scribus-Objekte um.</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation>Importiert OpenOffice.org-Draw-Dateien</translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation>Importiert die meisten OpenOffice.org-Draw-Dateien in das aktuelle Dokument und wandelt deren Vektordaten in Scribus-Objekte um.</translation>
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
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Name der Datei vor jeden Absatzstil anfügen</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Einstellungen speichern</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Optionen für den Import von OpenDocument-Dateien</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Existierende Stile im aktuellen Dokument überschreiben</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Absatzstile zusammenfügen</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Absatzstile nach Attributen zusammenfügen. Das verringert die Anzahl der Absatzstile, selbst wenn diese im Originaldokument anders benannt sind.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Name des Dokuments an den Absatzstil in Scribus anhängen.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Diese Einstellungen zum Standard machen und beim Import von OASIS-Dateien nicht nochmals nachfragen.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Absatzstile überschreiben</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation>Scribus-Dokument</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation>Scribus 1.2.x-Dokument</translation>
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
        <translation>Linienstärke</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation>Als PDF speichern</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Ausgabe in &amp;Datei:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Än&amp;dern...</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation>Eine Datei &amp;pro Seite speichern</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="obsolete">Diese Funktion ermöglicht den Export genau einer PDF-Datei pro Seite. Seitennummern werden automatisch eingefügt.
Dies ist außerordentlich nützlich für das Ausschießen von PDF-Dateien im professionellen Druck.</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation>Jede Seite wird mit automatischer Nummerierung als separate PDF-Datei exportiert. Hilfreich fürs Ausschießen.</translation>
    </message>
    <message>
        <source>The save button will be disabled if you are trying to export PDF/X-3 and the info string is missing from the PDF/X-3 tab.</source>
        <translation>Sie können die PDF-Datei erst speichern, wenn Sie einen Infotext im PDF/X-3-Register angegeben haben.</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Speichern unter</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="obsolete">Ausgabe in &amp;Datei:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Speichern</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">PDF-Dateien (*.pdf);;Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="obsolete">Als PDF speichern</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="obsolete">Ausgabe in eine Datei &amp;pro Seite</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="obsolete">Diese Funktion ermöglicht den Export genau einer PDF-Datei pro Seite. Seitennummern werden automatisch eingefügt.
Dies ist außerordentlich nützlich für das Ausschießen von PDF-Dateien im professionellen Druck.</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation>PDF speichern</translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation>Musterseite wird exportiert:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Seiten werden exportiert:</translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation>Analysiere Objekte auf der Seite:</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Druckvorschau</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Stellt den Text auf Kosten der Render-Geschwindigkeit glatter dar.
Funktioniert nur mit Type-1-Schriftarten</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Aktiviert Transparenzen und transparente Objekt im Dokument. Erfordert Ghostscript 7.07 oder höher</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Benutzt zur Vorschau CMYK anstelle der RGB-Farben</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Aktiviert/deaktiviert den Cyan-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Aktiviert/deaktiviert den Magenta-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Aktiviert/deaktiviert den Gelb-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Aktiviert/deaktiviert den Schwarz-Auszug</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Text &amp;weichzeichnen</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Bil&amp;der weichzeichnen</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>&amp;Transparenz aktivieren</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;CMYK anzeigen</translation>
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
        <translation>&amp;Unterfarbenreduktion</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Unterfarbenreduktion ersetzt einige Grautöne, die beim Mischen von Cyan, Magenta und
Yellow entstehen, durch Schwarz. Dadurch kann sich die Qualität von Bilder, die vor allem
neutrale Farbtöne oder Grautöne enthalten, verbessern. Allerdings sollte Sie da immer ein
bisschen experimentieren, wie Sie die besten Ergebnisse erhalten.
UFR reduziert auch die Gefahr einer Übersättigung durch die CMY-Farben.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Schriftarten (TrueType &amp;&amp; OpenType), EPS, PDF und Vektorgrafiken werden schöner dargestellt,
dafür erhöht sich benötigte Renderingzeit gerinfügig</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="obsolete">Es sind Fehler aufgetreten.
Benutzen Sie den Druckvorstufen-Check, um die Fehler zu beheben</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="obsolete">Ignorieren</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation>Separationsbezeichnung</translation>
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
        <translation>Gelb</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Schwarz</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation>Zoom:</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Drucken...</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation>Stellt den Text auf Kosten der Rendergeschwindigkeit glatter dar.
Funktioniert nur mit Type-1-Schriftarten</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Schriftarten (TrueType und OpenType), EPS, PDF und Vektorgrafiken werden schöner dargestellt.
Dafür erhöht sich benötigte Renderzeit geringfügig</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Mit dieser Option werden Grautöne, die aus Cyan, Magenta und Gelb gemischt würden, 
durch reine Abstufungen von Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr ähnlich sind. Diese Option kann zu besseren Bildern führen.
Sie müssen jedoch von Fall zu Fall entscheiden, ob Sie bessere Ergebnisse
erzielen. Zudem reduziert UFR die Gefahr der Übersättigung.</translation>
    </message>
    <message>
        <source>Resize the scale of the page.</source>
        <translation>Vorschaugröße.</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation>Musterseiten werden verarbeitet:</translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation>Seiten werden exportiert:</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
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
        <translation>Polyline</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Pfadtext</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Kopie von</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Relates To</source>
        <translation>Verweist auf</translation>
    </message>
    <message>
        <source>Is Parent Of</source>
        <translation>Ist Elternobjekt von</translation>
    </message>
    <message>
        <source>Is Child Of</source>
        <translation>Ist Kindobjekt von</translation>
    </message>
    <message>
        <source>Page Item Attributes</source>
        <translation type="obsolete">Eigenschaften des Seitenobjekts</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="obsolete">Name</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Wert</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation type="obsolete">Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation type="obsolete">Beziehung</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation type="obsolete">Beziehung zu</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+H</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Kopieren</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">&amp;Inhalt löschen</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation>Keine</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation>Eigenschaften des Seitenobjekts</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Wert</translation>
    </message>
    <message>
        <source>Parameter</source>
        <translation>Parameter</translation>
    </message>
    <message>
        <source>Relationship</source>
        <translation>Beziehung</translation>
    </message>
    <message>
        <source>Relationship To</source>
        <translation>Beziehung zu</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
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
        <translation>A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>PageLayouts</name>
    <message>
        <source>Page Layout</source>
        <translation>Seitenlayout</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="obsolete">Einzelne Seite</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation type="obsolete">Doppelseiten</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="obsolete">3-fach gefaltet</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="obsolete">4-fach gefaltet</translation>
    </message>
    <message>
        <source>Page #</source>
        <translation type="obsolete">Seite #</translation>
    </message>
    <message>
        <source>is the first Page.</source>
        <translation type="obsolete">ist die erste Seite.</translation>
    </message>
    <message>
        <source>First Page is:</source>
        <translation>Erste Seite:</translation>
    </message>
</context>
<context>
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation>Doppelseiten</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation>Mitte rechts</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation>Ziehen Sie einzelne Seiten oder Musterseiten auf den Papierkorb, um sie zu löschen</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation>Dies sind alle vorhandenen Musterseiten. Ziehen Sie eine Musterseite in das Fenster mit den Seitensymbolen, um eine neue Seite zu erstellen</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Seiten anordnen</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation>Verfügbare Musterseiten:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Seiten des Dokuments:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Seite </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> von %1</translation>
    </message>
    <message>
        <source>%1 of %1</source>
        <translation>%1 von %1</translation>
    </message>
    <message>
        <source>%1 of %2</source>
        <translation>%1 von %2</translation>
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
        <translation>Kanzleipapier</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>US-Letter</translation>
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
        <translation>Mittel</translation>
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
        <translation>Ergebnis</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Suchergebnis für: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Auswählen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Goto</source>
        <translation>Gehe zu</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Nicht vorhanden</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Pfad</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Drucken</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation>Suche abbrechen</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Bilder verwalten</translation>
    </message>
    <message>
        <source>Scribus - Image Search</source>
        <translation>Scribus-Bildersuche</translation>
    </message>
    <message>
        <source>The search failed: %1</source>
        <translation>Die Suche war nicht erfolgreich: %1</translation>
    </message>
    <message>
        <source>No images named &quot;%1&quot; were found.</source>
        <translation>Keine Bilder mit dem Namen &quot;%1&quot; gefunden .</translation>
    </message>
    <message>
        <source>Select a base directory for search</source>
        <translation>Startverzeichnis für Suche auswählen</translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Als &amp;Bild speichern...</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Als Bild exportieren</translation>
    </message>
    <message>
        <source>Exports selected pages as bitmap images.</source>
        <translation>Exportiert die ausgewählten Seiten als Bitmap-Dateien.</translation>
    </message>
</context>
<context>
    <name>PluginManager</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">fest</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Importieren</translation>
    </message>
    <message>
        <source>Standard</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Standard</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Unbekannt</translation>
    </message>
    <message>
        <source>Cannot find plugin</source>
        <comment>plugin manager</comment>
        <translation>PlugIn ist nicht vorhanden</translation>
    </message>
    <message>
        <source>unknown error</source>
        <comment>plugin manager</comment>
        <translation>Unbekannter Fehler</translation>
    </message>
    <message>
        <source>Cannot find symbol (%1)</source>
        <comment>plugin manager</comment>
        <translation>Kann das Symbol %1 nicht finden</translation>
    </message>
    <message>
        <source>Plugin: loading %1</source>
        <comment>plugin manager</comment>
        <translation>Lade Plug-In %1</translation>
    </message>
    <message>
        <source>init failed</source>
        <comment>plugin load error</comment>
        <translation>Laden fehlgeschlagen</translation>
    </message>
    <message>
        <source>unknown plugin type</source>
        <comment>plugin load error</comment>
        <translation>Unbekannter PlugIn-Typ</translation>
    </message>
    <message>
        <source>Plugin: %1 loaded</source>
        <comment>plugin manager</comment>
        <translation>PlugIn: %1 geladen</translation>
    </message>
    <message>
        <source>Plugin: %1 failed to load: %2</source>
        <comment>plugin manager</comment>
        <translation>PlugIn: %1 konnte geladen werden: %2</translation>
    </message>
    <message>
        <source>PostScript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript-Dateien (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG-Bilder (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG-Bilder (*.svg);;</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org-Zeichnung (*.sxd);;</translation>
    </message>
</context>
<context>
    <name>PluginManagerPrefsGui</name>
    <message>
        <source>Plugin Manager</source>
        <translation>PlugIn-Manager</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation>PlugIn</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation>Aufruf</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation>Laden?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation>PlugIn-ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation>Sie müssen Scribus neu starten, damit die Änderungen wirksam werden.</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygon-Eigenschaften</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Ec&amp;ken:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Faktor über&amp;nehmen</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fa&amp;ktor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Zahl der Ecken des Polygons</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grad der Drehung des Polygons</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Hier stellen Sie ein, wie stark konvex oder konkav das Polygon gezeichnet wird</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Vorschau der gewählten Eigenschaften</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Ein negativer Wert steht für eine konkave (sternförmige) Figur,
ein positiver Wert für eine konvexe Form</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation>Ein negativer Wert steht für eine konkave (sternförmige) Figur, ein positiver Wert für eine konvexe Form</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Bibliothek</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Oberfläche</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Pfade</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Automatisch Speichern</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="obsolete">Andere Optionen</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Vorschau</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">Klein</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Mittel</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hilfslinien</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Externe Tools</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Postscript-Interpreter</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Bildbearbeitungsprogramm</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="obsolete">Drucken</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Auswahl der Standard-Fensterdekoration.
Scribus stellt alle verfügbaren KDE und QT-Themen zur Verfügung</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Schriftgröße für Menüs und Dialoge</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Maßeinheit für Dokumente</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Anzahl der Zeilen, die Scribus bei der Bewegung des Mausrades scrollen soll</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Anzahl der zuletzt geöffneten Dokumente im Dateimenü</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standardverzeichnis für Dokumente</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standardverzeichnis für Scripte</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standardgröße der Seite</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standardausrichtung der Seite in neuen Dokumenten</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Breite der Seite, veränderbar, wenn Benutzerdefiniert ausgewählt ist</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Höhe der Seite, veränderbar, wenn Benutzerdefiniert ausgewählt ist</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktiviert doppelseitiges Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Die erste Seite im Dokument soll eine linke Seite sein</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="obsolete">Scribus speichert im angegebenen Intervall selbstständig eine Sicherungskopie 
des Dokuments mit der Erweiterung .bak</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Intervall zwischen zwei automatischen Sicherungen</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation type="obsolete">Ändert die Größe der Vorschau im Bibliotheksfenster</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Farbe des Seitenhintergrundes</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Zeigt den nicht druckbaren Bereich in der Farbe der Seitenrandlinien</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Legt die Standard-Vergrößerungseinstellung fest</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Pfad zum Ghostscript-Interpreter</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Text bei EPS- und PDF-Anzeige weichzeichnen</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Grafiken bei EPS- und PDF-Anzeige weichzeichnen</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">Objekte außerhalb der Seitenränder auf den gedruckten oder exportierten Seiten nicht zeigen</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="obsolete">Bibliothek nach jeder Veränderung erneut speichern</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Thema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">Schrift&amp;größe:</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Radvorschub:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Zuletzt verwendete Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Dokumente:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>Aus&amp;wählen...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-Profile:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ausw&amp;ählen...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripte:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Auswä&amp;hlen...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Aus&amp;richtung:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">&amp;Aktiviert</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="obsolete">Änderungen beim &amp;Verlassen speichern</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="obsolete">Groß</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Nicht druck&amp;baren Rand in der Seitenrandlinienfarbe zeigen</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Größe des Displays an&amp;passen</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>Name der &amp;ausführbaren Datei:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>&amp;Text weichzeichnen</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>&amp;Bilder weichzeichnen</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Name der &amp;ausführbaren Datei:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="obsolete">Auf Sei&amp;tenränder beschneiden</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation type="obsolete">Unterfarben&amp;reduktion anwenden</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Auswäh&amp;len...</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Mit dieser Option werden Grautöne, die aus Cyan, Magenta und Gelb gemischt würden, 
durch reine Abstufungen von Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr ähnlich sind. Diese Option kann zu besseren Bildern führen.
Sie müssen jedoch von Fall zu Fall entscheiden, ob Sie bessere Ergebnisse
erzielen. Zudem reduziert UFR die Gefahr der Übersättigung.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="obsolete">Pfad zum Bildbearbeitungsprogramm. Wenn Sie GIMP benutzen möchten
und es in Ihrer Distro enthalten ist, ist der Befehl gimp-remote empfehlenswert,
weil Sie Bilder in bereits laufenden Instanzen von GIMP bearbeiten können.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Sprache:</translation>
    </message>
    <message>
        <source>Document T&amp;emplates:</source>
        <translation>Dokument&amp;vorlagen:</translation>
    </message>
    <message>
        <source>Units:</source>
        <translation>Einheiten:</translation>
    </message>
    <message>
        <source>Undo/Redo</source>
        <translation>Rückgängig/Wiederholen</translation>
    </message>
    <message>
        <source>Action history length</source>
        <translation>Anzahl der Rückgängig/Wiederherstellen-Aktionen</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Silbentrennung</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Schriftarten</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Farbmanagement</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF-Export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Eigenschaften des Dokumentobjekts</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Inhaltsverzeichnisse
und 
Indizes</translation>
    </message>
    <message>
        <source>Keyboard Shortcuts</source>
        <translation>Tastenkürzel</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farbe:</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Bilder anzeigen</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Verkettung von Textrahmen anzeigen</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Kontrollzeichen anzeigen</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Rahmen anzeigen</translation>
    </message>
    <message>
        <source>Scratch Space</source>
        <translation>Ablagefläche</translation>
    </message>
    <message>
        <source>Always ask before fonts are replaced when loading a document</source>
        <translation>Immer nachfragen, bevor Schriftarten ersetzt werden</translation>
    </message>
    <message>
        <source>Preview of current Paragraph Style visible when editing Styles</source>
        <translation>Vorschau des aktuellen Absatzstils während der Bearbeitung anzeigen</translation>
    </message>
    <message>
        <source>Miscellaneous</source>
        <translation>Verschiedenes</translation>
    </message>
    <message>
        <source>Plugin Manager</source>
        <translation type="obsolete">Plugin-Manager</translation>
    </message>
    <message>
        <source>Plugin</source>
        <translation type="obsolete">Plugin</translation>
    </message>
    <message>
        <source>How to run</source>
        <translation type="obsolete">Aufruf</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Typ</translation>
    </message>
    <message>
        <source>Load it?</source>
        <translation type="obsolete">Laden?</translation>
    </message>
    <message>
        <source>Plugin ID</source>
        <translation type="obsolete">Plugin-ID</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Datei</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nein</translation>
    </message>
    <message>
        <source>You need to restart the application to apply the changes.</source>
        <translation type="obsolete">Sie müssen Scribus neu starten, damit die Änderungen wirksam werden.</translation>
    </message>
    <message>
        <source>Plugins</source>
        <translation>PlugIns</translation>
    </message>
    <message>
        <source>Turns the display of linked frames on or off</source>
        <translation type="obsolete">Schaltet die Anzeige von verketteten Textrahmen an oder ab</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Schaltet die Anzeige von Textrahmen an oder aus</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Schaltet die Anzeige von Bildern an oder aus</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with.
Leave this blank to choose based on environment variables.
You can still override this by passing a command line option when starting Scribus</source>
        <translation type="obsolete">Wählen Sie die Sprache, in der Scribus starten soll.
Ist das Feld leer, liest Scribus die entsprechende Umgebungsvariable aus.
Sie können auch eine Kommandozeilenoption beim Start von Scribus angeben,
die diese Einstellung überschreibt</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation type="obsolete">Verzeichnis für die ICC-Profile. Diese Einstellung kann
nur geändert werden, wenn kein Dokument geöffnet ist.</translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation>Zusätzliches Verzeichnis für Dokumentvorlagen</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="obsolete">Legt die Größe des Bearbeitungsverlaufes in Schritten fest.
0 bedeutet unendlich.</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Druckvorstufenüberprüfung</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the slider.</source>
        <translation>Verschieben Sie das Lineal mit dem Regler, um die Anzeige anzupassen.</translation>
    </message>
    <message>
        <source>dpi</source>
        <translation>dpi</translation>
    </message>
    <message>
        <source>Resolution:</source>
        <translation>Auflösung:</translation>
    </message>
    <message>
        <source>Lorem Ipsum</source>
        <translation>Beispieltext</translation>
    </message>
    <message>
        <source>Always use standard Lorem Ipsum</source>
        <translation>Immer den Standard-Beispieltext verwenden</translation>
    </message>
    <message>
        <source>Count of the Paragraphs:</source>
        <translation>Anzahl der Absätze:</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Nicht druckende Zeichen wie Absatzmarken in Textrahmen anzeigen</translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation>Legen Sie ein Lineal auf dem Bildschirm an und verschieben Sie den Regler, um den Zoom-Level einzustellen, damit die Dokumente in der richtigen Größe angezeigt werden</translation>
    </message>
    <message>
        <source>Defines amount of space left of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Legt den Raum links vom Dokument fest, der als Ablagefläche zur Verfügung steht</translation>
    </message>
    <message>
        <source>Defines amount of space right of the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Legt den Raum rechts vom Dokument fest, der als Ablagefläche zur Verfügung steht</translation>
    </message>
    <message>
        <source>Defines amount of space above the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Legt den Raum oberhalb des Dokumentes fest, der als Ablagefläche zur Verfügung steht</translation>
    </message>
    <message>
        <source>Defines amount of space below the document canvas available as a pasteboard for creating and modifying elements and dragging them onto the active page</source>
        <translation>Legt den Raum unterhalb des Dokumentes fest, der als Ablagefläche zur Verfügung steht</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Lineal relativ zur Seite</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Abstand zwischen den Seiten</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Horizontal:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Vertikal:</translation>
    </message>
    <message>
        <source>Show Startup Dialog</source>
        <translation>Start-Dialog zu Beginn anzeigen</translation>
    </message>
    <message>
        <source>Locate Ghostscript</source>
        <translation>Tragen Sie den Pfad zu Ghostscript ein</translation>
    </message>
    <message>
        <source>Locate your image editor</source>
        <translation>Wählen Sie Ihr Bildbearbeitungsprogramm</translation>
    </message>
    <message>
        <source>PostScript Interpreter</source>
        <translation>Postscript-Interpreter</translation>
    </message>
    <message>
        <source>Enable or disable  the display of linked frames.</source>
        <translation>Aktiviert oder deaktiviert die Anzeige von Rahmenverbindungen.</translation>
    </message>
    <message>
        <source>Select your default language for Scribus to run with. Leave this blank to choose based on environment variables. You can still override this by passing a command line option when starting Scribus</source>
        <translation>Wählen Sie die Standardsprache für Scribus aus. Tragen Sie hier nichts ein, wenn Sie die Umgebungsvariable Ihres Systems verwenden möchten. Sie können diese jederzeit umgehen, wenn Sie Scribus mit dem entsprechenden Parameter starten</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Wählen Sie die Fensterdekoration für Scribus. Alle verfügbaren KDE- oder Qt-Themen können Sie hier wählen</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open.</source>
        <translation type="obsolete">Standardverzeichnis für ICC-Profile. Kann nur geändern werden, wenn alle Dokumenten geschlossen sind.</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation>Ist diese Option aktiviert, speichert Scribus eine Sicherungskopie ihres Dokuments mit der Erweiterung .bak jedes Mal, wenn die Zeit um ist</translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation>Länge der Undo-History. 0 bedeutet unendlich viele Aktionen werden aufgezeichnet.</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation>Pfad und Dateiname Ihrer Bildbearbeitung. Wenn Sie GIMP verwenden und Ihre Distribution es unterstützt, sollten Sie &apos;gimp-remote&apos; verwenden, weil Sie Bilder dann in einer vorhandenen Instanz von gimp bearbeiten können.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis. UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Mit dieser Option werden Grautöne, die aus Cyan, Magenta und Gelb gemischt würden, durch reine Abstufungen von Schwarz ersetzt. Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst, die Grau sehr ähnlich sind. Diese Option kann zu besseren Bildern führen. Sie müssen jedoch von Fall zu Fall entscheiden, ob Sie bessere Ergebnisse erzielen. Zudem reduziert UFR die Gefahr der Übersättigung.</translation>
    </message>
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation>Schriftgröße (&amp;Menüs):</translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation>Schriftgröße (&amp;Paletten):</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation>Wählen Sie die Standard-Fensterdekoration. Scribus zeigt Ihnen alle KDE- und Qt-Stile an, falls Qt so konfiguriert ist, dass es die KDE-Plugins durchsucht.</translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation>Schriftgröße für die Werkzeugfenster</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation>Verzeichnis für ICC-Profile. Kann nur geändert werden, wenn kein Dokument geöffnet ist. Scribus sucht automatisch in den Systemverzeichnissen von Mac OSX und Windows. Unter Linux und Unix sucht Scribus in $home/.color/icc,/usr/share/color/icc und /usr/local/share/color/icc </translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Defaults</source>
        <translation>&amp;Standards laden</translation>
    </message>
    <message>
        <source>Save...</source>
        <translation>Speichern...</translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation>Einstellungen speichern</translation>
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
        <translation>Soll die Konfiguration aus Scribus 1.2 übernommen werden?</translation>
    </message>
    <message>
        <source>Scribus has detected existing Scribus 1.2 preferences files.
Do you want to migrate them to the new Scribus version?</source>
        <translation>Scribus hat eine bestehende Scribus 1.2-Konfigurationsdatei gefunden.
Möchten Sie diese in der neuen Scribus-Version verwenden?</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="obsolete">Einzelne Seite</translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation type="obsolete">Doppelseiten</translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Linke Seite</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Rechte Seite</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="obsolete">3fach gefaltet</translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="obsolete">Mitte</translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="obsolete">4fach gefaltet</translation>
    </message>
    <message>
        <source>Middle Left</source>
        <translation type="obsolete">Mitte links</translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="obsolete">Mitte rechts</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Could not open preferences file &quot;%1&quot; for writing: %2</source>
        <translation>Konnte Datei &quot;%1&quot; nicht öffnen, um in &quot;%2&quot; zu schreiben</translation>
    </message>
    <message>
        <source>Writing to preferences file &quot;%1&quot; failed: QIODevice status code %2</source>
        <translation>Das Schreiben in die Datei &quot;%1&quot; ist fehlgeschlagen: QIODevice status code %2</translation>
    </message>
    <message>
        <source>Failed to open prefs file &quot;%1&quot;: %2</source>
        <translation>Konnte die Datei &quot;%1&quot; nicht öffnen: %2</translation>
    </message>
    <message>
        <source>Failed to read prefs XML from &quot;%1&quot;: %2 at line %3, col %4</source>
        <translation>Konnte die XML-Datei &quot;%1&quot; nicht öffnen: %2 in Zeile %3, Spalte %4</translation>
    </message>
    <message>
        <source>Error Writing Preferences</source>
        <translation>Fehler beim Speichern der Einstellungen</translation>
    </message>
    <message>
        <source>Scribus was not able to save its preferences:&lt;br&gt;%1&lt;br&gt;Please check file and directory permissions and available disk space.</source>
        <comment>scribus app error</comment>
        <translation>Scribus konnte die Einstellungen nicht speichern &lt;br&gt;%1&lt;br&gt;. Bitte überprüfen Sie die Zugriffsrechte für die Datei und das Verzeichnis sowie den verfügbaren Speicherplatz.</translation>
    </message>
    <message>
        <source>Error Loading Preferences</source>
        <translation>Fehler beim Laden der Einstellungen</translation>
    </message>
    <message>
        <source>Scribus was not able to load its preferences:&lt;br&gt;%1&lt;br&gt;Default settings will be loaded.</source>
        <translation>Scribus konnte die Einstellungen nicht laden: &lt;br&gt;%1&lt;br&gt; Es werden die Standardeinstellungen geladen.</translation>
    </message>
</context>
<context>
    <name>PresetLayout</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keines</translation>
    </message>
    <message>
        <source>Book</source>
        <translation type="obsolete">Buch</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation>Zeitschrift</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, &apos;Book&apos; sets margins classically (Gutenberg). &apos;Book&apos; is proposed for two-sided documents. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Hier können Sie ein Standard-Layout auswählen.
&quot;Keines&quot; belässt die Stege wie gehabt.
&quot;Buch&quot; setzt die Stege im klassischen Buchformat.
&quot;Zeitschrift&quot; setzt alle Stege auf identische Werte.
Der ausschlaggebende Wert ist der Bundsteg.</translation>
    </message>
    <message>
        <source>Guttenberg</source>
        <translation type="obsolete">Gutenberg</translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation>Fibonacci</translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation>Goldener Schnitt</translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation>Neun Bereiche</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Guttenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="obsolete">Sie können festgelegte Seitenlayouts hier auswählen. &quot;Kein&quot; verändert die Ränder nicht, Gutenberg bedeutet klassische Ränder, Magazin setzt alle Ränder auf gleiche Werte. Maßgeblich ist der Wert für Links/Innen.</translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation>Gutenberg</translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation>Wählen Sie festgelegte Seitenlayouts. &apos;Keins&apos; verändert keine Einstellungen, Gutenberg setzt klassische Ränder, Magazine stellt alle Ränder auf den gleichen Wert. Maßgeblich ist der Wert für Links/Innen.</translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation>Keine</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation>Ö&amp;ffnen...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>&amp;Save As...</source>
        <translation type="obsolete">Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation>Ver&amp;lassen</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Run</source>
        <translation>Aus&amp;führen</translation>
    </message>
    <message>
        <source>&amp;Save Output...</source>
        <translation>Aus&amp;gabe speichern...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>Script Console</source>
        <translation>Script-Konsole</translation>
    </message>
    <message>
        <source>Write your commands here. A selection is processed as script</source>
        <translation>Schreiben Sie Ihre Befehle hier. Markierte Befehle werden als Script ausgeführt</translation>
    </message>
    <message>
        <source>Output of your script</source>
        <translation>Ausgabe des Scripts</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation>Python-Scripte (*.py)</translation>
    </message>
    <message>
        <source>Open File With Python Commands</source>
        <translation type="obsolete">Datei mit Python-Befehlen öffnen</translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation>Python-Befehle in Datei schreiben</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation>Text-Dateien (*.txt)</translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation>Aktuelle Ausgabe speichern</translation>
    </message>
    <message>
        <source>&amp;Run As Console</source>
        <translation type="obsolete">In der &amp;Konsole ausführen</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>Run As &amp;Console</source>
        <translation>In &amp;Konsole ausführen</translation>
    </message>
    <message>
        <source>Scribus Python Console</source>
        <translation>Scribus Python-Konsole</translation>
    </message>
    <message>
        <source>This is derived from standard Python console so it contains some limitations esp. in the case of whitespaces. Please consult Scribus manual for more informations.</source>
        <translation>Dies stammt von der normalen Python-Konsole, weshalb es einige Einschränkungen enthält, vor allem im Bereich Leerzeichen. Bitte lesen Sie das Scribus-Handbuch, um weitere Informationen zu erhalten.</translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation>Python Script öffnen</translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Farb&amp;ton:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sat:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>&amp;Val:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rot:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Grün:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Bla&amp;u:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>A&amp;lphakanal:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Grundfar&amp;ben</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Benutzerdefinierte Farben</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>Eigene Farben &amp;definieren &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>Zu benutzerdefinierten Farben &amp;hinzufügen</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Farbauswahl</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Datei kopieren oder verschieben</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Lesen: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Schreiben: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Dateiname:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Datei&amp;typ:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Eine Verzeichnis nach oben</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Größe</translation>
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
        <translation>Attribute</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Su&amp;chen in:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Zurück</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Neuen Ordner erstellen</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Liste</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Ausführlich</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Voransicht der Dateiinformation</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Voransicht des Dateiinhalts</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lesen/Schreiben</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Nur Lesen</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Nur Schreiben</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Gesperrt</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Link auf Datei</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Link auf Verzeichnis</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Link auf Spezialdatei</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Verzeichnis</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Spezialattribut</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>Ö&amp;ffnen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Umbenennen</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>Erne&amp;ut laden</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Nach &amp;Namen sortieren</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Nach &amp;Größe sortieren</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Nach &amp;Datum sortieren</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Unsortiert</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Sortieren</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>&amp;Versteckte Dateien anzeigen</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>die Datei</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>das Verzeichnis</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>den symbolischen Link</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>%1 löschen</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Sind Sie sicher, dass Sie %1 &quot;%2&quot; löschen möchten?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Ja</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>N&amp;ein</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Neues Verzeichnis 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Neues Verzeichnis</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Neues Verzeichnis %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Verzeichnis suchen</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Verzeichnisse</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Fehler</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Verzeichnis:</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Datei wurde nicht gefunden.
Überprüfen Sie Pfad- und Dateinamen.</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>Schrift&amp;art</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Formatierung</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Du&amp;rchgestrichen</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Unterstrichen</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>&amp;Zeichensatz</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Schriftart auswählen</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Ausrichten</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Anpassen...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Über Qt&lt;/h3&gt;&lt;p&gt;Dieses Programm verwendet Qt Version %1&lt;/p&gt;&lt;p&gt;Qt ist ein plattformübergreifendes Framework zum Erstellen von GUI-Programmen in C++.&lt;/p&gt;&lt;p&gt;Qt bietet Portierungsmöglichkeiten mit nur einer Quellcode-Basis auf MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux und allen anderen großen kommerziellen Unix-Versionen.&lt;br&gt;Qt ist auch für eingebettete Systeme erhältlich.&lt;/p&gt;&lt;p&gt;Qt ist ein Produkt von Trolltech. Weitere Informationen finden Sie unter &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Initialisierung...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Wollen sie die Datei
%1
wirklich überschreiben?</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Onlinereferenz</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-Bilder (*.svg *.svgz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-Bilder (*.svg);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Die Datei existiert bereits. Soll sie überschrieben werden?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>existiert bereits. Soll die Datei überschrieben werden?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Fehler beim Schreiben der Datei(en).</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Als Bild speichern</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Export erfolgreich.</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Newsletter</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Broschüren</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Kataloge</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Flyer</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Karten</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Briefköpfe</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Briefumschläge</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Visitenkarten</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Kalender</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Werbung</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Beschriftungen</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menüs</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programme</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF-Formulare</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Zeitschriften</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Poster</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Ankündigungen</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Textdokumente</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Hefter</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Eigene Vorlagen</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Neu von &amp;Vorlage...</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-Präsentationen</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">&amp;Als Vorlage speichern...</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Als &amp;Bild speichern...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">D&amp;ruckvorschau</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">Handbuch für den &amp;Scripter...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scripte für Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>Script &amp;ausführen...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Zuletzt verwendete Scripte</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Konsole zeigen</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Seite als &amp;SVG speichern...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">&amp;SVG importieren...</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">&amp;EPS/PS importieren...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Alle unterstützten Formate (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Text importieren</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle unterstützten Formate</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML-Dateien</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Textdateien</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Kommaseparierte Dateien</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>CVS_data</translation>
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
Externe Links</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Die Schriftart %1 ist fehlerhaft und wird ab sofort ignoriert</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Textfilter</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Cover</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanisch</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Baskisch</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgarisch</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasilianisch</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalanisch</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Chinesisch</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tschechisch</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dänisch</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Niederländisch</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Englisch</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Englisch (GB)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Deutsch (reformiert)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finnisch</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Französisch</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galizisch</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griechisch</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungarisch</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesisch</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italienisch</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreanisch</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Litauisch</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norwegisch (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norwegisch (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norwegisch</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polnisch</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisch</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Schwedisch</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spanisch</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Spanisch (Latein)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slowakisch</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovenisch</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbisch</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>Ü&amp;ber das Script...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Über das Script</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Keine Farbbezeichnung angegeben.</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Umbenennen einer Farbe ohne Bezeichnung fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kann keine Farbe ohne Bezeichnung erstellen.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kann keine Farbe ohne Bezeichnung löschen.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Kann keine Farbe ohne Bezeichnung ersetzen.</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Einheit außerhalb des Bereichs. Benutzen Sie eine der UNIT*-Konstanten.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Ziel ist kein Bildrahmen.</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Eckenradius muss positiv sein.</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann die Schriftgröße nur von Textrahmen bestimmen.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann Schriftart nur von Textrahmen bestimmen.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann die Größe des Textes nur von Textrahmen bestimmen.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann die Spaltenanzahl nur von Textrahmen bestimmen.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann den Zeilenabstand nur von Textrahmen bestimmen.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann den Spaltenabstand nur von Textrahmen bestimmen.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann Text nur von Textrahmen auslesen.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann Text nur in Textrahmen schreiben.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Kann Text nur in Textrahmen anfügen.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Ausrichtung nicht gültig. Verwenden Sie eine der scribus.ALIGN*-Konstanten.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Auswahlindex außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>Objekt ist kein verketteter Textrahmen, also kann die Verkettung nicht gelöst werden.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Kann den letzten Rahmen einer Kette nicht abtrennen. Lösen Sie erst die Verkettung der anderen Rahmen.</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="obsolete">&amp;OpenOffice.org Zeichnung importieren...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org-Zeichnung (*.sxd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>OpenOffice.org Writer-Dokumente</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Farbe nicht gefunden - Python-Fehler</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Benutzerdefinierte (optionale) Konfiguration:</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Standardkonfiguration:</translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Short &amp;Words...</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Die Umbruchkontrolle untersucht den Text. Bitte haben Sie einen Moment Geduld...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Die Umbruchkontrolle ist abgeschlossen.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Türkisch</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ukrainisch</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Walisisch</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Der Dateiname muss eine Zeichenfolge sein.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Kann den Bildtyp nicht löschen.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Der Bildtyp muss eine Zeichenfolge sein.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>&apos;allTypes&apos; kann nicht verändert werden (Read-Only)</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Beim Export des Bildes sind Fehler aufgetreten</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Farbe nicht gefunden.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Die Farbe ist im Dokument nicht vorhanden.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Die Farbe ist in den Standardfarben nicht vorhanden.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Skalieren auf 0% ist nicht möglich.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Angegebener Rahmen ist kein Bildrahmen.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Schriftart nicht gefunden.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Leere Objekte können nicht gerendert werden.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Der Name der Ebene darf nicht leer sein.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Die Ebene wurde nicht gefunden.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Die letzte Ebene kann nicht entfernt werden.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Kann keine Ebene ohne Namen erstellen.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Index ist außerhalb des gültigen Bereichs.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Die Textrichtung lässt sich nur für Textrahmen festlegen.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Schriftgröße außerhalb des Bereichs - gültig sind Werte zwischen 1 und 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Schriftgröße kann nur für Textrahmen geändert werden.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Schriftart kann nur für Textrahmen geändert werden.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Zeilenabstand ist muss größer oder gleich 0,1 sein.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Zeilenabstand kann nur für Textrahmen geändert werden.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Spaltenanzahl darf nicht negativ sein.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Spaltenabstand kann nur für Textrahmen geändert werden.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Spaltenanzahl muss größer gleich 1 sein.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Anzahl der Spalten kann nur für Textrahmen geändert werden.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Text kann nur in einem Textrahmen ausgewählt werden</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Text kann nur in einem Textrahmen gelöscht werden.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Füllfarbe des Textes kann nur für Textrahmen geändert werden.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Text kann nur in Textrahmen als durchgestrichen formatiert werden.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Text kann nur in Textrahmen als schattiert formatiert werden.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Nur Textrahmen können verkettet werden.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Der Zielrahmen muss leer sein.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Zielrahmen ist schon mit einem anderen Rahmen verknüpft.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Zielrahmen wird schon von einem anderen Textrahmen als Ziel benutzt.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Quell- und Zielrahmen sind das gleiche Objekt.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Nur für Textrahmen kann eine Verkettung aufgelöst werden.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Nur Textrahmen können in Umrisse konvertiert werden.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Nur Textrahmen können Lesezeichen besitzen</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Nur Textrahmen geben Informationen zurück</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>OpenDocument-Textdateien</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisch</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugiesisch</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugiesisch (BR)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Scribus ist abgestürzt</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Absturz durch Signal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Master Page </source>
        <translation>Musterseite</translation>
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
        <translation>Kann die Ausgabedatei %1 nicht öffnen</translation>
    </message>
    <message>
        <source>Output stream not writeable</source>
        <translation>Ausgabe kann nicht geschrieben werden</translation>
    </message>
    <message>
        <source>Verification of settings failed: %1</source>
        <translation>Überprüfung der Einstellungen fehlgeschlagen: %1</translation>
    </message>
    <message>
        <source>Could not open input file %1</source>
        <translation>Die Datei %1 kann nicht geöffnet werden</translation>
    </message>
    <message>
        <source>Unable to read settings XML:</source>
        <translation>Fehler beim Lesen der XML-Einstellungen:</translation>
    </message>
    <message>
        <source>%1 (line %2 col %3)</source>
        <comment>Load PDF settings</comment>
        <translation>%1 (Zeile %2 Spalte %3)</translation>
    </message>
    <message>
        <source>Unable to read settings XML: %1</source>
        <translation>Fehler beim Lesen der XML-Einstellungen: %1</translation>
    </message>
    <message>
        <source>null root node</source>
        <comment>Load PDF settings</comment>
        <translation>null root node</translation>
    </message>
    <message>
        <source>&lt;pdfVersion&gt; invalid</source>
        <comment>Load PDF settings</comment>
        <translation>&lt;pdfVersion&gt; ist ungültig</translation>
    </message>
    <message>
        <source>found %1 &lt;%2&gt; nodes, need 1.</source>
        <comment>Load PDF settings</comment>
        <translation>found %1 &lt;%2&gt; nodes, need 1.</translation>
    </message>
    <message>
        <source>unexpected null &lt;%2&gt; node</source>
        <comment>Load PDF settings</comment>
        <translation>unexpected null &lt;%2&gt; node</translation>
    </message>
    <message>
        <source>node &lt;%1&gt; not an element</source>
        <comment>Load PDF settings</comment>
        <translation>node &lt;%1&gt; not an element</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; lacks `value&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;%1&gt; lacks `value&apos; attribute</translation>
    </message>
    <message>
        <source>element &lt;%1&gt; value must be `true&apos; or `false&apos;</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;%1&gt; value must be `true&apos; or `false&apos;</translation>
    </message>
    <message>
        <source>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</source>
        <comment>Load PDF settings</comment>
        <translation>element &lt;lpiSettingsEntry&gt; lacks `name&apos; attribute</translation>
    </message>
    <message>
        <source>Freetype2 library not available</source>
        <translation>Freetype2-Bibliotheken sind nicht verfügbar</translation>
    </message>
    <message>
        <source>Font %1 is broken, no embedding</source>
        <translation>Die Schriftart %1 ist fehlerhaft und kann nicht eingebettet werden</translation>
    </message>
    <message>
        <source>Font %1 is broken (read stream), no embedding</source>
        <translation>Die Schriftart %1 ist fehlerhaft und kann nicht eingebettet werden</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType2), discarding it</source>
        <translation>Die Schriftart %1 ist fehlerhaft (FreeType2) und wird ignoriert</translation>
    </message>
    <message>
        <source>Font %1 is broken (no Face), discarding it</source>
        <translation>Font %1 is broken (no Face), discarding it</translation>
    </message>
    <message>
        <source>Font %1 has broken glyph %2 (charcode %3)</source>
        <translation>Die Schriftart %1 enthält das fehlerhafte Zeichen %2 (Code %3)</translation>
    </message>
    <message>
        <source>Font %1 is broken and will be discarded</source>
        <translation>Die Schriftart %1 ist fehlerhaft und wird ignoriert</translation>
    </message>
    <message>
        <source>Font %1 cannot be read, no embedding</source>
        <translation>Die Schriftart %1 kann nicht gelesen werden und wird nicht eingebettet</translation>
    </message>
    <message>
        <source>Failed to load font %1 - font type unknown</source>
        <translation>Fehler beim Laden der Schriftart %1 - Typ unbekannt</translation>
    </message>
    <message>
        <source>Font %1 loaded from %2(%3)</source>
        <translation>Schriftart %1 geladen von %2(%3)</translation>
    </message>
    <message>
        <source>Font %1(%2) is duplicate of %3</source>
        <translation>Die Schriftart %1(%2) ist ein Duplikat von %3</translation>
    </message>
    <message>
        <source>Loading font %1 (found using fontconfig)</source>
        <translation>Schriftart %1 wird geladen (erkannt von fontconfig)</translation>
    </message>
    <message>
        <source>Font %1 (found using fontconfig) is broken, discarding it</source>
        <translation>Die Schriftart %1 (erkannt von fontconfig) ist fehlerhaft und wird ignoriert</translation>
    </message>
    <message>
        <source>Failed to load a font - freetype2 couldn&apos;t find the font file</source>
        <translation>Fehler beim Laden einer Schriftart - FreeType2 konnte die Datei nicht finden</translation>
    </message>
    <message>
        <source>Font %1 is broken (FreeType), discarding it</source>
        <translation>Die Schriftart %1 ist fehlerhaft (FreeType) und wird ignoriert</translation>
    </message>
    <message>
        <source>Font %1  has invalid glyph %2 (charcode %3), discarding it</source>
        <translation>Die Schriftart %1 enthält das ungültige Zeichen %2 (Code %3) und wird ignoriert</translation>
    </message>
    <message>
        <source>extracting face %1 from font %2 (offset=%3, nTables=%4)</source>
        <translation>extracting face %1 from font %2 (offset=%3, nTables=%4)</translation>
    </message>
    <message>
        <source>memcpy header: %1 %2 %3</source>
        <translation>memcpy header: %1 %2 %3</translation>
    </message>
    <message>
        <source>table &apos;%1&apos;</source>
        <translation>Tabelle &apos;%1&apos;</translation>
    </message>
    <message>
        <source>memcpy table: %1 %2 %3</source>
        <translation>memcpy table: %1 %2 %3</translation>
    </message>
    <message>
        <source>memcpy offset: %1 %2 %3</source>
        <translation>memcpy offset: %1 %2 %3</translation>
    </message>
    <message>
        <source>Scribus Development Version</source>
        <translation>Scribus Entwicklerversion</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="obsolete">Sie benutzen eine Entwicklungsversion von Scribus 1.3.x. Das Dokument, das Sie gerade bearbeiten, wurde mit Scribus 1.2.2 oder niedriger erstellt. Wenn Sie die Datei abspeichern, können Sie sie nicht mehr in Scribus 1.2.2 verwenden, es sei denn, Sie speichern das Dokument über Datei-&gt;Speichern unter. Wollen Sie wirklich fortfahren?</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation type="obsolete">&amp;Fortfahren</translation>
    </message>
    <message>
        <source>Invalid argument: </source>
        <translation type="obsolete">Ungültiges Argument:</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation type="obsolete">Die Datei %1 existiert nicht - Abbruch.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation type="obsolete">Benutzung: scribus [Optionen...] [Datei]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation type="obsolete">Optionen:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation type="obsolete">Hilfe anzeigen (dieser Text) und beenden</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language</source>
        <translation type="obsolete">Benutzt xx als Kürzel für eine Sprache</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation type="obsolete">Installierte Sprachen auflisten</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation type="obsolete">Zeigt Informationen auf der Konsole, wenn Schriftarten geladen werden</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation type="obsolete">Splash-Screen beim Start nicht anzeigen</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation type="obsolete">Version ausgeben und beenden</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation type="obsolete">Folgende Sprachen sind für Scribus installiert:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation type="obsolete">Um die Standardsprache außer Kraft zu setzen:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation type="obsolete">scribus -l xx oder scribus --lang xx; xx steht für den entsprechenden Ländercode.</translation>
    </message>
    <message>
        <source>Scribus Version </source>
        <translation type="obsolete">Scribus-Version</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="obsolete">Scribus, Open Source Desktop-Publishing</translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="obsolete">---------------------------------------</translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="obsolete">Homepage:      http://www.scribus.net </translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="obsolete">Dokumentation:  http://docs.scribus.net</translation>
    </message>
    <message>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="obsolete">Fehler: http://bugs.scribus.net</translation>
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
        <source>Points (pt)</source>
        <translation>Punkte (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation>Millimeter (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Zoll (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Centimeters (cm)</source>
        <translation>Zentimeter (cm)</translation>
    </message>
    <message>
        <source>File exists</source>
        <translation>Datei existiert bereits</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="obsolete">Eine Datei mit dem Namen &apos;%1&apos; existiert bereits.
Wollen Sie diese Datei mit der Datei, die Sie speichern wollen, überschreiben?</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Ersetzen</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>&amp;Color Wheel...</source>
        <translation type="obsolete">Farb&amp;rad...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">Schriftarten&amp;vorschau...</translation>
    </message>
    <message>
        <source>Document Template: </source>
        <translation>Dokumentvorlagen:</translation>
    </message>
    <message>
        <source>Failed to open document.</source>
        <comment>python error</comment>
        <translation>Fehler beim Öffnen des Dokuments.</translation>
    </message>
    <message>
        <source>Failed to save document.</source>
        <comment>python error</comment>
        <translation>Fehler beim Speichern des Dokuments.</translation>
    </message>
    <message>
        <source>Argument must be page item name, or PyCObject instance</source>
        <translation>Das Argument muss entweder ein Seiteneintrag oder eine Instanz von PyCObject sein</translation>
    </message>
    <message>
        <source>Property not found</source>
        <translation>Eigenschaft nicht gefunden</translation>
    </message>
    <message>
        <source>Child not found</source>
        <translation>Kind nicht gefunden</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert result type &apos;%1&apos;.</source>
        <translation>Kann den Ergebnistyp &apos;%1&apos; nicht konvertieren.</translation>
    </message>
    <message>
        <source>Property type &apos;%1&apos; not supported</source>
        <translation>Eigenschaft &apos;%1&apos; wird nicht unterstützt</translation>
    </message>
    <message>
        <source>Couldn&apos;t convert &apos;%1&apos; to property type &apos;%2&apos;</source>
        <translation>Konnte &apos;%1&apos; nicht nach &apos;%2&apos; konvertieren</translation>
    </message>
    <message>
        <source>Types matched, but setting property failed.</source>
        <translation>Types matched, but setting property failed.</translation>
    </message>
    <message>
        <source>Cannot group less than two items</source>
        <comment>python error</comment>
        <translation>Zum Gruppieren müssen mindestens zwei Objekte vorhanden sein</translation>
    </message>
    <message>
        <source>Can&apos;t group less than two items</source>
        <comment>python error</comment>
        <translation>Zum Gruppieren müssen mindestens zwei Objekte vorhanden sein</translation>
    </message>
    <message>
        <source>Need selection or argument list of items to group</source>
        <comment>python error</comment>
        <translation>Need selection or argument list of items to group</translation>
    </message>
    <message>
        <source>Unable to save pixmap</source>
        <comment>scripter error</comment>
        <translation>Pixmap kann nicht gespeichert werden</translation>
    </message>
    <message>
        <source>An object with the requested name already exists.</source>
        <comment>python error</comment>
        <translation>Es existiert bereits ein Objekt mit diesem Namen.</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values).</source>
        <comment>python error</comment>
        <translation>Point list must contain at least two points (four values).</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values.</source>
        <comment>python error</comment>
        <translation>Point list must contain an even number of values.</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values).</source>
        <comment>python error</comment>
        <translation>Point list must contain at least three points (six values).</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values).</source>
        <comment>python error</comment>
        <translation>Point list must contain at least four points (eight values).</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values.</source>
        <comment>python error</comment>
        <translation>Point list must have a multiple of six values.</translation>
    </message>
    <message>
        <source>Object not found.</source>
        <comment>python error</comment>
        <translation>Objekt nicht gefunden.</translation>
    </message>
    <message>
        <source>Style not found.</source>
        <comment>python error</comment>
        <translation>Stil nicht gefunden.</translation>
    </message>
    <message>
        <source>Cannot set style on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Ein Stil kann nur auf ein Textrahmen angewendet werden.</translation>
    </message>
    <message>
        <source>Failed to save EPS.</source>
        <comment>python error</comment>
        <translation>Fehler beim Speichern der EPS-Datei.</translation>
    </message>
    <message>
        <source>Page number out of range.</source>
        <comment>python error</comment>
        <translation>Seitenzahl ist außerhalb des gültigen Bereichs.</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument is not list: must be list of float values.</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument contains non-numeric values: must be list of float values.</translation>
    </message>
    <message>
        <source>argument contains no-numeric values: must be list of float values.</source>
        <comment>python error</comment>
        <translation>argument contains no-numeric values: must be list of float values.</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</source>
        <comment>python error</comment>
        <translation>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12.</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</source>
        <comment>python error</comment>
        <translation>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100.</translation>
    </message>
    <message>
        <source>Line style not found.</source>
        <comment>python error</comment>
        <translation>Linienstil nicht gefunden.</translation>
    </message>
    <message>
        <source>Only text frames can be checked for overflowing</source>
        <comment>python error</comment>
        <translation>Nur Textrahmen können auf Überfüllung geprüft werden</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation>Sc&amp;ript</translation>
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
</translation>
    </message>
    <message>
        <source>Short Words Manual</source>
        <translation type="obsolete">Handbuch für die Umbruchkontrolle</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source> c</source>
        <translation>c</translation>
    </message>
    <message>
        <source>c</source>
        <translation>c</translation>
    </message>
    <message>
        <source>Cicero (c)</source>
        <translation>Cicero (c)</translation>
    </message>
    <message>
        <source>The filename should not be empty string.</source>
        <comment>python error</comment>
        <translation>Der Dateiname darf nicht leer sein.</translation>
    </message>
    <message>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="obsolete">Wiki: http://wiki.scribus.net</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie #%1 von</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Schwarz</translation>
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
        <translation>Gelb</translation>
    </message>
    <message>
        <source>Color Wheel</source>
        <translation>Farbkreis</translation>
    </message>
    <message>
        <source>Font Preview</source>
        <translation>Schriftartenvorschau</translation>
    </message>
    <message>
        <source>My Plugin</source>
        <translation>Mein Plug-In</translation>
    </message>
    <message>
        <source>New From Template</source>
        <translation>Neu von Vorlage</translation>
    </message>
    <message>
        <source>Export As Image</source>
        <translation>Als Bild exportieren</translation>
    </message>
    <message>
        <source>PS/EPS Importer</source>
        <translation>PS/EPS-Importer</translation>
    </message>
    <message>
        <source>Save As Template</source>
        <translation>Als Vorlage speichern</translation>
    </message>
    <message>
        <source>Scripter</source>
        <translation>Scripter</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Umbruchkontrolle für Abkürzungen</translation>
    </message>
    <message>
        <source>SVG Export</source>
        <translation>SVG-Export</translation>
    </message>
    <message>
        <source>SVG Import</source>
        <translation>SVG-Import</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw Importer</source>
        <translation>OpenOffice.org Draw-Importer</translation>
    </message>
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation>Scribus ist wegen der Ausnahme %1 abgestürzt</translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation>Fontcache wird erstellt</translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation>Neuen Font gefunden, Überprüfung läuft...</translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation>Veränderten Font gefunden, Überprüfung läuft...</translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation>Fontcache wird gelesen</translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation>Aktualisierter Fontcache wird geschrieben</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Suche nach Schriftarten läuft</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation>Sie arbeiten mit einer Entwicklerversion von Scribus 1.3.x. Das Dokument, mit dem Sie arbeiten, wurde mit Scribus 1.2.3 oder niedriger erstellt. Wenn Sie jetzt speichern, können Sie die Datei nicht mehr in Scribus 1.2.3 verwenden, es sei denn, Sie verwenden die Funktion Datei &gt; Speichern unter. Wollen Sie fortfahren?</translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation>Die Änderungen am Dokument wurden nicht gesichert und Sie möchten zur gespeicherten Version zurückkehren. Wollen Sie das wirklich?</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation type="obsolete">Rechts-nach-links Anordnung der Buttons verwenden (z.B. Abbrechen/Nein/Ja statt Ja/Nein/Abbrechen)</translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation>Eine Datei mit dem Namen &apos;%1&apos; ist schon vorhanden.&lt;br/&gt;Wollen Sie diese Datei mit dem aktuellen Dokument ersetzen?</translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation>firstPageOrder is bigger than allowed.</translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation>Unterstützung für das alte .sla-Format</translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation>Deutsch (herkömmlich)</translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation>Postscript Datei erstellen</translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation>Datei drucken</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;p&gt;Sie versuchen, mehr Seiten zu importieren, als ab der aktuellen Seite im Dokument vorhanden sind.&lt;/p&gt;Was wollen Sie tun?&lt;br&gt;&lt;ul&gt;&lt;li&gt;Fehlende Seiten &lt;b&gt;erstellen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;Import bei der letzten Seite &lt;b&gt;stoppen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;&lt;b&gt;Abbrechen&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation>Er&amp;stellen</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation>Thai</translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation>Strichcode-Generator</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation>OpenOffice.org-Zeichnung (*.sxd *.odg);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation>Word-Dateien</translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation>Palm PDB-Dateien</translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation>PDB_data</translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation>PDB-Importer</translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation>Fehler beim Öffnen der Datei %1</translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation>Diese Datei wird nicht als PDB-Dokument erkannt. Falls es doch eins ist, melden Sie uns bitte einen Fehler.</translation>
    </message>
    <message>
        <source>Luxembourgish</source>
        <translation>Luxemburgisch</translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>System-Menü</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Aufrollen</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Herabrollen</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Wiederherstellen</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimieren</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximieren</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>Wieder&amp;herstellen</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>Ver&amp;schieben</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe ändern</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>M&amp;inimieren</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximieren</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Im &amp;Vordergrund halten</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimieren</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Wiederherstellen</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>&amp;Aufrollen</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Herabrollen</translation>
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
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Dokument einrichten</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Seitenränder</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Aktiviert das doppelseitige Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Macht die erste Seite des Dokuments zu einer linken Seite</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Nummer der &amp;ersten Seite:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Au&amp;srichtung:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Breite:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Höhe:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation>&amp;Einheit:</translation>
    </message>
    <message>
        <source>Layout</source>
        <translation type="obsolete">Layout</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Automatisch speichern</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
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
        <translation>Dokumentinformationen</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Hilfslinien</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farbe:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Nicht druck&amp;baren Rand in der Seitenrandlinienfarbe zeigen</translation>
    </message>
    <message>
        <source>Alt+U</source>
        <translation>Alt+U</translation>
    </message>
    <message>
        <source>Show Pictures</source>
        <translation>Bilder anzeigen</translation>
    </message>
    <message>
        <source>Show Text Chains</source>
        <translation>Verkettung von Textrahmen anzeigen</translation>
    </message>
    <message>
        <source>Show Text Control Characters</source>
        <translation>Kontrollzeichen anzeigen</translation>
    </message>
    <message>
        <source>Show Frames</source>
        <translation>Rahmen anzeigen</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Typografie</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Hyphenator</source>
        <translation>Silbentrennung</translation>
    </message>
    <message>
        <source>Fonts</source>
        <translation>Schriftarten</translation>
    </message>
    <message>
        <source>PDF Export</source>
        <translation>PDF-Export</translation>
    </message>
    <message>
        <source>Document Item Attributes</source>
        <translation>Eigenschaften des Dokumentobjekts</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Inhaltsverzeichnisse und Indizes</translation>
    </message>
    <message>
        <source>Color Management</source>
        <translation>Farbmanagement</translation>
    </message>
    <message>
        <source>Turns the of linked frames on or off</source>
        <translation type="obsolete">Schaltet die Anzeige von verketteten Textrahmen an oder ab</translation>
    </message>
    <message>
        <source>Turns the display of frames on or off</source>
        <translation>Schaltet die Anzeige von Textrahmen an oder aus</translation>
    </message>
    <message>
        <source>Turns the display of pictures on or off</source>
        <translation>Schaltet die Anzeige der Bilder an oder aus</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Hintergrundfarbe</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Zeigt den nicht druckbaren Bereich in der Farbe der Seitenrandlinien</translation>
    </message>
    <message>
        <source>Preflight Verifier</source>
        <translation>Druckvorstufenüberprüfung</translation>
    </message>
    <message>
        <source>Display non-printing characters such as paragraph markers in text frames</source>
        <translation>Nicht druckende Zeichen wie Absatzmarken in den Textrahmen anzeigen</translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="obsolete">Seitengröße auf alle Seiten anwenden</translation>
    </message>
    <message>
        <source>Apply margin settings to all Pages</source>
        <translation type="obsolete">Seitenränder für alle Seiten übernehmen</translation>
    </message>
    <message>
        <source>Rulers relative to Page</source>
        <translation>Lineal relativ zur Seite</translation>
    </message>
    <message>
        <source>Minimum Scratch Space</source>
        <translation>Minimale Ablagefläche</translation>
    </message>
    <message>
        <source>Gaps between Pages</source>
        <translation>Abstand zwischen den Seiten</translation>
    </message>
    <message>
        <source>Horizontal:</source>
        <translation>Horizontal:</translation>
    </message>
    <message>
        <source>Vertical:</source>
        <translation>Vertikal:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Optionen</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation>Aktivieren oder deaktivieren Sie die Anzeige von verketteten Textrahmen.</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Farben anpassen</translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation>Einstellung für Größe auf alle Seiten anwenden</translation>
    </message>
    <message>
        <source>Sections</source>
        <translation>Abschnitte</translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation>Wendet die Größenänderung auf alle Seiten im Dokument an</translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Scripte (*.py);; Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Run as Extension Script</source>
        <comment>run script dialog</comment>
        <translation>Als Erweiterungsscript ausführen</translation>
    </message>
</context>
<context>
    <name>SMBase</name>
    <message>
        <source>Style Manager</source>
        <translation>Stilverwaltung</translation>
    </message>
    <message>
        <source>Column 1</source>
        <translation>Spalte 1</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <source>C&amp;lone</source>
        <translation>&amp;Klonen</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+K</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>O&amp;K</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+K</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>A&amp;pply</source>
        <translation>An&amp;wenden</translation>
    </message>
    <message>
        <source>Alt+P</source>
        <translation>Alt+W</translation>
    </message>
    <message>
        <source>Ca&amp;ncel</source>
        <translation>A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+B</translation>
    </message>
</context>
<context>
    <name>SMLineStyle</name>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>Lines</source>
        <translation>Linien</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Stilvorlage für aktuellen Absatz</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Stil verändern</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Tonwert der Textfarbe</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Füllfarbe verändern</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Textumrissfarbe</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Tonwert der Textumrissfarbe</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Umrissfarbe verändern</translation>
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
        <translation>Schriftart des Objekts</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Zeichenbreiten skalieren</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Schriftart verändern</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation>Höhe der Zeichen</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Zeicheneinstellungen bearbeiten</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manueller Zeichenabstand</translation>
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
        <translation>Seite als &amp;SVG speichern...</translation>
    </message>
    <message>
        <source>Exports SVG Files</source>
        <translation>Exportiert SVG-Dateien</translation>
    </message>
    <message>
        <source>Exports the current page into an SVG file.</source>
        <translation>Exportiert die aktuelle Seite als SVG-Datei.</translation>
    </message>
</context>
<context>
    <name>SVGImportPlugin</name>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>&amp;SVG importieren...</translation>
    </message>
    <message>
        <source>Imports SVG Files</source>
        <translation>Importiere SVG-Dateien</translation>
    </message>
    <message>
        <source>Imports most SVG files into the current document,
converting their vector data into Scribus objects.</source>
        <translation>Importiert die meisten SVG-Dateien in das aktuelle Dokument
und wandelt deren Vektordaten in Scribus-Objekte um.
</translation>
    </message>
    <message>
        <source>Scalable Vector Graphics</source>
        <translation>Scalable Vector Graphics</translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation>Die SVG-Datei enthält einige nicht unterstützte Features</translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Umbruchkontrolle für Abkürzungen</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Umbruchkontrolle anwenden auf:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Markierte Textrahmen</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Aktive &amp;Seite</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>&amp;Alle Objekte</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Die markierten Rahmen wurden bearbeitet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Aktuelle Seite wurde bearbeitet.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Alle Objekte wurden bearbeitet.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation>Benutzerdefinierte Einstellungen</translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation>Globale Konfiguration</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation>&amp;Zurücksetzen</translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation>Benutzerdefinierte Konfiguration speichern</translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation>Globale Konfiguration laden und benutzerdefinierte Einstellungen löschen</translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation>Benutzerkonfiguration bearbeiten. Wenn Sie speichern, wird sie anstelle der globalen Konfiguration benutzt</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Umbruchkontrolle für Abkürzungen</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation>Es sind schon benutzerdefinierte Einstellungen vorhanden. Wollen Sie diese wirklich überschreiben?</translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation>Fehler beim Schreiben der Datei %1.</translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation>Benutzerdefinierte Einstellungen gespeichert</translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation>Globale Konfiguration wiederhergestellt</translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation>Fehler beim Öffnen der Datei %1</translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>&amp;Als Vorlage speichern...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation>Dokument als Vorlage abspeichern</translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
        <translation>Dokument als Vorlage abspeichern. So können Sie einmal erstellte Layouts immer wieder verwenden</translation>
    </message>
</context>
<context>
    <name>ScActionPlugin</name>
    <message>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">Zieldatei %1 nicht gefunden %2</translation>
    </message>
    <message>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="obsolete">Konnte Zieldatei %1 nicht öffnen: %2</translation>
    </message>
</context>
<context>
    <name>ScPlugin</name>
    <message>
        <source>Persistent</source>
        <comment>plugin manager plugin type</comment>
        <translation>Fest</translation>
    </message>
    <message>
        <source>Import</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Import</translation>
    </message>
    <message>
        <source>Export</source>
        <comment>plugin manager plugin type</comment>
        <translation type="obsolete">Export</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation>Aktion</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation>Öffnen/Speichern/Import/Export</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Unbekannt</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation>Oben</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation>Unten</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation>Andocken erlauben...</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation>Richtung...</translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation>Drucken...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Kopie #%1 von</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation type="obsolete">Datei</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Suche nach Schriftarten</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="obsolete">Schwerwiegender Fehler</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Voreinstellungen werden geladen</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">Tastaturkürzel werden gelesen</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Bibliothek wird geladen</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Plug-Ins werden geladen</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="obsolete">Über Qt</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">X-Pos:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Y-Pos:</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="obsolete">Fertig</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="obsolete">Größe</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="obsolete">Tonwert</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Lade...</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Textdateien (*.txt);;Alle Dateien(*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Speichern unter</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Speichere...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Drucke...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">Drucken fehlgeschlagen!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Scribus-Handbuch</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Alle</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Passe Farben an</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Englisch</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Deutsch</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Spanisch</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Italienisch</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Französisch</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Russisch</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Dänisch</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Slowakisch</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">Ungarisch</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Tschechisch</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Niederländisch</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugiesisch</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ukrainisch</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Polnisch</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Griechisch</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Katalanisch</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Finnisch</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Alle unterstützten Formate</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Irisch</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">&amp;Objekt</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Seite</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Ansicht</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="obsolete">&amp;Werkzeuge</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">&amp;Fenster</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Hilfe</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Einige Objekte sind gesperrt.</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Litauisch</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Schwedisch</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Slovenisch</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">&amp;Zuletzt verwendete öffnen</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importieren</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="obsolete">E&amp;xportieren</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Form</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">S&amp;til</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Hintereinander</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">&amp;Nebeneinander</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Farben</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="obsolete">Schrift&amp;art</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Größe</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Effekte</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">Aus&amp;richtung</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">Ton&amp;wert</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Seiten werden importiert...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Seiten importieren</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="obsolete">&lt;p&gt;Sie versuchen, mehr Seiten zu importieren als im aktiven Dokument vorhanden sind.&lt;/p&gt;Wählen Sie bitte aus:&lt;br&gt;&lt;ul&gt;&lt;li&gt;fehlende Seiten &lt;b&gt;erstellen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;Bis zur letzten vorhandenen Seite &lt;b&gt;einfügen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;&lt;b&gt;Abbrechen&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Erstellen</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importieren</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Importvorgang erfolgreich abgeschlossen</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">Nichts zum Importieren gefunden</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Größe:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">Ton&amp;wert:</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikaans</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Fontsystem initialisiert...</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="obsolete">Portugiesisch (BR)</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="obsolete">Kroatisch</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation type="obsolete">Story Editor wird initialisiert</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation type="obsolete">ICC-Profile werden gelesen</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation type="obsolete">Silbentrennung wird initialisiert</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Bearbeiten</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation type="obsolete">Auf Ihrem System sind keine Schriftarten vorhanden.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation type="obsolete">Scribus wird nun beendet.</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
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
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;PDF-Optionen</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation type="obsolete">Um&amp;wandeln in</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation type="obsolete">&amp;Einfügen</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Zeichen</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="obsolete">Anführungszeichen</translation>
    </message>
    <message>
        <source>Space</source>
        <translation type="obsolete">Leerzeichen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="obsolete">Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="obsolete">Dokumente (*.sla *.scd);;</translation>
    </message>
    <message>
        <source>Postscript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="obsolete">Postscript-Dateien (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="obsolete">SVG-Bilder (*.svg *.svgz);;</translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="obsolete">SVG-Bilder (*.svg);;</translation>
    </message>
    <message>
        <source>File %1 
is not in an acceptable format</source>
        <translation type="obsolete">Scribus kann das Format der Datei %1 nicht lesen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="obsolete">Einige Schriftarten in diesem Dokument wurden ersetzt:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="obsolete"> wurde ersetzt durch: </translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="obsolete">Einige ICC-Profile in dem Dokument sind auf Ihrem System nicht vorhanden:</translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="obsolete">(konvertiert)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="obsolete">Die Datei %1 kann nicht geschrieben werden</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Speichern unter</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Checker to correct them</source>
        <translation type="obsolete">Es wurden einige Fehler festgestellt.
Benutzen Sie bitte den Druckvorstufen-Check, 
um die Fehler zu beheben</translation>
    </message>
    <message>
        <source>Abort</source>
        <translation type="obsolete">Abbrechen</translation>
    </message>
    <message>
        <source>Ignore</source>
        <translation type="obsolete">Ignorieren</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation type="obsolete">Objekte in Benutzung können nicht ausgeschnitten werden</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation type="obsolete">Das Objekt %1 wird gerade im Story Editor bearbeitet, deswegen kann es nicht ausgeschnitten werden</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation type="obsolete">Die folgenden Programme fehlen:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="obsolete">Ghostscript: Sie können weder EPS-Dateien noch die Druckvorschau verwenden</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS-Dateien (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Detected some Errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="obsolete">Es sind Fehler aufgetreten.
Benutzen Sie den Druckvorstufen-Check, um die Fehler zu beheben</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation type="obsolete">Alle &amp;sperren</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="obsolete">Alle &amp;entsperren</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="obsolete">Bulgarisch</translation>
    </message>
    <message>
        <source>The program</source>
        <translation type="obsolete">Das Programm</translation>
    </message>
    <message>
        <source>is already running!</source>
        <translation type="obsolete">läuft bereits!</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Information</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">fehlt!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation type="obsolete">Die gewählte Farbe existiert im Farbset des Dokuments noch nicht. Bitte geben Sie einen Namen für die neue Farbe ein.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation type="obsolete">Farbe nicht gefunden</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation type="obsolete">Eine Farbe mit diesem Namen existiert schon. Bitte geben Sie einen anderen Namen ein.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation type="obsolete">&amp;Anordnung</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation type="obsolete">Auf andere Ebene verschieben</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
        <translation type="obsolete">OpenOffice.org Zeichnung (*.sxd);;</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation type="obsolete">&amp;Vorschau</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="obsolete">Tastenkürzel einlesen</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="obsolete">Vorschau</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Anordnung</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Auf andere &amp;Ebene verschieben</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="obsolete">Das Format der Datei %1 kann nicht verwendet werden</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="obsolete">PostScript</translation>
    </message>
    <message>
        <source>Scribus has detected some errors.
Consider using the Pre-flight Checker to correct them</source>
        <translation type="obsolete">Scribus hat Fehler entdeckt.
Bitte verwenden Sie die Druckvorstufenüberprüfung, um
diese zu finden und zu korrigieren</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="obsolete">A&amp;bbruch</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="obsolete">&amp;Ignorieren</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Name:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="obsolete">Neue Musterseite</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="obsolete">Neue Musterseite</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="obsolete">Scribus hat Fehler entdeckt.
Bitte verwenden Sie die Druckvorstufenüberprüfung, um
diese zu finden und zu korrigieren.</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation type="obsolete">Scribus hat Fehler entdeckt.
Bitte verwenden Sie die Druckvorstufenüberprüfung, um
diese zu finden und zu korrigieren</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation type="obsolete">-Seite %1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation type="obsolete">Einige Objekte sind gesperrt.</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation type="obsolete">Das Programm %1 läuft schon!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="obsolete">Das Programm %1 fehlt!</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="obsolete">In Musterseite umwandeln</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="obsolete">Scribus hat einige Fehler festgestellt. Sie sollten die Druckvorstufenüberprüfung verwenden, um sie zu korrigieren</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="obsolete">Kein Stil</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="obsolete">Vorschau</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Farbe</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Document Colors</source>
        <translation>Dokumentfarben</translation>
    </message>
</context>
<context>
    <name>ScribusDoc</name>
    <message>
        <source>New Layer</source>
        <translation>Neue Ebene</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Hintergrund</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Wollen Sie wirklich den gesamten Text löschen?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation>Gerade verwendete Objekte können nicht gelöscht werden</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation>Das Objekt %1 wird gerade im Story Editor bearbeitet und kann deswegen nicht gelöscht werden</translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Plug-Ins werden geladen</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation>Tastenkürzel einlesen</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Einstellungen werden geladen</translation>
    </message>
    <message>
        <source>Initializing Story Editor</source>
        <translation>Story Editor wird initialisiert</translation>
    </message>
    <message>
        <source>Reading ICC Profiles</source>
        <translation>ICC-Profile werden gelesen</translation>
    </message>
    <message>
        <source>Initializing Hyphenator</source>
        <translation>Silbentrennung wird initialisiert</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Bibliothek wird geladen</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Tastaturkürzel werden gelesen</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Schriften werden gesucht</translation>
    </message>
    <message>
        <source>There are no fonts found on your system.</source>
        <translation>Auf Ihrem System sind keine Schriftarten vorhanden.</translation>
    </message>
    <message>
        <source>Exiting now.</source>
        <translation>Scribus wird nun beendet.</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Fataler Fehler</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Fontsystem initialisiert</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>&amp;Zuletzt verwendete öffnen</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>E&amp;xportieren</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>S&amp;til</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Ton&amp;wert</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>Schrift&amp;art</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekte</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Einstellungen für die Druckvorschau</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Anordnung</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Auf andere &amp;Ebene verschieben</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-Optionen</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
    </message>
    <message>
        <source>C&amp;onvert To</source>
        <translation>Um&amp;wandeln in</translation>
    </message>
    <message>
        <source>I&amp;nsert</source>
        <translation>&amp;Einfügen</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Anführungszeichen</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Leerzeichen</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Seite</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Ansicht</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Fenster</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Hilfe</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>Aus&amp;richtung</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Fertig</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Seiten werden importiert...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Seiten importieren</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importvorgang erfolgreich abgeschlossen</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nichts zum Importieren gefunden</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation>Das Format der Datei %1 kann nicht verwendet werden</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Lade...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation>Einige ICC-Profile in dem Dokument sind auf Ihrem System nicht vorhanden:</translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation> wurde ersetzt durch: </translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation>(konvertiert)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Alle unterstützten Formate</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation>Die Datei %1 kann nicht geschrieben werden</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Speichere...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus hat einige Fehler festgestellt. Sie sollten die Druckvorstufenüberprüfung verwenden, um sie zu korrigieren</translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation>&amp;Ignorieren</translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation>A&amp;bbruch</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Drucke...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Dokument</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Drucken fehlgeschlagen!</translation>
    </message>
    <message>
        <source>Cannot Cut In-Use Item</source>
        <translation>Objekte in Benutzung können nicht ausgeschnitten werden</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The cut operation will be cancelled</source>
        <translation>Das Objekt %1 wird gerade im Story Editor bearbeitet, deswegen kann es nicht ausgeschnitten werden</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Scribus-Handbuch</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textdateien (*.txt);;Alle Dateien(*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation>In Musterseite umwandeln</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="obsolete">Neue Musterseite</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Ton&amp;wert:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
    <message>
        <source>The following programs are missing:</source>
        <translation>Die folgenden Programme fehlen:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation>Ghostscript: Sie können weder EPS-Dateien noch die Druckvorschau verwenden</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation>Scribus hat Fehler entdeckt.
Bitte verwenden Sie die Druckvorstufenüberprüfung, um
diese zu finden und zu korrigieren.</translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
        <translation>EPS-Dateien (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Detected some errors.
Consider using the Preflight Verifier to correct them</source>
        <translation>Scribus hat Fehler entdeckt.
Bitte verwenden Sie die Druckvorstufenüberprüfung, um
diese zu finden und zu korrigieren</translation>
    </message>
    <message>
        <source>-Page%1</source>
        <translation>-Seite %1</translation>
    </message>
    <message>
        <source>Some objects are locked.</source>
        <translation>Einige Objekte sind gesperrt.</translation>
    </message>
    <message>
        <source>&amp;Lock All</source>
        <translation>Alle &amp;sperren</translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation>Alle &amp;entsperren</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <source>The program %1 is already running!</source>
        <translation>Das Programm %1 läuft schon!</translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation>Das Programm %1 fehlt!</translation>
    </message>
    <message>
        <source>The selected color does not exist in the document&apos;s color set. Please enter a name for this new color.</source>
        <translation>Die gewählte Farbe existiert im Farbsatz des Dokuments noch nicht. Bitte geben Sie einen Namen für die neue Farbe ein.</translation>
    </message>
    <message>
        <source>Color Not Found</source>
        <translation>Farbe nicht gefunden</translation>
    </message>
    <message>
        <source>The name you have selected already exists. Please enter a different name for this new color.</source>
        <translation>Eine Farbe mit diesem Namen existiert schon. Bitte geben Sie einen anderen Namen ein.</translation>
    </message>
    <message>
        <source>&amp;Level</source>
        <translation>&amp;Anordnung</translation>
    </message>
    <message>
        <source>Send to Layer</source>
        <translation>Auf andere Ebene verschieben</translation>
    </message>
    <message>
        <source>Previe&amp;w Settings</source>
        <translation>&amp;Vorschau</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Werkzeuge</translation>
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
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Leerzeichen &amp;&amp; Umbrüche</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaturen</translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation>Neue Musterseite %1</translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation>Anzahl der Kopien: %1
Horizontaler Versatz: %2
Vertikaler Versatz: %3</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation>Ghostscript: Sie können weder EPS-Dateien noch die Druckvorschau verwenden</translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation>Ghostscript fehlt: Die Postscript-Druckvorschau wird nicht verfügbar sein</translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation>Wollen Sie das vorhandene Bild wirklich ersetzen?</translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
    <message>
        <source>Invalid argument: </source>
        <translation>Ungültiges Argument:</translation>
    </message>
    <message>
        <source>File %1 does not exist, aborting.</source>
        <translation>Die Datei %1 existiert nicht - Abbruch.</translation>
    </message>
    <message>
        <source>Usage: scribus [option ... ] [file]</source>
        <translation>Verwendung: scribus [Optionen...] [Datei]</translation>
    </message>
    <message>
        <source>Options:</source>
        <translation>Optionen:</translation>
    </message>
    <message>
        <source>Print help (this message) and exit</source>
        <translation>Hilfe anzeigen (dieser Text) und beenden</translation>
    </message>
    <message>
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
        <translation>Benutzt xx als Kurzversion für eine Sprache, z.B. &apos;en&apos; oder &apos;de&apos;</translation>
    </message>
    <message>
        <source>List the currently installed interface languages</source>
        <translation>Verfügbare Sprachen auflisten</translation>
    </message>
    <message>
        <source>Show information on the console when fonts are being loaded</source>
        <translation>Zeigt Informationen auf der Konsole, wenn Schriftarten geladen werden</translation>
    </message>
    <message>
        <source>Do not show the splashscreen on startup</source>
        <translation>Splashscreen beim Start nicht anzeigen</translation>
    </message>
    <message>
        <source>Output version information and exit</source>
        <translation>Version ausgeben und beenden</translation>
    </message>
    <message>
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation>Buttons rechtsläufig anordnen (z.B. Abbrechen/Nein/Ja statt Ja/Nein/Abbrechen)</translation>
    </message>
    <message>
        <source>filename</source>
        <translation>Dateiname</translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
        <translation>Verwendet &apos;Dateiname&apos; als Pfad für die Konfigurationsdatei</translation>
    </message>
    <message>
        <source>Installed interface languages for Scribus are as follows:</source>
        <translation>Folgende Sprachen sind für Scribus installiert:</translation>
    </message>
    <message>
        <source>To override the default language choice:</source>
        <translation>Um die Standardsprache außer Kraft zu setzen:</translation>
    </message>
    <message>
        <source>scribus -l xx or scribus --lang xx, where xx is the language of choice.</source>
        <translation>scribus -l xx oder scribus --lang xx; xx steht für den entsprechenden Ländercode.</translation>
    </message>
    <message>
        <source>Scribus Version</source>
        <translation>Scribus-Version</translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation>Scribus, Open Source Desktop-Publishing</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Homepage</translation>
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
        <translation>Bekannte Probleme</translation>
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
        <translation>Ebene</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>An diese Stelle kopieren</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>An diese Stelle verschieben</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Datei:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Ursprüngliche PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Aktuelle PPI:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Verketteter Text</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Textrahmen</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text auf einem Pfad</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Absätze:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Wörter:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Zeichen:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Drucken:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>ja</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>nein</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Information</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation>Einstellungen für die Druckvorschau</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-Optionen</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Auf andere &amp;Ebene verschieben</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>An&amp;ordnung</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Umwandeln in</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Wollen Sie wirklich den gesamten Text löschen?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation type="obsolete">Gerade verwendete Objekte können nicht gelöscht werden</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="obsolete">Das Objekt %1 wird gerade im Story Editor bearbeitet und kann deswegen nicht gelöscht werden</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Kein</translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="obsolete">Einheit</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation>Textrahmen verketten</translation>
    </message>
    <message>
        <source>There is a problem with text frames linking. You are trying to link filled frames or a frame to the same one itself</source>
        <translation type="obsolete">Beim Verketten der Textrahmen ist ein Fehler aufgetreten. Sie versuchen, volle Textrahmen oder den gleichen Rahmen mit sich selbst zu verbinden</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation>Sie versuchen einen Rahmen mit sich selbst zu verketten.</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="obsolete">Wollen Sie wirklich den gesamten Text löschen?</translation>
    </message>
    <message>
        <source>Cannot Convert In-Use Item</source>
        <translation>Ein in Bearbeitung befindliches Objekt kann nicht
umgewandelt werden</translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The convert to outlines operation for this item will be skipped</source>
        <translation>Das Objekt %1 wird gerade im Story Editor bearbeitet und kann deswegen nicht umgewandelt werden</translation>
    </message>
    <message>
        <source>Page %1 to %2</source>
        <translation>Seite %1 bis %2</translation>
    </message>
    <message>
        <source>Colorspace: </source>
        <translation>Farbraum:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Unbekannt</translation>
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
        <translation>Graustufen</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Datei:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>wurde seit dem letzten Speichern verändert.</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">&amp;Ignorieren</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">Trotzdem sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Jetzt speichern</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation>&amp;Verwerfen</translation>
    </message>
</context>
<context>
    <name>ScriptPlugin</name>
    <message>
        <source>Embedded Python scripting support.</source>
        <translation>Unterstützung für eingebettete Python-Scripts.</translation>
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
        <translation>Fehler bei der Ausführung eines Scripts</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Falls das Script ein offizelles Scribus-Script ist, melden Sie den Fehler bitte auf &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Diese Meldung befindet sich auch in Ihrer Zwischenablage. Benutzen Sie Strg+V, um sie in den Bugtracker zu kopieren.</translation>
    </message>
    <message>
        <source>Examine Script</source>
        <translation>Script untersuchen</translation>
    </message>
    <message>
        <source>Python Scripts (*.py)</source>
        <translation type="obsolete">Python-Scripte (*.py)</translation>
    </message>
    <message>
        <source>There was an internal error while trying the command you entered. Details were printed to stderr. </source>
        <translation>Ihr Befehl konnte auf Grund eines internen Fehlers nicht ausgeführt werden. Weitere Hinweise finden Sie auf stderr.</translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation>Konnte das Python-PlugIn nicht einrichten. Weitere Hinweise finden Sie auf stderr.</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation>Pythonscripts (*.py);; Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation>Handbuch für:</translation>
    </message>
    <message>
        <source>Script</source>
        <translation>Script</translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation>enthält keinen docstring!</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation>Einstellungen für den Scripter</translation>
    </message>
    <message>
        <source>Extension Scripts</source>
        <translation type="obsolete">Erweiterungsscripte</translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation>Erweiterungsscripte aktivieren</translation>
    </message>
    <message>
        <source>Startup Script</source>
        <translation type="obsolete">Autostart-Script</translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation>Erweiterungen</translation>
    </message>
    <message>
        <source>Console</source>
        <translation>Konsole</translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation>Autostart-Script:</translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation>Fehler:</translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation>Kommentare:</translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation>Stichworte:</translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation>Zeichen:</translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation>Zahlen:</translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation>Strings:</translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation>Base Texts:</translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation>Farbe wählen</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Ändern...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation>Autostartscript auswählen</translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Seitenvorschau anzeigen</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Master Page Names</source>
        <translation type="obsolete">Namen der Musterseiten anzeigen</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Suchen &amp;&amp; Ersetzen</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Suchen nach:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Absatzstil</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Schriftart</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Schriftgröße</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Füllfarbe</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Randfarbe</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Keine</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Ersetzen durch:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Suche beendet</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Zentriert</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Blocksatz</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Erzw. Blocksatz</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Ganzes Wort</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>Groß-/&amp;Kleinschreibung ignorieren</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Suchen</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Ersetzen</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>&amp;Alles ersetzen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation>Suche beendet, %1 Übereinstimmungen gefunden</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Seiten anordnen</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Seiten des Dokuments:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Doppelseiten</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="obsolete">Ziehen Sie einzelne Seiten oder Musterseiten auf den Papierkorb, um sie zu löschen</translation>
    </message>
    <message>
        <source>Previews all the pages of your document</source>
        <translation type="obsolete">Anzeige aller vorhandenen Seiten im Dokument</translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="obsolete">Dies sind alle vorhandenen Musterseiten. Ziehen Sie eine Musterseite in das Fenster mit den Seitensymbolen, um eine neue Seite zu erstellen</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="obsolete">Verfügbare Musterseiten:</translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="obsolete">Linke Seite zuerst</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Felder auswählen</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Verfügbare Felder</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Ausgewählte Felder</translation>
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
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Andere...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Ton&amp;wert:</translation>
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
        <translation>X-Offset</translation>
    </message>
    <message>
        <source>Y-Offset</source>
        <translation>Y-Offset</translation>
    </message>
</context>
<context>
    <name>ShortWordsPlugin</name>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation>Umbruc&amp;hkontrolle für Abkürzungen...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation>Umbruchkontrolle</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words.</source>
        <translation type="obsolete">Ein spezielles Plug-In, um ein geschütztes Leerzeichen vor oder hinter Abkürzungen einzufügen.</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
    <message>
        <source>Edit Styles...</source>
        <translation>Stile bearbeiten...</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Stilvorlagen bearbeiten</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Neue Stilvorlage</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Nein</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Ja</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Dokumente (*.sla *.scd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">An&amp;fügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplizieren</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Speichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Wollen Sie diesen Absatzstil wirklich löschen?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="obsolete">Möchten Sie diesen Stil wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Textbearbeitung</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Aktueller Absatz:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Wörter:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Zeichen:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Gesamt:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Absätze:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Warnung</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textdateien (*.txt);;Alle Dateien(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="obsolete">Textrahmen auffrischen</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Wollen Sie die Änderungen speichern?</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Änderungen übernehmen</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Änderungen ignorieren</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Text neu vom Textrahmen</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>In Datei &amp;speichern...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>Von Datei &amp;laden...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>&amp;Dokument speichern</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>Änderungen &amp;übernehmen</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Änderungen &amp;ignorieren</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Ausschneiden</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Kopieren</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Textrahmen auffrischen</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Alles aus&amp;wählen</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="obsolete">Text von Datei laden</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="obsolete">Text in Datei speichern</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Text aus Textrahmen neu laden</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Suchen &amp;&amp; Ersetzen...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>Stil&amp;vorlagen bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Schriftarten&amp;vorschau...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Hintergrund...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Schriftart...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="obsolete">Suchen und Ersetzen</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Intelligente Textmarkierung</translation>
    </message>
    <message>
        <source>&amp;Insert Glyph...</source>
        <translation>&amp;Zeichen einfügen...</translation>
    </message>
    <message>
        <source>Clear All Text</source>
        <translation>Text löschen</translation>
    </message>
    <message>
        <source>Story Editor - %1</source>
        <translation>Story Editor - %1</translation>
    </message>
    <message>
        <source>Do you really want to lose all your changes?</source>
        <translation>Alle Änderungen verwerfen?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation>Wollen Sie wirklich den gesamten Text löschen?</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>Ein&amp;fügen</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Zeichen</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation>Anführungszeichen</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation>Leerzeichen &amp;&amp; Umbrüche</translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation>Ligaturen</translation>
    </message>
    <message>
        <source>Space</source>
        <translation>Leerzeichen</translation>
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
        <translation>Versatz</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linienstärke</translation>
    </message>
</context>
<context>
    <name>StyleManager</name>
    <message>
        <source>More than one item selected</source>
        <translation>Es ist mehr als ein Objekt markiert</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitälchen</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Durchgestrichen</translation>
    </message>
    <message>
        <source>Underline Words Only</source>
        <translation>Nur Wörter unterstreichen</translation>
    </message>
    <message>
        <source>All Caps</source>
        <translation>Großbuchstaben</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Umriss</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation>Schatten</translation>
    </message>
    <message>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation>Umriss</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Name der Datei vor jeden Absatzstil anfügen</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Einstellungen merken</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Optionen für den OpenOffice.org Writer-Import</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Damit werden bereits vorhandene Stile im aktuellen Dokument überschrieben</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Absatzstile zusammenführen</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Absatzstile nach Attributen zusammenführen. Diese Option führt zu weniger Absatzstilen, selbst wenn im Originaldokument Stile anders benannt sind.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Name des Dokuments an Absatzstile anhängen.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Diese Einstellungen zum Standard machen und beim Import von Dateien, die mit OpenOffice 1.x erstellt wurden nicht erneut nachfragen.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation>Absatzstile überschreiben</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>At the beginning</source>
        <translation>Am Anfang</translation>
    </message>
    <message>
        <source>At the end</source>
        <translation>Am Ende</translation>
    </message>
    <message>
        <source>Not Shown</source>
        <translation>Nicht sichtbar</translation>
    </message>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Inhaltsverzeichnisse und Indizes</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Inhaltsverzeichnis</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Der Rahmen, in den das Inhaltsverzeichnis eingefügt werden soll</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Platzierung der Seitenzahlen:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Bezeichnung des Dokumentobjekts:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Das Dokumentobjekt, welches auf Rahmen als Grundlage für die Erstellung von Einträgen angewendet wird</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Platzierung von Seitennummern am Anfang oder am Ende einer Zeile, beziehungsweise überhaupt nicht</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Nicht druckende Einträge anzeigen</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Auch Rahmen einbeziehen, die nicht gedruckt werden</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Der Abstatzstil, der für die Einträge verwendet wird</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Absatzstil:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Zielrahmen:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefsBase</name>
    <message>
        <source>Table of Contents and Indexes</source>
        <translation>Inhaltsverzeichnis und Index</translation>
    </message>
    <message>
        <source>Table Of Contents</source>
        <translation>Inhaltsverzeichnis</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Hinzufügen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+H</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>The frame the table of contents will be placed into</source>
        <translation>Der Rahmen, in den das Inhaltsverzeichnis eingefügt werden soll</translation>
    </message>
    <message>
        <source>Page Numbers Placed:</source>
        <translation>Platzierung der Seitenzahlen:</translation>
    </message>
    <message>
        <source>Item Attribute Name:</source>
        <translation>Bezeichnung des Dokumentobjekts:</translation>
    </message>
    <message>
        <source>The Item Attribute that will be set on frames used as a basis for creation of the entries</source>
        <translation>Das Dokumentobjekt, welches auf Rahmen als Grundlage für die Erstellung von Einträgen angewendet wird</translation>
    </message>
    <message>
        <source>Place page numbers of the entries at the beginning or the end of the line, or not at all</source>
        <translation>Platzierung von Seitennummern am Anfang oder am Ende einer Zeile, beziehungsweise überhaupt nicht</translation>
    </message>
    <message>
        <source>List Non-Printing Entries</source>
        <translation>Nicht druckende Einträge anzeigen</translation>
    </message>
    <message>
        <source>Include frames that are set to not print as well</source>
        <translation>Auch Rahmen einbeziehen, die nicht gedruckt werden</translation>
    </message>
    <message>
        <source>The paragraph style used for the entry lines</source>
        <translation>Der Abstatzstil, der für die Einträge verwendet wird</translation>
    </message>
    <message>
        <source>Paragraph Style:</source>
        <translation>Absatzstil:</translation>
    </message>
    <message>
        <source>Destination Frame:</source>
        <translation>Zielrahmen:</translation>
    </message>
    <message>
        <source>Inde&amp;x</source>
        <translation>Inde&amp;x</translation>
    </message>
</context>
<context>
    <name>TabCheckDoc</name>
    <message>
        <source>Ignore all errors</source>
        <translation>Fehler ignorieren</translation>
    </message>
    <message>
        <source>Automatic check before printing or exporting</source>
        <translation>Dokument automatisch vor dem Druck oder Export überprüfen</translation>
    </message>
    <message>
        <source>Check for missing glyphs</source>
        <translation>Auf fehlende Schriftzeichen überprüfen</translation>
    </message>
    <message>
        <source>Check for objects not on a page</source>
        <translation>Auf Objekte überprüfen, die außerhalb des Dokuments platziert sind</translation>
    </message>
    <message>
        <source>Check for overflow in text frames</source>
        <translation>Auf Überfüllung der Textrahmen prüfen</translation>
    </message>
    <message>
        <source>Check for transparencies used</source>
        <translation>Auf Probleme mit Transparenzen überprüfen</translation>
    </message>
    <message>
        <source>Check for missing images</source>
        <translation>Auf fehlende Bilder überprüfen</translation>
    </message>
    <message>
        <source>Check image resolution</source>
        <translation>Auflösung der Bilder überprüfen</translation>
    </message>
    <message>
        <source>Lowest allowed resolution</source>
        <translation>Minimal erlaubte Auflösung der Bilder</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Check for placed PDF Files</source>
        <translation>Auf importierte PDF-Dateien überprüfen</translation>
    </message>
    <message>
        <source>Check for PDF Annotations and Fields</source>
        <translation>Auf PDF-Anmerkungen und PDF-Felder überprüfen</translation>
    </message>
    <message>
        <source>Add Profile</source>
        <translation>Profil hinzufügen</translation>
    </message>
    <message>
        <source>Remove Profile</source>
        <translation>Profil entfernen</translation>
    </message>
</context>
<context>
    <name>TabGuides</name>
    <message>
        <source>Common Settings</source>
        <translation>Gängige Einstellungen</translation>
    </message>
    <message>
        <source>Placing in Documents</source>
        <translation>Platzierung im Dokument</translation>
    </message>
    <message>
        <source>In the Background</source>
        <translation>Im Hintergrund</translation>
    </message>
    <message>
        <source>In the Foreground</source>
        <translation>Im Vordergrund</translation>
    </message>
    <message>
        <source>Snapping</source>
        <translation>Einrasten</translation>
    </message>
    <message>
        <source>Snap Distance:</source>
        <translation>Distanz zum Einrasten:</translation>
    </message>
    <message>
        <source>Grab Radius:</source>
        <translation>Fangradius:</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation>Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Farbe:</translation>
    </message>
    <message>
        <source>Show Margins</source>
        <translation>Seitenrandlinien zeigen</translation>
    </message>
    <message>
        <source>Show Page Grid</source>
        <translation>Seitenraster zeigen</translation>
    </message>
    <message>
        <source>Major Grid</source>
        <translation>Großes Raster</translation>
    </message>
    <message>
        <source>Spacing:</source>
        <translation>Abstand:</translation>
    </message>
    <message>
        <source>Minor Grid</source>
        <translation>Kleines Raster</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Grundlinienraster anzeigen</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation>Einstellungen für Grundlinien</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Grundlinen&amp;raster:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Grundlinien&amp;versatz:</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Abstand zwischen den Teillinien</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Abstand zwischen den Hauptlinien</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Abstand, ab dem ein Objekt an den Hilfslinien einrastet</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radius des Bereichs, in dem es möglich ist, die Objektbegrenzungen zu aktivieren</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Farbe der Teillinien</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Farbe der Hauptlinien</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Farbe der Hilfslinien</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farbe für die Seitenränder</translation>
    </message>
    <message>
        <source>Color for the baseline grid</source>
        <translation>Farbe des Grundlinienrasters</translation>
    </message>
    <message>
        <source>Turns the basegrid on or off</source>
        <translation>Schaltet das Grundraster ein oder aus
</translation>
    </message>
    <message>
        <source>Turns the gridlines on or off</source>
        <translation>Schaltet das Raster ein oder aus</translation>
    </message>
    <message>
        <source>Turns the guides on or off</source>
        <translation>Schaltet die Hilfslinien ein oder aus</translation>
    </message>
    <message>
        <source>Turns the margins on or off</source>
        <translation>Schaltet die Ränder an oder aus</translation>
    </message>
    <message>
        <source>Guides are not visible through objects on the page</source>
        <translation>Die Hilfslinien sind hinter den Objekten der Seite verborgen</translation>
    </message>
    <message>
        <source>Guides are visible above all objects on the page</source>
        <translation>Die Hilfslinien über allen Objekten der Seite sichtbar</translation>
    </message>
    <message>
        <source>Distance between the lines of the baseline grid</source>
        <translation>Abstand der Grundlinien</translation>
    </message>
    <message>
        <source>Distance from the top of the page for the first baseline</source>
        <translation>Abstand vom oberen Seitenrand bis zur ersten Grundlinie</translation>
    </message>
</context>
<context>
    <name>TabManager</name>
    <message>
        <source>Manage Tabulators</source>
        <translation>Tabulatoren bearbeiten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation>Bereich festlegen</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Alle Seiten</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Seiten &amp;wählen</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotation:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Kompatibilität:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>Bindun&amp;g:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Linker Rand</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Rechter Rand</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>&amp;Vorschaubilder erzeugen</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Verkettete Textrahmen als &amp;PDF-Artikel speichern</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>Lesezeichen &amp;integrieren</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="obsolete">Ebenen exportieren</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>&amp;Resolution for EPS Graphics:</source>
        <translation>Auflösung für &amp;EPS-Dateien:</translation>
    </message>
    <message>
        <source>Com&amp;press Text and Vector Graphics</source>
        <translation>&amp;Text und Vektorgrafik komprimieren</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Optionen für Bilder</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisch</translation>
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
        <translation>Keine</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Methode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Qualität:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Maximal</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Hoch</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mittel</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Niedrig</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Minimal</translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="obsolete">Auflösung ändern auf:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Allgemein</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Alle Schriftarten vollständig ein&amp;betten</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Von allen Schriftarten nur &amp;benutzte Schriftzeichen einbetten</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Einbetten</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Verfügbare Schriftarten:</translation>
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
        <translation>Schriftarten vollständig einbetten:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Nur benutzte Schriftzeichen einbetten:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>Schriftar&amp;ten</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation>&amp;Präsentationseffekte aktivieren</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Seiten&amp;vorschau anzeigen</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Anzeigedauer:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Dauer des &amp;Effekts:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Effekt&amp;typ:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Bewegte Linien:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;von:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>&amp;Richtung:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sek</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Kein Effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Jalousie</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Einblenden</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Auflösen</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Schachbrett</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Teilen</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Wischen</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Innen</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Außen</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Von links nach rechts</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Von oben nach unten</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Von unten nach oben</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Von rechts nach links</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Schräg von links oben</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Effekt auf alle Seiten an&amp;wenden</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Verschlüsselung benutzen</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Passwörter</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Benutzer:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>&amp;Besitzer:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Berechtigungen</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Dokument &amp;drucken erlauben</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Dokument än&amp;dern erlauben</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Kopieren von &amp;Text und Bildern zulassen</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Hinzufügen von &amp;Anmerkungen erlauben</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>Sicher&amp;heit</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allgemein</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Ausgabe &amp;vorgesehen für:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Monitor / Internet</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Drucker</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation>Graustufen</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Benutzerdefinierte Ausgabeeinstellungen verwenden</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Ausgabeeinstellungen</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>&amp;Häufigkeit:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Winkel:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>&amp;Punktaufbau:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Punkt</translation>
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
        <translation>Ellipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Farben:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>ICC-Profile benutzen</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Renderpriorität:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ farbmetrisch</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Farbsättigung</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Absolut farbmetrisch</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Bilder:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Eingebettete Profile nicht benutzen</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3-Ausgabebedingung</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Infotext:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Ausgabe&amp;profil:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Trim-Box</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Schriftarten in PDFs einbetten. Das schützt das
Layout und Erscheinungsbild des Dokuments vor Veränderungen.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="obsolete">Präsentationseffekte aktivieren im Adobe&amp;#174; Reader&amp;#174;. Nur im Vollbildmodus sichtbar.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Vorschau für aufgeführte Seiten anzeigen.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Zeit bis zur Anzeige der nächsten Seite.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Länge des Effekts in Sekunden.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Auswahl des Effekts.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Richtung für die Effekte Rollen und Schachbrett.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Startposition für die Effekte Einblenden und Teilen.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Richtung für die Effekte Jalousie und Teilen.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Gewählten Effekt auf alle Seiten anwenden.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Alle Seiten als PDF exportieren</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Einen Seitenbereich als PDF exportieren</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Geben Sie mit Hilfe von Kommata oder Bindestrichen ein, welche
Seiten importiert werden sollen, zum Beispiel
1-5 oder 3,4. * steht  für alle Seiten.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Legt die Art des Bundes in der PDF-Datei fest. Verwenden Sie die 
Standardeinstellung (links), solange Sie nicht genau wissen, dass 
Sie diese ändern müssen.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Erzeugt Vorschaubilder für jede Seite der PDF-Datei.
Einige Programme nutzen diese Bilder zur Navigation.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Erzeugt PDF-Artikel, die für die Navigation in verketteten Textrahmen nützlich sind.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="obsolete">Die Ebenen Ihres Dokuments werden in die PDF-Datei exportiert.
Nur in PDF 1.5 verfügbar.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Lesezeichen im Dokument in die PDF-Datei einbetten.
Nützlich zur Navigation in langen PDF-Dateien.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Ausgabe-Auflösung für Text und Vektorgrafiken.
Dies beeinflusst nicht die Auflösung der Bitmap-Grafiken.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Kompression von Text und Grafiken reduziert die Dateigröße.
Nicht ohne Grund deaktivieren.
</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Kompressions-Methode für Bitmap-Bilder.
Automatisch - Scribus wählt die beste Methode.
ZIP - Gut für Bilder mit Farbflächen.
JPEG - erzeugt kleinere PDF&apos;s bei Dokumenten mit viel Bildern ohne große Kompressionsverluste.
Diese Einstellung muss normalerweise nicht geändert werden.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Kompressionsgrad: Minimal (25%), Niedrig (50%), Mittel (75%), Hoch (80%), Maximal (95%)</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Punkte pro Zoll) für den Export von Bildern.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Aktiviert die Sicherheitsfunktionen in der exportierten PDF-Datei.
Wenn Acrobat 4.0 ausgewählt ist, wird die PDF-Datei mit 40 Bit verschlüsselt.
Wenn Acrobat 5.0 ausgewählt ist, wird die PDF-Datei mit 128 Bit verschlüsselt.
Hinweis: Die Verschlüsselung ist nicht so zuverlässig wie GPG oder PGP und 
besitzt einige Einschränkungen.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Wählen Sie ein Masterpasswort, das alle Sicherheitsfunktionen
in der exportierten Datei aktiviert</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Wählen Sie ein Passwort, das es Benutzern erlaubt, die PDF-Datei anzusehen.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Drucken erlauben. Wenn nicht ausgewählt, ist das Drucken verboten.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Veränderung der PDF-Datei gestatten. Wenn nicht ausgewählt, wird die Veränderung verhindert.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Kopieren von Text oder Bildern gestatten.
Wenn nicht ausgewählt, können weder Text von Bilder kopiert werden.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Hinzufügen von Anmerkungen gestatten.
Wenn nicht ausgewählt, wird das Hinzufügen von Anmerkungen verhindert.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Farbmodell für die Ausgabe der PDF-Datei.
Wählen Sie Monitor/Internet für PDF-Dateien, die auf dem Monitor angezeigt oder mit herkömmlichen Druckern gedruckt werden sollen.
Wählen Sie Drucker, um die PDF-Datei auf einem CMYK-Drucker auszugeben.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Diese erweiterte Option ist normalerweise deaktiviert und sollte nur aktiviert werden,
wenn Ihre Druckerei es verlangt und sie die notwendigen Informationen haben.
Andernfalls könnte ihre PDF-Datei fehlerhaft gedruckt und nicht plattformübergreifend verwendet werden.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Farbprofil für Füllfarben einbetten</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Farbprofil für Füllfarben</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Rendermethode für Füllfarben</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Farbprofil für Bilder einbetten</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Farbprofile der Quellbilder nicht benutzen</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Farbprofil für Bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Rendermethode für Bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Ausgabeprofil zum Drucken. Wenn möglich, benutzen Sie das Profil Ihres Druckers.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Erforderliche Eingabe für PDF/X-3, ohne die 
das Dokument nicht PDF/X-3-konform ist. Benutzen Sie z.B. den Titel des Dokuments.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Abzug für Anschnitt oben</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Abzug für Anschnitt unten</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Abzug für Anschnitt links</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Abzug für Anschnitt rechts</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Dokument horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Dokument vertikal spiegeln</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">Legt die PDF-Kompatiblität fest. Standard ist PDF 1.3, weil das die größte Kompatiblität ermöglicht.
Wählen Sie PDF 1.4, wenn Sie Funktionen wie Transparenz oder 128 bit-Verschlüsselung benötigen.
PDF/X-3 ist gedacht für gewerbliche Druckereien und nur möglich, wenn Sie das Farbmanagement aktiviert haben.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="obsolete">Berechnet die Auflösung von Bitmap-Grafiken in der gewünschten Höhe neu. 
Ansonsten wird die ursprüngliche Auflösung verwendet. Die Aktivierung dieser 
Option kann den Speicherbedarf erhöhen und die Ausgabe verlangsamen.</translation>
    </message>
    <message>
        <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Verlustfreie Kompression von Text und Grafiken. 
Ohne guten Grund sollten Sie diese Option nicht deaktivieren. 
Sie reduziert die Dateigröße.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Kompressionsmethode für Bilddateien. Automatisch überlässt Scribus die Wahl der besten Methode. ZIP ist verlustfrei und sinnvoll für Bilder mit Füllfarben. JPEG ist besser geeignet zur Erstellung kleinerer PDF-Dateien, die viele Fotos enthalten, auch wenn damit unter Umständen ein geringer Qualitätsverlust einhergeht. Belassen Sie die Einstellung bei Automatisch, solange Sie keine spezielle Kompressionsmethode benötigen.</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation>Konvertiert Schmuckfarben zu Prozessfarben</translation>
    </message>
    <message>
        <source>Compression &amp;Method:</source>
        <translation type="obsolete">Kompressions&amp;methode:</translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation>Kompressions&amp;qualität:</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation>Fonts in PDF einbetten. Dadurch bleibt das Layout und das Aussehen des Dokuments erhalten.</translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Länge eines Effekts. Je kürzer, desto schneller ist der Effekt, je länger, desto langsamer.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation>Geben Sie mit Kommata getrennt die Seiten ein, die verarbeitet werden sollen. * steht für alle Seiten, 1-5 für einen Bereich oder eine einzelne Seite. </translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="obsolete">Legt die Kompatibilität der PDF-Datei fest. Standard ist PDF 1.3 mit der größten Kompatibilität. Mit PDF 1.4 können Sie Transparenz und 128bit-Verschlüsselung verwenden. PDF/X-3 ist vorgesehen für kommerziellen RGB-Druck mit Farbprofilen. Es ist nur verfügbar, wenn Farbprofile aktiviert sind - verwenden Sie PDX/X-3 nur, wenn Ihre Druckerei das wünscht oder beim Druck auf einem Vierfarb-Laserdrucker.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation>Legt die Bindung für PDF-Dateien fest. Nur verändern, wenn Sie es wirklich brauchen. Standard: Links.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation>Erzeugt kleine Vorschaubilder von jeder Seite. Einige PDF-Betrachter nutzen diese Vorschaubilder zur Navigation.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation type="obsolete">Exportiert auch Ebenen in die PDF-Datei. Erst ab PDF 1.5 unterstützt.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation>Fügt die Lesezeichen mit in die PDF-Datei ein. So können Sie in langen PDF-Dateien besser navigieren.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation>Auflösung für Text und Vektorgrafiken. Beeinflusst nicht die Auflösung von Bitmapdaten wie Fotos.</translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation>Aktiviert verlustlose Komprimierung von Text und Grafiken. Verringert die Größe der PDF-Datei. Nicht ohne Grund deaktivieren.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="obsolete">Methode für die Komprimierung von Bitmapdaten. Automatisch bedeutet, dass Scribus die beste Methode wählt. ZIP ist verlustlos und gut für Bilder mit flächigen Farben. Mit JPEG erzeugen Sie kleinere PDFs, geeignet für viele Fotos. Hier können kleinere Qualitätsverluste auftreten. Empfehlung: Verändern Sie die Methode Automatisch nur, wenn Sie es wirklich benötigen.</translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation>Komprimierungsqualität für verlustbehaftete Komprimierung: Minimum (25%), Niedrig (50%), Mittel (75%) und Maximum (95%). Diese Einstellung wirkt sich nicht direkt auf die Größe der Bilder aus, weil Größe und Qualitätsverlust von Bild zu Bild variieren.</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation type="obsolete">Berechnet die Bilder neu auf die angegebene Auflösung. Ist die Option nicht aktiviert, wird die Originalauflösung verwendet. Dadurch wird mehr Speicher benötigt und der Export verlangsamt.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Aktiviert die Sicherheitsfunktionen in der exportierten PDF-Datei. Wenn Sie PDF 1.3 gewählt haben, wird die Datei mit 40-Bit-Verschlüsselung geschützt, bei PDF 1.4 wird 128-Bit-Verschlüsselung verwendet. Warnung: PDF-Verschlüsselung ist nicht so verlässlich wie GPG- oder PGP-Verschlüsselung und besitzt einige Einschränkungen.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation>Wählen Sie ein Master-Passwort, das alle Sicherheitsfunktionen in der PDF-Datei aktiviert oder deaktiviert</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation>Erlaubt das Entnehmen von Text oder Grafiken aus der PDF-Datei. Falls nicht aktiviert, können Texte oder Grafiken nicht kopiert werden.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation>Erlaubt das Hinzufügen von Anmerkungen. Falls nicht aktiviert, können keine Anmerkungen eingefügt werden.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Farbmodell für die Ausgabe der PDF-Datei. Wählen Sie Monitor / Internet für die Ausgabe auf einem Monitor oder das Drucken mit einem durchschnittlichen Tintenstrahldrucker. Wählen Sie Drucker, wenn auf einer echten Vierfarb-Maschine gedruckt werden soll.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Die ist eine fortgeschrittene Einstellung, die standardmäßig nicht aktiviert ist. Sie sollte nur aktiviert werden, wenn dies ausdrücklich von Ihrer Druckerei gefordert wird und diese Ihnen alle benötigten Details mitgeteilt hat. Ansonsten könnte Ihre PDF-Datei nicht korrekt gedruckt werden und Probleme beim plattformübergreifenden Datenaustausch bereiten.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Vorgeschriebener Text für PDF/X-3, ohne den die Datei nicht der PDF/X-3-Spezifikation entspricht. Wir empfehlen, den Titel des Dokumentes zu verwenden.</translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation>Ermöglicht die Umwandlung von Schmuck- in Prozessfarben. Solange Sie Schmuckfarben nicht wirklich benötigen, sollten Sie dies am besten aktiviert lassen.</translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation>&amp;Ebenen exportieren</translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation>Kom&amp;primierungsmethode:</translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation>Bilder &amp;neu berechnen auf:</translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation>Anzeigeeinstellungen</translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation>Seitenlayout</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation>Einzelne Seite</translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation>Fortlaufend</translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation>Doppelseite Links</translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation>Doppelseite Rechts</translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation>Anzeige</translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation>Standardeinstellung des Betrachters verwenden</translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation>Vollbildmodus verwenden</translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation>Lesezeichen anzeigen</translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation>Vorschaubilder anzeigen</translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation>Reiter für Ebenen anzeigen</translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation>Symbolleisten ausblenden</translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation>Menüleiste ausblenden</translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation>Seiten auf Größe des Betrachterfensters zoomen</translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation>Erweiterte Aktionen</translation>
    </message>
    <message>
        <source>Javascript to be executed
when Document is opened:</source>
        <translation type="obsolete">Javascript, das beim Öffnen 
des Dokuments ausgeführt wird:</translation>
    </message>
    <message>
        <source>No Script</source>
        <translation>Kein Script</translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation>Betrachter</translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation>Auf Seitenränder beschneiden</translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation>Verlustbehaftet - JPEG</translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation>Verlustlos - Zip</translation>
    </message>
    <message>
        <source>Image Compression Method</source>
        <translation>Kompressionsmethode für Bilder</translation>
    </message>
    <message>
        <source>Javascript to be executed
when PDF document is opened:</source>
        <translation>Javascript, das beim Öffnen
der PDF-Datei ausgeführt wird:</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; and other PDF viewers which support this in full screen mode.</source>
        <translation>Aktiviert die Präsentationseffekte beim Öffnen der Datei in Adobe&amp;#174; Reader&amp;#174; im Vollbildmodus und anderen PDF-Viewern, die das unterstützen.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF 1.5 is necessary when you wish to preserve objects in separate layers within the PDF.  PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation>Bestimmt die PDF-Kompatibilität. Standardwert ist PDF 1.3, welches größtmögliche Kompatibilität garantiert. Wählen Sie PDF 1.4, wenn Ihr Dokument Transparenzen enthält oder 128-Bit-Verschlüsselung verwenden soll. PDF 1.5 benötigen Sie, wenn Sie Ebenen im PDF-Dokument erhalten wollen. PDF/X-3 sollten Sie verwenden, wenn Sie RGB-Farbmanagement im kommerziellen Druck benötigen und kann nur ausgewählt werden, wenn Sie das Farbmanagement aktiviert haben. Verwenden Sie es nur, wenn Ihr Drucker dies verlangt oder in einigen Fällen, wenn auf einem Vierfarb-Laserdrucker gedruckt werden soll.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is chosen.</source>
        <translation>Ebenen bleiben beim Eport erhalten. Nur bei Version PDF 1.5.</translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options. This only affects JPEG images</source>
        <translation>Kompressionsmethode für JPEG-Bilddateien. Automatisch erlaubt Scribus, die beste Methode zu wählen. ZIP ist verlustlos und gut für Bilder mit Füllfarben. JPEG eignet sich zum Erstellen kleinerer PDF-Dateien, allerdings kann es hier zu leichten Verlusten der Bildqualität kommen. Belassen Sie es am besten bei Automatisch, es sei denn Sie benötigen diese speziellen Kompressionsoptionen</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. Enabling this will increase memory usage and slow down export.</source>
        <translation>Bilder in der gewählten Auflösung neu rendern. Wenn Sie diese Option wählen, erhöht sich der Speicherbedarf, und der Export dauert länger. Wenn Sie die Option nicht wählen, wird das Bild in der ursprünglichen Auflösung gerendert.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer. Choose Grayscale when you want a grey scale PDF.</source>
        <translation>Farbmodell für die PDF-Ausgabe. Monitor/Internet wird bevorzugt für die Darstellung am Bildschirm und zum Drucken auf Inkjet-Druckern verwendet, Drucker für das Drucken auf einem CMYK-Drucker. Wählen Sie Graustufen, um eine Graustufen-PDF zu erstellen.</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins in the exported file</source>
        <translation>Objekte außerhalb des Seitenrandes nicht exportieren</translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation>Schriftart:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Größe:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Fill Color:</source>
        <translation>Füllfarbe:</translation>
    </message>
    <message>
        <source>Stroke Color:</source>
        <translation>Linienfarbe:</translation>
    </message>
    <message>
        <source>Tab Fill Character:</source>
        <translation>Füllzeichen:</translation>
    </message>
    <message>
        <source>Tab Width:</source>
        <translation>Tab-Breite:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Spalten:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>A&amp;bstand:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Falsches Üben von Xylophonmusik quält jeden größeren Zwerg</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Linien&amp;farbe:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Tonwert:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Füll&amp;farbe:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>&amp;Tonwert:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation>Linienstil:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linien&amp;breite:</translation>
    </message>
    <message>
        <source>Line S&amp;tyle:</source>
        <translation>Liniens&amp;til:</translation>
    </message>
    <message>
        <source>Arrows:</source>
        <translation>Pfeile:</translation>
    </message>
    <message>
        <source>Start:</source>
        <translation>Anfang:</translation>
    </message>
    <message>
        <source>End:</source>
        <translation>Ende:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Freie Skalierung</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Horizontal skalieren:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Vertikal skalieren:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>Bild an &amp;Rahmen anpassen</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Seitenverhältnisse beibe&amp;halten</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Füll&amp;farbe:</translation>
    </message>
    <message>
        <source>Use embedded Clipping Path</source>
        <translation>Eingebetteten Pfad verwenden</translation>
    </message>
    <message>
        <source>On Screen Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Full Resolution Preview</source>
        <translation>Vorschau in voller Auflösung</translation>
    </message>
    <message>
        <source>Normal Resolution Preview</source>
        <translation>Vorschau in normaler Auflösung</translation>
    </message>
    <message>
        <source>Low Resolution Preview</source>
        <translation>Vorschau in geringer Auflösung</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mini&amp;mum:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ximum:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>Schritt&amp;weite:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Einstellungen für Textrahmen</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Einstellungen für Bildrahmen</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Einstellungen für Formen</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Einstellungen für Vergößerungsstufen</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Eigenschaften von Linien</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Eigenschaften von Polygonen</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Schriftart für neue Textrahmen</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Schriftgröße für neue Textrahmen</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Schriftfarbe</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Anzahl der Spalten in neuen Textrahmen</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Abstand zwischen den Spalten</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Vorschau der gewählten Schriftart</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Bildrahmen ermöglichen das Skalieren von Bildern auf eine beliebige Größe</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Horizontale Skalierung des Bildes</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Vertikale Skalierung des Bildes</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Seitenverhältnisse beibehalten</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Bilder werden auf die Größe des Rahmens skaliert</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Bilder behalten ihre ursprünglichen Seitenverhältnisse</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Füllfarbe für Bildrahmen</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Intensität der Füllfarbe</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Linienfarbe der Form</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Intensität der Linienfarbe</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Füllfarbe der Form</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Linienstil der Form</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Linienbreite der Form</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Kleinstmögliche Vergrößerungsstufe</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Größtmögliche Vergrößerungstufe</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Abstand zwischen zwei Vergrößerungsstufen</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Farbe der Linien</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Tonwert der Farbe</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Linienstil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Linienbreite</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Benutzerdefiniert:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Benutzerdefiniert: </translation>
    </message>
    <message>
        <source>Text Color:</source>
        <translation>Textfarbe:</translation>
    </message>
    <message>
        <source>Shading:</source>
        <translation>Tonwert:</translation>
    </message>
    <message>
        <source>Text Stroke:</source>
        <translation>Umriss:</translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Trennstrich</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Unterstrich</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Versatz:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>&amp;Versatz:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
    </message>
    <message>
        <source>Displacement:</source>
        <translation>Versatz:</translation>
    </message>
    <message>
        <source>Auto</source>
        <translation>Auto</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Linienbreite:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Durchgestrichen</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitälchen</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing</source>
        <translation>Auto&amp;matischer Zeilenabstand</translation>
    </message>
    <message>
        <source>Line Spacing:</source>
        <translation>Zeilenabstand:</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Versatz über der Grundlinie auf einer Zeile</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Relative Größe der tiefgestellen Buchstaben im Vergleich zur normalen Schriftgröße</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Versatz unter der Grundlinie auf einer Zeile</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Relative Größe der hochgestellten Buchstaben im Vergleich zur normalen Schriftgröße</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Relative Größe der Kapitälchen im Vergleich zur normalen Schriftgröße</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Prozentuale Vergrößerung des Zeilenabstandes zur Schriftgröße</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font expressed as a percentage of the fonts descender</source>
        <translation>Versatz unterhalb der Grundlinie der normalen Schrift als Prozentwert des Unterbandes</translation>
    </message>
    <message>
        <source>Line width expressed as a percentage of the font size</source>
        <translation>Linienstärke ausgedrückt in Prozent bezogen auf die Schriftgröße</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the normal font expressed as a percentage of the fonts ascender</source>
        <translation>Versatz über der Grundlinie der Schriftart ausgedrückt in Prozent bezogen auf die Größe des Oberbandes</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Links</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Rechts</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Komma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Zentriert</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Alle löschen</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Einzug für die erste Zeile des Absatzes</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Einzug von links für den ganzen Absatz</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Alle Tabulatoren löschen</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Position:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Kein</translation>
    </message>
    <message>
        <source>Fill Char:</source>
        <translation>Füllzeichen:</translation>
    </message>
    <message>
        <source>Custom:</source>
        <translation>Benutzerdefiniert:</translation>
    </message>
    <message>
        <source>Custom: </source>
        <translation>Benutzerdefiniert: </translation>
    </message>
    <message>
        <source>Dot</source>
        <translation>Punkt</translation>
    </message>
    <message>
        <source>Hyphen</source>
        <translation>Trennstrich</translation>
    </message>
    <message>
        <source>Underscore</source>
        <translation>Unterstrich</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Dokumentstruktur</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Gruppe</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation>Freie Objekte</translation>
    </message>
    <message>
        <source>Page </source>
        <translation>Seite </translation>
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
        <translation>Versatz</translation>
    </message>
    <message>
        <source>Linewidth</source>
        <translation>Linienstärke</translation>
    </message>
</context>
<context>
    <name>UndoManager</name>
    <message>
        <source>Add vertical guide</source>
        <translation>Vertikale Hilfslinie hinzufügen</translation>
    </message>
    <message>
        <source>Add horizontal guide</source>
        <translation>Horizontale Hilfslinie hinzufügen</translation>
    </message>
    <message>
        <source>Remove vertical guide</source>
        <translation>Vertikale Hilfslinie entfernen</translation>
    </message>
    <message>
        <source>Remove horizontal guide</source>
        <translation>Horizontale Hilfslinie entfernen</translation>
    </message>
    <message>
        <source>Move vertical guide</source>
        <translation>Vertikale Hilfslinie verschieben</translation>
    </message>
    <message>
        <source>Move horizontal guide</source>
        <translation>Horizontale Hilfslinie verschieben</translation>
    </message>
    <message>
        <source>Lock guides</source>
        <translation>Hilfslinien sperren</translation>
    </message>
    <message>
        <source>Unlock guides</source>
        <translation>Hilfslinien entsperren</translation>
    </message>
    <message>
        <source>Move</source>
        <translation>Verschieben</translation>
    </message>
    <message>
        <source>Resize</source>
        <translation>Größe ändern</translation>
    </message>
    <message>
        <source>Rotate</source>
        <translation>Drehung</translation>
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
        <translation>Markierung</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Gruppieren</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation>Gruppe markieren</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Erstellen</translation>
    </message>
    <message>
        <source>X: %1, Y: %2
W: %3, H: %4</source>
        <translation>X: %1, Y: %2
W: %3, H: %4</translation>
    </message>
    <message>
        <source>Align/Distribute</source>
        <translation>Ausrichten/Verteilen</translation>
    </message>
    <message>
        <source>Items involved</source>
        <translation>Objekte einbezogen</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Set fill color</source>
        <translation>Füllfarbe festlegen</translation>
    </message>
    <message>
        <source>Color1: %1, Color2: %2</source>
        <translation>Farbe1: %1, Farbe2: %2</translation>
    </message>
    <message>
        <source>Set fill color shade</source>
        <translation>Tonwert der Füllfarbe festlegen</translation>
    </message>
    <message>
        <source>Set line color</source>
        <translation>Linienfarbe festlegen</translation>
    </message>
    <message>
        <source>Set line color shade</source>
        <translation>Tonwert der Linienfarbe festlegen</translation>
    </message>
    <message>
        <source>Flip horizontally</source>
        <translation>Horizontal kippen</translation>
    </message>
    <message>
        <source>Flip vertically</source>
        <translation>Vertikal kippen</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Sperren</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Entsperren</translation>
    </message>
    <message>
        <source>Lock size</source>
        <translation>Größe sperren</translation>
    </message>
    <message>
        <source>Unlock size</source>
        <translation>Größe entsperren</translation>
    </message>
    <message>
        <source>Ungroup</source>
        <translation>Gruppe auflösen</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Umbenennen</translation>
    </message>
    <message>
        <source>From %1
to %2</source>
        <translation>Von %1
bis %2</translation>
    </message>
    <message>
        <source>Apply Master Page</source>
        <translation>Musterseite anwenden</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Einfügen</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Ausschneiden</translation>
    </message>
    <message>
        <source>Set fill color transparency</source>
        <translation>Deckkraft der Füllfarbe festlegen</translation>
    </message>
    <message>
        <source>Set line color transparency</source>
        <translation>Deckkraft der Linienfarbe festlegen</translation>
    </message>
    <message>
        <source>Set line style</source>
        <translation>Linienstil festlegen</translation>
    </message>
    <message>
        <source>Set the style of line end</source>
        <translation>Endstil der Linie festlegen</translation>
    </message>
    <message>
        <source>Set the style of line join</source>
        <translation>Eckenstil der Linien festlegen</translation>
    </message>
    <message>
        <source>Set line width</source>
        <translation>Linienbreite festlegen</translation>
    </message>
    <message>
        <source>No style</source>
        <translation>Kein Stil</translation>
    </message>
    <message>
        <source>Set custom line style</source>
        <translation>Benutzerdefinierter Linienstil</translation>
    </message>
    <message>
        <source>Do not use custom line style</source>
        <translation>Keinen benutzerdefinierten Linienstil verwenden</translation>
    </message>
    <message>
        <source>Set start arrow</source>
        <translation>Startpfeil festlegen</translation>
    </message>
    <message>
        <source>Set end arrow</source>
        <translation>Endpfeil festlegen</translation>
    </message>
    <message>
        <source>Create table</source>
        <translation>Tabelle erstellen</translation>
    </message>
    <message>
        <source>Rows: %1, Cols: %2</source>
        <translation>Zeilen: %1, Spalten: %2</translation>
    </message>
    <message>
        <source>Set font</source>
        <translation>Schriftart festlegen</translation>
    </message>
    <message>
        <source>Set font size</source>
        <translation>Schriftgröße festlegen</translation>
    </message>
    <message>
        <source>Set font width</source>
        <translation>Breite der Schrift festlegen</translation>
    </message>
    <message>
        <source>Set font height</source>
        <translation>Höhe der Schrift festlegen</translation>
    </message>
    <message>
        <source>Set font fill color</source>
        <translation>Füllfarbe der Schrift festlegen</translation>
    </message>
    <message>
        <source>Set font stroke color</source>
        <translation>Linienfarbe der Schrift festlegen</translation>
    </message>
    <message>
        <source>Set font fill color shade</source>
        <translation>Tonwert der Füllfarbe festlegen</translation>
    </message>
    <message>
        <source>Set font stroke color shade</source>
        <translation>Tonwert der Linienfarbe festlegen</translation>
    </message>
    <message>
        <source>Set kerning</source>
        <translation>Kerning festlegen</translation>
    </message>
    <message>
        <source>Set line spacing</source>
        <translation>Zeilenabstand festlegen</translation>
    </message>
    <message>
        <source>Set paragraph style</source>
        <translation>Absatzstil festlegen</translation>
    </message>
    <message>
        <source>Set language</source>
        <translation>Sprache festlegen</translation>
    </message>
    <message>
        <source>Align text</source>
        <translation>Text ausrichten</translation>
    </message>
    <message>
        <source>Set font effect</source>
        <translation>Texteffekt anwenden</translation>
    </message>
    <message>
        <source>Image frame</source>
        <translation>Bildrahmen</translation>
    </message>
    <message>
        <source>Text frame</source>
        <translation>Textrahmen</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polygon</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation>Bézierkurve</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polylinie</translation>
    </message>
    <message>
        <source>Convert to</source>
        <translation>Umwandeln in</translation>
    </message>
    <message>
        <source>Import SVG image</source>
        <translation>SVG-Bild importieren</translation>
    </message>
    <message>
        <source>Import EPS image</source>
        <translation>EPS-Datei importieren</translation>
    </message>
    <message>
        <source>Import OpenOffice.org Draw image</source>
        <translation>OpenOffice.org-Zeichnung importieren</translation>
    </message>
    <message>
        <source>Scratch space</source>
        <translation>Ablagefläche</translation>
    </message>
    <message>
        <source>Text flows around the frame</source>
        <translation>Text umfließt den Rahmen</translation>
    </message>
    <message>
        <source>Text flows around bounding box</source>
        <translation>Text umfließt die Bounding Box</translation>
    </message>
    <message>
        <source>Text flows around contour line</source>
        <translation>Text umfließt Konturlinie</translation>
    </message>
    <message>
        <source>No text flow</source>
        <translation>Kein Textfluss</translation>
    </message>
    <message>
        <source>No bounding box</source>
        <translation>Keine Bounding Box</translation>
    </message>
    <message>
        <source>No contour line</source>
        <translation>Keine Kontourlinie</translation>
    </message>
    <message>
        <source>Page %1</source>
        <translation>Seite %1</translation>
    </message>
    <message>
        <source>Set image scaling</source>
        <translation>Bildgröße festlegen</translation>
    </message>
    <message>
        <source>Frame size</source>
        <translation>Rahmengröße</translation>
    </message>
    <message>
        <source>Free scaling</source>
        <translation>Freie Skalierung</translation>
    </message>
    <message>
        <source>Keep aspect ratio</source>
        <translation>Größenverhältnis beibehalten</translation>
    </message>
    <message>
        <source>Break aspect ratio</source>
        <translation>Größenverhältnis nicht beibehalten</translation>
    </message>
    <message>
        <source>Edit contour line</source>
        <translation>Konturlinie bearbeiten</translation>
    </message>
    <message>
        <source>Edit shape</source>
        <translation>Form bearbeiten</translation>
    </message>
    <message>
        <source>Reset contour line</source>
        <translation>Konturlinie zurücksetzen</translation>
    </message>
    <message>
        <source>Add page</source>
        <translation>Seite hinzufügen</translation>
    </message>
    <message>
        <source>Add pages</source>
        <translation>Seiten hinzufügen</translation>
    </message>
    <message>
        <source>Delete page</source>
        <translation>Seite löschen</translation>
    </message>
    <message>
        <source>Delete pages</source>
        <translation>Seiten löschen</translation>
    </message>
    <message>
        <source>Add layer</source>
        <translation>Ebene hinzufügen</translation>
    </message>
    <message>
        <source>Delete layer</source>
        <translation>Ebene löschen</translation>
    </message>
    <message>
        <source>Rename layer</source>
        <translation>Ebene umbenennen</translation>
    </message>
    <message>
        <source>Raise layer</source>
        <translation>Ebene nach oben verschieben</translation>
    </message>
    <message>
        <source>Lower layer</source>
        <translation>Ebene nach unten verschieben</translation>
    </message>
    <message>
        <source>Send to layer</source>
        <translation>Auf Ebene verschieben</translation>
    </message>
    <message>
        <source>Enable printing of layer</source>
        <translation>Drucken der Ebene aktivieren</translation>
    </message>
    <message>
        <source>Disable printing of layer</source>
        <translation>Drucken der Ebene deaktiveren</translation>
    </message>
    <message>
        <source>Change name of the layer</source>
        <translation>Name der Ebene ändern</translation>
    </message>
    <message>
        <source>Get image</source>
        <translation>Bild laden</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Text auf einem Pfad</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation>Objekt drucken aktivieren</translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation>Objekt drucken deaktivieren</translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
        <translation>Mehrfach duplizieren</translation>
    </message>
    <message>
        <source>Change Image Offset</source>
        <translation>Bildposition ändern</translation>
    </message>
    <message>
        <source>Change Image Scale</source>
        <translation>Bildgröße ändern</translation>
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
</context>
<context>
    <name>UndoPalette</name>
    <message>
        <source>Initial State</source>
        <translation>Anfangszustand</translation>
    </message>
    <message>
        <source>Action History</source>
        <translation>Aktionsverlauf</translation>
    </message>
    <message>
        <source>Show selected object only</source>
        <translation>Nur markiertes Objekt anzeigen</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Rückgängig</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Wieder&amp;herstellen</translation>
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
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation>Minimale Seitenränder für Format: %1</translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation>Druckerränder verwenden</translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation>&amp;Drucker wählen:</translation>
    </message>
    <message>
        <source>Margins</source>
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Right:</source>
        <translation>Rechts:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Oben:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Unten:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Links:</translation>
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
        <translation>A&amp;bbrechen</translation>
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
        <translation>Wert einfügen</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Geben Sie einen Wert ein und klicken Sie auf OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Geben Sie einen Wert ein und klicken Sie auf OK</translation>
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
        <translation>Wert an das Script senden</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Umbruchkontrolle für Abkürzungen</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Geschütztes Leerzeichen anwenden auf:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Markierte Textrahmen</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aktive &amp;Seite</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Alle Objekte</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Standardeinstellungen durch Benutzereinstellungen überschreiben</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Wenn die Datei mit den benutzerdefinierten Einstellungen existiert
(%1), können Sie wählen, ob Sie Ihre Einstellungen an die globale 
Konfiguration anhängen wollen.

Sie können mit dieser Einstellung auch vordefinierte Werte mit ihren 
eigenen Einstellungen ersetzen.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Die markierten Rahmen wurden verarbeitet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aktuelle Seite wurde verarbeitet.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Alle Objekte wurden verarbeitet.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Umbruchkontrolle für Abkürzungen</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">In folgenden Sprachen verfügbar</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Über die Umbruchkontrolle</translation>
    </message>
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation type="obsolete">&amp;Systemkonfiguration bearbeiten...</translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation type="obsolete">&amp;Benutzerkonfiguration bearbeiten...</translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="obsolete">&amp;Editor einstellen...</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="obsolete">&amp;Informationen und Sprachen...</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">&amp;Hilfe</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="obsolete">Umbruchkontrolle für Abkürzungen</translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation type="obsolete">Sie bearbeiten die Datei %1 im Read-only Modus</translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="obsolete">Fehler: Die Anwendung &quot;%1&quot; kann nicht gestartet werden.</translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation type="obsolete">Umbruchkontrolle einrichten</translation>
    </message>
    <message>
        <source>Enter name of the plain text editor executable:</source>
        <translation type="obsolete">Geben Sie den Pfad zu Ihrem Texteditor ein:</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Eigenschaften...</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Text</source>
        <translation>Text</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Verknüpfung</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Schaltfläche</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Textfeld</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Kontrollkästchen</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Kombinationsfeld</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Listenfeld</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>PDF-Felder einfügen</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>PDF-Anmerkungen einfügen</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Wählen Sie den Importfilter aus</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisch</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Text ohne Formatierung importieren</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importfilter:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Kodierung:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Nur Text importieren</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Wählen Sie den Importfilter aus</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Verknüpfung mit Importfilter speichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association and do not ask again to select an importer for files of this type.</source>
        <translation>Die Verknüpfung von Dateityp und Importfilter speichern und nicht mehr nachfragen.</translation>
    </message>
</context>
<context>
    <name>nftdialog</name>
    <message>
        <source>New From Template</source>
        <translation>Neu von Vorlage</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Verwendungszweck</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Erstellt mit</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Entfernen</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Öffnen</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Vorlagen herunterladen</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Vorlagen installieren</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Bitte entpacken Sie die Vorlage nach ~/.scribus/templates für sich selbst oder nach PREFIX/share/scribus/templates für alle Benutzer.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Vorlage vorbereiten</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Vorlage entfernen</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>template.xml übersetzen</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Datum</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Weitere Dokumentvorlagen finden Sie unter http://www.scribus.net/ im Download-Bereich.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Sie müssen sicherstellen, dass Bilder und Schriften frei benutzt werden können. Wenn Sie Fonts nicht verbreiten dürfen, dann binden Sie diese beim Export als Vorlage nicht ein.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Wenn Sie Vorlagen erstellen, sollten Sie darauf achten, dass Sie die Informationen im Abschnitt &quot;Vorlagen installieren&quot; beachten. Das bedeutet, Benutzer sollten in der Lage sein, die Vorlage einfach in das entsprechende Verzeichnis zu kopieren und zu benutzen.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Wenn Sie eine Vorlage von dieser Liste entfernen, wird nur der entsprechende Eintrag in der Datei template.xml gelöscht, nicht die Vorlage selbst. Sie können Vorlagen nur löschen, wenn Sie Schreibrechte für template.xml besitzen.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopieren Sie die existierende template.xml nach template.lang_COUNTRY.xml (derselbe Ländercode wie bei den qm-Dateien), zum Beispiel template.fi.xml für Finnisch. Diese Kopie muss sich im selben Verzeichnis wie template.xml befinden.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Als Vorlage speichern</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Name</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Kategorie</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Verwendungszweck</translation>
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
        <translation>Mehr Details</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Weniger Details</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>US-Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>Querformat</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>Hochformat</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Filter erstellen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Gespeicherten Filter auswählen</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Geben Sie einen Namen für diesen Filter ein</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Name des Filters</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">A&amp;bbrechen</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Diese Zeile aktivieren oder deaktivieren</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Diese Zeile entfernen</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Neue Zeile hinzufügen</translation>
    </message>
    <message>
        <source>to</source>
        <translation>bis</translation>
    </message>
    <message>
        <source>and</source>
        <translation>und</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>Ergebnisse entfernen</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>Ergebnisse nicht entfernen</translation>
    </message>
    <message>
        <source>words</source>
        <translation>Wörter</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Entfernen</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Ersetzen</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Anwenden</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Linker Wert ist ein regulärer Ausdruck</translation>
    </message>
    <message>
        <source>with</source>
        <translation>mit</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>Absatzstil</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>Alle Instanzen von</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>Alle Absätze</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>Absätze beginnen mit</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>Absätze mit weniger als</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>Absätze mit mehr als</translation>
    </message>
</context>
</TS>
