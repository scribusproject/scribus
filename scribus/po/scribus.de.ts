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
        <translation>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -&gt; bool

Erzeugt ein neues Dokument, und gibt den Wert \&quot;true \&quot;zurück, wenn das erfolgreich war. Die Parameter haben folgende Bedeutungen:

    size = Ein Zahlenpaar (Breite,Höhe), das die Größe des Dokuments beschreibt. Sie können auch Konstanten verwenden, angegeben durch PAPER_&lt;Papiertyp&gt;, z.B. PAPER_A4 usw.

    margins = Die Werte (links,rechts,oben,unten) geben die Größe der Ränder an

    orientation = entweder PORTRAIT oder LANDSCAPE

    firstPageNumber = die Seitenzahl der ersten Seite des Dokuments. Normalerweise 1 - höhere Zahlen sind sinnvoll, wenn Sie ein Projekt auf mehrere Dateien aufteilen.

    unit: Legt die Maßeinheit für das Dokument fest. Gültige Konstanten sind: UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS und UNIT_POINTS.

    facingPages = FACINGPAGES, NOFACINGPAGES

    firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

Die Werte für Breite, Höhe und die Ränder müssen in der Einheit angegeben werden, die für das Dokument festgelegt ist. Die Konstanten PAPER_* sind in Punkt angegeben. Falls die Maßeinheit für Ihr Dokument nicht Punkt ist, achten Sie auf diesen Sachverhalt.

Beispiel: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
</translation>
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
Standard ist bool=True. Diese Einstellung bleibt bestehen, auch wenn das Skript beendet ist.
Stellen Sie also sicher, dass setRedraw(True) aufgerufen wird, bevor das Skript beendet wird.</translation>
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
        <source>newPage(where [,&quot;template&quot;])

Creates a new page. If &quot;where&quot; is -1 the new Page is appended to the
document, otherwise the new page is inserted before &quot;where&quot;. Page numbers are
counted from 1 upwards, no matter what the displayed first page number of your
document is. The optional parameter &quot;template&quot; specifies the name of the
template page for the new page.

May raise IndexError if the page number is out of range
</source>
        <translation>newPage(where [,&quot;template&quot;])

Erstellt eine neue Seite. Wenn &quot;where&quot; den Wert -1 hat, wird die Seite hinten angefügt, ansonsten
vor der Seite &quot;where&quot;. Seitenzahlen werden von 1 aufwärts gezählt, egal, welche Seitenzahl
auf der Seite angezeigt wird. Das optionale Argument &quot;template&quot; gibt den Namen der Vorlage für
die neue Seite an.

Wenn die Seitenzahl außerhalb des gültigen Bereichs ist, tritt der Fehler IndexError auf</translation>
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
        <translation>register_macro_callable(name, callable, accel=&apos;&apos;)

Erstellt ein Makro mit dem Namen &quot;name&quot; mit dem Objekt &quot;callable&quot;, das aufrufbar ist und
bereits existiert. &quot;callable&quot; muss ohne Argumente angegeben werden.
Der String &quot;accel&quot; legt ein eventuelles Tastenkürzel für dieses Makro fest.
&quot;Callable&quot; darf keine Klasse sein. Erlaubt sind dagegen Funktionen, Methoden und Klasseninstanzen,
die eine __call__-Methode ohne Argument bereitstellen. 
Es ist sowohl erlaubt, mehr als ein &quot;callable&quot; zu verwenden, als auch mehrere Methoden einer Instanz zu benutzen.</translation>
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
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation>defineColor(&quot;name&quot;, c, m, y, k)

Legt eine neue Farbe mit dem Namen &quot;name&quot; an und den CMYK-Werten c,m,y,k an. 
Die vier Farbwerte müssen zwischen 0 und 255 liegen.

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
        <translation>fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; string with filename

Zeigt den Dialog zum Öffnen einer Datei an mit dem Titel &quot;caption&quot;. Anhand von &quot;filter&quot;
werden die Dateien ausgewählt, die angezeigt werden sollen. Sie können auch einen
Standard-Dateinamen oder -Pfad auswählen. haspreview=true aktivert die kleine Dokument-
vorschau. Bie issave=True verhält sich der Dialog wie ein &quot;Speichern unter&quot; Dialog, andernfalls
wie ein &quot;Dokument öffnen&quot;-Dialog. Normalerweise sind beide Optionen auf False gesetzt.

Der Dateifilter hat die Form &apos;Beschreibung (*.typ *typ2 ...)&apos;, zum Beispiel
&apos;Bilder (*.png *.xpm *.jpg)&apos;.

Für weitere Details lesen Sie bitte in der Qt-Dokumentation nach.

Beispiele:
fileDialog(&apos;Datendatei öffnen&apos;,&apos;CSV-Dateien (*.csv)&apos;)
fileDialog(&apos;Report sichern&apos;,defaultname=&apos;report.txt&apos;,issave=True)</translation>
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
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getCornerRadius([&quot;name&quot;]) -&gt; integer

Gibt den Eckradius von &quot;name&quot; zurück. Die Einheit des Radius ist Punkt.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.</translation>
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
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation>getPosition([&quot;name&quot;]) -&gt; (x,y)

Gibt das Zahlenpaar (x,y) mit der Position des Objekts &quot;name&quot; zurück.
Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt verwendet.
Die Angabe erfolgt in der Maßeinheit des Dokuments - siehe die
UNIT_*-Konstanten.</translation>
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
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>rotateObjectAbs(rot [, &quot;name&quot;])

Setzt die Drehung des Objekts &quot;name&quot; auf &quot;rot&quot;. Positive Werte bedeuten Drehung
im Uhrzeigersinnd. Ist &quot;name&quot; nicht angegeben, wird das aktuelle Objekt benutzt.</translation>
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
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</source>
        <translation type="obsolete">rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Erzeugt einen Vorschautext von der Schriftart &quot;name&quot; mit dem Text &quot;sample&quot;
und der Größe &quot;size&quot;. Das Bild wird unter dem Namen &quot;filename&quot; gespeichert.
Gibt den Wert True zurück, wenn die Aktion erfolgreich war.

Kann die Schriftart nicht gefunden werden, tritt der Fehler NotFoundError auf.
Ist &quot;sample&quot; oder &quot;filename&quot; leer, tritt der Fehler ValueError auf.</translation>
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
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the Layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation type="obsolete">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Gibt True zurück, wenn die Ebene sichtbar ist und False, wenn die Ebene
unsichtbar ist. 

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf.
Ist der Name der Ebene ungültig, tritt der Fehler ValueError auf.</translation>
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

Gibt True zurück, wenn die Ebene gedruckt wird und False, wenn die Ebene nicht
gedruck wird.

Ist die Ebene nicht vorhanden, tritt der Fehler NotFoundError auf.
Ist der Name der Ebene ungültig, tritt der Fehler ValueError auf.</translation>
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

Erzeugt eine Bezierkurve und gibt ihren Namen zurück. Die Punkte werden in
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
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="obsolete">getPageMargins()

Gibt die Seitenränder als eine Folge von (links, rechts, oben, unten) in der aktuellen
Maßeinheit zurück. Siehe UNIT-Konstanten und getPageSize().</translation>
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
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame. Text
must be UTF encoded (see setText() as reference) The first character has an
index of 0. &quot;name&quot; If &quot;name&quot; is not given the currently selected Item is
used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation type="obsolete">insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Fügt den Text &quot;text&quot; an der Position &quot;pos&quot; in den Rahmen &quot;name&quot; ein.
Der Text muss UTF8-kodiert sein (siehe setText() als Beispiel). Das erste Zeichen
hat den Index 0. Ist &quot;name&quot; nicht angegeben, wird der aktuelle Textrahmen verwendet.

Ist die Einfüge-Position ungültig, wird der Fehler ValueError ausgegeben.</translation>
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
        <translation type="obsolete">selectText(start, count, [&quot;name&quot;])

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
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared. If &quot;count&quot; is -1, all text in the frame will
be selected. If &quot;name&quot; is not given the currently selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation>selectText(start, count, [&quot;name&quot;])

Wählt &apos;count&apos; Zeichen im Textrahmen &apos;name&apos; aus, beginnend von &apos;start&apos;.
Die Zählung der Zeichen beginnt bei 0. 
&apos;count&apos;=0 löscht eine bestehende Markierung. 
&apos;count&apos;=-1 markiert den gesamten Text im Textrahmen.
Ist &apos;name&apos; nicht angegeben, wird der aktuelle Rahmen verwendet.

Kann den Fehler IndexError verursachen, wenn die Auswahl über die Grenzen
des Textes hinausgeht.</translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
</source>
        <translation>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Erstellt ein Makro mit dem Namen &quot;name&quot;, indem &quot;sourcetext&quot; als Quelltext ausgewertet wird.
&quot;sourcetext&quot; muss den gleichen Regeln folgen, wie Makros, die in der GUI erstellt wurden.
Falls angegeben setzt der String &quot;accel&quot; ein Tastenkürzel für dieses Makro.</translation>
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
        <source>getColorAsRGB(&quot;name&quot;) -&gt; tuple

Returns a tuple (R,G,B) containing the three color components of the
color &quot;name&quot; from the current document, converted to the RGB colour
space. If no document is open, returns the value of the named color
from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>getColorAsRGB(&quot;name&quot;) -&gt; tuple

Gibt ein Tupel (R,G,B) zurück, dass die drei Farbkomponenten der Farbe &quot;color&quot;
des aktuellen Dokuments im RGB-Modus widerspiegelt. Wenn kein Dokument
geöffnet ist, wird die Farbe aus den Standard-Dokumentfarben genommen.

Der Fehler NotFoundError tritt auf, wenn die Farbe nicht vorhanden ist.
Der Fehler ValueError tritt auf, wenn der Name der Farbe ungültig ist.</translation>
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
        <source>isLayerVisible(&quot;layer&quot;) -&gt; bool

Returns whether the layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerVisible(&quot;layer&quot;) -&gt; bool

Gibt zurück, ob die Ebene &quot;layer&quot; sichtbar ist oder nicht. True bedeutet sichtbar,
False bedeutet unsichtbar.

Der Fehler NotFoundError tritt auf, wenn die Ebene nicht vorhanden ist.
Der Fehler ValueError tritt auf, wenn der Name der Ebene ungültig ist.</translation>
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
        <source>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserts the text &quot;text&quot; at the position &quot;pos&quot; into the text frame &quot;name&quot;.
Text must be UTF encoded (see setText() as reference). The first character has
an index of 0. Inserting at position -1 appends text to the frame. If &quot;name&quot;
is not given the currently selected Item is used.

May throw IndexError for an insertion out of bounds.
</source>
        <translation>insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Fügt den Text &quot;text&quot; an der Position &quot;pos&quot; in den Textrahmen &quot;name&quot; ein.
Der Text muss UTF-kodiert sein (siehe setText()). Das erste Zeichen hat den
Index 0. Eine Position von -1 fügt den Text zu dem vorhandenen Text hinzu.
Ist &quot;name&quot; nicht angegeben, wird der aktuelle Textrahmen verwendet.

Der Fehler IndexError tritt auf, wenn eine ungültige Position angegeben wird.</translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3 </translation>
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
        <source>Contributions from:</source>
        <translation>Beiträge von:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Windows-Portierung:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Deutsch:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Französisch:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italienisch:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Ungarisch:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ukrainisch:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Bulgarisch:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galizisch:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Türkisch:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Litauisch:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Polnisch:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Tschechisch:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Slowakisch:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Dänisch:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norwegisch:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Walisisch:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Russisch:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Brasilianisch:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Finnisch:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Slovenisch:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Baskisch:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Dieser Dialog zeigt die Version, das Kompilierungsdatum und
die unterstützten Bibliotheken in Scribus an
Die Zeichen stehen für C=CUPS, C=LittleCMS und T=TIFF.
Fehlende Unterstützung wird duch ein Sternchen angezeigt</translation>
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
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation>Schwedisch:</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Entwickler-Team:</translation>
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
        <source>English (British):</source>
        <translation>Englisch (Britisch):</translation>
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
        <source>Catalan:</source>
        <translation>Katalanisch:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>Koreanisch:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>Spanisch:</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Offizielle Übersetzungen und Übersetzer:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>Serbisch:</translation>
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
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Dieses Dialogfeld zeigt die Version, das Kompilierungsdatum und
die verfügbaren Bibliotheken von Scribus an.
C-C-T steht dabei für C=littlecms, C=CUPS und T=TIFF.
* steht für fehlende Bibliotheken</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation>%1 %2 %3</translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation>Afrikaans:</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation>Portugiesisch (Brasilien):</translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation>Weitere Optionen</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Erstellt eine PostScript-Level 3 Datei</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Erstellt eine PostScript-Level 2 Datei. Vorsicht:
hierbei können sehr große Dateien entstehen</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Erstellt eine PostScript-Level 1 Datei. Vorsicht:
hierbei können sehr große Dateien entstehen</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Seiten &amp;horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Seiten &amp;vertikal spiegeln</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>&amp;ICC-Profile anwenden</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript-Level &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript-Level &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript-Level &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Unterfarben&amp;reduktion durchführen</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Mit dieser Option werden ein paar Grautöne, die entstehen,
wenn Black aus Cyan, Magenta und Yellow gemischt wird, durch Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr nahe stehen. Diese Option kann zu besseren Bildern führen,
allerdings müssen Sie von Fall zu Fall entscheiden, wie Sie bessere Ergebnisse
bekommen. Außerdem reduziert UFR die Gefahr einer Übersättigung mit CMY.
</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation>Mediengröße festlegen</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation>Diese Option setzt die Mediengröße
der Postscript-Datei. Aktivieren Sie die Option
nur, wenn es Ihre Druckerei fordert.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation>Abstand/Ausrichtung</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Ausrichten</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Waagrecht</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Linken Kanten</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Mitten</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Rechten Kanten</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertikal</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Oberen Kanten</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Unteren Kanten</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>An&amp;wenden</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;zwischen:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>&amp;Ausrichten</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>&amp;Versatz</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>&amp;Gleichmäßig verteilen</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>zwi&amp;schen:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>&amp;Nicht verändern</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>A&amp;usrichten</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Ve&amp;rsatz</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Gleich&amp;mäßig verteilen</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>N&amp;icht verändern</translation>
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
        <translation>Voreinstellung ist angekreuzt</translation>
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
        <translation>Hier eine komma-separierte Liste der Felder eintragen</translation>
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
        <translation>Bilder (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Alle Dateien (*)</translation>
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
        <translation>JavaScript</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation>Musterseite anwenden</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Vorlage:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Auf &amp;aktuelle Seite anwenden</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Anwenden von &amp;Seite:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Auf alle &amp;geraden Seiten anwenden</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Auf alle &amp;ungeraden Seiten anwenden</translation>
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
        <translation>Bibliotheken (*.scs);;Alle Dateien (*)</translation>
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
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
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
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Speichern &amp;unter...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Klein</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Mittel</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Groß</translation>
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
        <source>Color Management Settings</source>
        <translation>Farbmanagement Einstellungen</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Farbprofile</translation>
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
        <translation>Relativ Farbmetrisch</translation>
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
        <translation>Farbprofil für Bilder</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Farbprofil für Objektfarben</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Farbprofil für den Monitor.</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Farbprofil für den Drucker.</translation>
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
        <translation>Standard-Rendering Methode für Ihren Monitor. Ohne einen Grund zur Veränderung
sollte entweder relativ farbmetrisch oder Wahrnehmung aktiviert sein.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Standard-Rendering Methode für den Drucker. Ohne einen Grund zur Veränderung
sollte entweder relativ farbmetrisch oder Wahrnehmung aktiviert sein.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Aktiviert die Darstellung der Farben basierend auf dem 
gewählten Druckerprofil.
</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Aktiviert die Darstellung der Farben, die mit dem aktuellen Profil ungenau wiedergegeben werden.
Das erfordert sehr genaue Profile und dient nur als Anhaltspunkt.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Farb-Management &amp;aktivieren</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Bilder:</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Web Farben</translation>
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
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Der Name der Farbe ist nicht eindeutig</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Sie können keine Farbe mit dem Namen &quot;%1&quot;.
Dieser Name ist für die transparente Farbe reserviert</translation>
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
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
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
        <translation>Umgekehrt Diagonaler Verlauf</translation>
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
        <translation>Keine</translation>
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
        <translation>Farbe des selektierten Objekts</translation>
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
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Benutzerdefinierter radialer Verlauf</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Optionen für CSV-Importer</translation>
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
        <translation>Keiner</translation>
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
        <translation>Dieser Dialog zeigt verschiedene CUPS-Optionen zum Drucken an.
Die verfügbaren Einstellungen sind von Ihrem Drucker anhängig.
Hlife -&gt; Über zeigt Ihnen den CUPS-Support an.
Dabei stehen die Zeichen für C=CUPS, C-LittleCMS und T=TIFF.
Fehlende Bibliotheken werden durch ein Sternchen angezeigt</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Geht zum Dokumenten Verzeichnis.
Wird in den Voreinstellungen eingestellt.</translation>
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
    <name>DelColor</name>
    <message>
        <source>Delete Color</source>
        <translation>Farbe löschen</translation>
    </message>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Ersetzen durch:</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Farbe löschen:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>Delete Pages</source>
        <translation>Seiten löschen</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Löschen von:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>bis:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation>Fehlende Schrift</translation>
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
        <source>OK</source>
        <translation>OK</translation>
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
        <translation>&amp;Stichworte:</translation>
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
        <source>&amp;Document</source>
        <translation>&amp;Dokument</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Weitere &amp;Informationen</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Die Person oder Organisation, die vorrangig verantwortlich für den Inhalt des Dokuments ist.
Diese Feld kann sowohl in das Scribus-Dokument als auch in die Meta-Daten einer PDF-Datei eingebettet werden</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Ein Name für das Dokment
Diese Feld kann sowohl in das Scribus-Dokument als auch in die Meta-Daten einer PDF-Datei eingebettet werden</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Ein Abriss des Inhalts.
Hier kann eine kurze Beschreibung oder Zusammenfassung stehen. Sie wird in die PDF-Datei exportiert</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>Worte, die den Inhalt des Dokuments beschreiben.
Der Inhalt dieses Feldes wird in die PDF-Datei exportiert und hilft Ihnen, PDF-Dateien zu indizieren und und wiederzufinden</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Eine Person oder Organisation, die für die Veröffentlichung des Dokuments verantwortlich ist</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Eine Person oder Organisation, die bei dem Dokument mitgearbeitet hat</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Ein Datum, das mit der Entstehung des Dokuments verbunden ist, nach ISO 8601 im Format YYYY-MM-DD </translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Die Gattung oder der Typ des Dokuments, z.B. Kategorien, Funktionen, Arten usw</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>Die physische oder digitale Veröffentlichungsform des Dokuments. Abmessung und Medien-Typ können hier notiert werden.
Für MIME-Typen sind auch RFC2045 und RFC2046 gebräuchlich</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Eine eindeutige Referenz zu dem Dokument in einem gegebenen Kontext wie ISBN oder URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>Die Sprache , in der der Inhalt des Dokuments geschrieben ist, normalerweise ein ISO-639 Sprachcode,
optional ergänzt durch ein Bindestrich und den ISO-3166 Ländercode, wie z.B. en-GB oder fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Eine Referenz zu einem ähnlichen Dokument, wenn möglich eine formale Identifikation wie ISBN oder URI benutzen</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Der Ausbreitungsbereich des Dokuments, wenn möglich mit Ort, Zeit und Gerichtsbarkeit</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informationen über Rechte in dem oder über das Dokument, z.B. Copyright, Patente oder Handelsmarken</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Eine Referenz zu einem Dokument, von dem sich das aktuelle Dokument ableitet, z.B. ISBN oder URI</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Postscript-Dateien (*.ps);;Alle Dateien (*)</translation>
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
        <translation>Druckerwahl</translation>
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
        <translation>&amp;Normal drucken</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>&amp;Farbauszüge drucken</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>In Fa&amp;rbe drucken, falls verfügbar</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>In &amp;Graustufen drucken</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Er&amp;weiterte Optionen...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Drucken</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Einen anderen Druckmanager benutzen, zum Beispiel kprinter oder gtklp,
um zusätzliche Optionen einstellen zu können</translation>
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
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation>Makro bearbeiten: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Makro-Manager für Scribus</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation>Die Datei &apos;%1&apos; ist schon vorhanden.
Wollen Sie die Datei überschreiben?</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation>Sie haben dieses Makro verändert.
Wollen Sie die Änderungen ignorieren?</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation>Es folgt eine gesamte Rückverfolgung:

%1</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation>Das Makro konnte nicht erstellt werden, deshalb kann 
es in der jetzigen Form nicht gespeichert werden. 
Fehler:
%1</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation>Scribus - Neues Makro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Das ist der Makro bearbeiten / Neues Makro - Dialog. Hier können Sie den Quellcode der Makros bearbeiten. Verändern Sie dazu den Code in dem Bereich &quot;Quellcode&quot; und klicken Sie auf OK, um die Änderungen zu übernehmen.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation>Quellcode:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation>Makro bearbeiten:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation>Der Name des Makros, das Sie bearbeiten möchten.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation>&lt;/qt&gt;Hier wird der Name des aktuellen Makros angezeigt.&lt;qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Alle Änderungen ignorieren und beenden.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Den Dialog verlassen und alle Änderungen verwerfen. Wenn Sie das Makro nicht speichern, aber die Änderungen trotzdem festhalten wollen, dann speichern Sie das Makro mit &quot;Quelltext speichern unter...&quot;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Änderungen speichern und beenden.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Änderungen speichern und beenden. Wenn es ein Problem mit dem Makro gibt, werden Sie eine Fehlermeldung sehen und der Dialog bleibt geöffnet.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;In diesem Textfeld befindet sich der Quelltext des Makros. Wenn Sie ein neues Makro erstellen, sehen Sie eine leere Fläche vor sich und wenn Sie ein vorhandenes Makro bearbeiten, sehen Sie dessen Quelltext.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation>Quelltext &amp;laden...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation>Alt+L</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ersetzt den aktuellen Quelltext mit dem einer vorhandenen Datei.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Neuen Quelltext in das Bearbeitungsfenster von Datei laden. Vorhandener Befehle werden dabei gelöscht. Der Quelltext muss ein Makro für Scribus sein. Wenn Sie eine anderes Script laden, müssen Sie es noch bearbeiten, bevor Sie es speichern, damit es als Scripter Makro funktioniert.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation>Quelltext speichern &amp;unter...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Speichert den aktuell vorhandenen Quelltext in eine Datei.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation>Speichert den Quelltext des Makros in eine Datei. Sie können diese Datei mit dem Befehl &quot;Quelltext laden...&quot; wieder aufrufen.</translation>
    </message>
    <message>
        <source>Python source files (.py)</source>
        <translation type="obsolete">Python Quelltext (.py)</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation>Datei speichern</translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation>Quelltext speichern</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation>Datei öffnen</translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation>Wählen Sie den Quelltext aus</translation>
    </message>
    <message>
        <source>Python source files (*.py)</source>
        <translation type="unfinished"></translation>
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
        <source>Effect:</source>
        <translation>Effekt:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Senkrechte Abstände</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Zeilenabstand</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Der Name der Stilvorlage ist nicht eindeutig</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Name des Absatz-Stils</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Schriftart des Objekts</translation>
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
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Stilistischer Effekt: Erzeugt einen übergroßen ersten Buchstaben in einem Absatz</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Legt die gesamte Höhe des Initials in Zeilennummern fest</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Text am Grundlinienraster ausrichten</translation>
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
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <source>&amp;Font:</source>
        <translation>Schrift&amp;art:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Ausrichtung:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Initialien</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Zeilen:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Füll&amp;farbe:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>&amp;Randfarbe:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Text am &amp;Grundlinienraster ausrichten</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>Zeilenab&amp;stand:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>&amp;Oberhalb:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Unterhalb:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Javascripte (*.js);;Alle Dateien (*)</translation>
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
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>&amp;All pages</source>
        <translation>Alle &amp;Seiten</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Ausgabeverzeichnis wechseln</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Verfügbare Export-Formate</translation>
    </message>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Wählen Sie ein Ausgabe-Verzeichnis</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Das Ausgabeverzeichnis - dort werden Ihre Bilder gespeichert.
Dateinamen der Bilder haben das Format &apos;NamedesDokuments-Seite.Dateiformat</translation>
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
        <translation>Eine Seitenbereich exportieren</translation>
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
        <translation>Gibt die Qualität der Bilder an - 100% beste Qualität...1% schlechteste Qualität</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Größe der Bilder. 100% verändert nichts, 200% für doppelt so große Bilder etc.</translation>
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
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Colors</source>
        <translation>Farben</translation>
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
        <source>New Color</source>
        <translation>Neue Farbe</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
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
        <source>Append colors to the current set from an existing document</source>
        <translation>Farben aus anderem Dokument nachladen</translation>
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
        <translation>Aktuelle Palette zur Voreinstellung machen</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>An&amp;fügen</translation>
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
        <source>&amp;Save Color Set</source>
        <translation>Farbset &amp;speichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Name:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>setHGuides(list)

Legt horizontale Hilfslinien fest, list muss eine Liste der Linien sein, angegeben in der Maßeinheit
des Dokuments, siehe UNIT-Konstanten.

Beispiel:
setHGuides(getHGuides() + [200.0, 210.0] # fügt eine neue Hilfslinie hinzu
setHGuides([90,250]) # ersetzt alle vorhandenen Hilfslinien</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation>Globale Schrifteinstellungen</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Verfügbare Schriften</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Schrift Ersetzungen</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Zusätzliche Pfade</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Schrift Name</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Ersatz</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Benutzen</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Einbetten in:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Unterteilen</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Pfad zur Schriftdatei</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Verfügbare &amp;Schriftarten</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Schriftarten-&amp;Ersetzung</translation>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>The quick brown fox jumps over the lazy dog</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Schriftartenvorschau</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation>Ausgewählte Schriftart dem Stil zuweisen</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation>Vorschau verlassen</translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation>Schriftgröße</translation>
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
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Leider ist kein Handbuch verfügbar! Auf http://docs.scribus.net finden Sie aktualisierte Handbücher und auf http://www.scribus.net finden Sie weitere Downloads.</translation>
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
        <source>S&amp;earch</source>
        <translation>S&amp;uchen</translation>
    </message>
    <message>
        <source>Ctrl+F</source>
        <translation>Strg+F</translation>
    </message>
    <message>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <source>unknown</source>
        <translation>unbekannt</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Suchen</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Suchbegriff:</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Trennungsvorschlag</translation>
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
        <source>Hyphenator Settings</source>
        <translation>Einstellungen</translation>
    </message>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Trenn-&amp;Vorschläge</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Text automatisch während der Ein&amp;gabe trennen</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Wenn Sie die Funktion Extras - Text trennen benutzen, können Sie aus einer Dialogbox aus allen möglichen Trennungen für jedes Wort wählen.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Aktiviert automatische Trennung während Ihrer Eingabe.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Aufeinanderfolgende &amp;Trennungen:</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Seite einfügen</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Einfügen von</translation>
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
        <source>Template (Right Page):</source>
        <translation>Rechte Seite basiert auf:</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Einfügen von</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Seite(n)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>&amp;Musterseite (Links):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Musterseite:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Tabelle einfügen</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Anzahl der Zeilen:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Anzahl der Spalten:</translation>
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
        <translation>Warnung</translation>
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
        <translation>Schl&amp;ießen</translation>
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
        <translation>Wollen Sie dieses Script wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Tastenkombinationen einstellen</translation>
    </message>
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
        <translation>Warnung</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Diese Tastenkombination wird bereits verwendet</translation>
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
        <translation>Neue Ebene hinzufügen</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Ebene löschen</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Ebene nach oben verschieben</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Ebene nach unten verschieben</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Neue Ebene</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Wollen Sie auch alle Objekte dieser Ebene löschen?</translation>
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
        <translation>Warnung</translation>
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
        <translation>&amp;Anfügen</translation>
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
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Wollen Sie diesen Absatzstil wirklich löschen?</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation>pt</translation>
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
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation>Das Objekt lässt sich nicht aufrufen</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation>Makros verwalten</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation>Öffnet ein Dialog, mit dem Sie Makros erstellen, löschen, bearbeiten, speichern und laden können.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation>Makros erstellen, bearbeiten und löschen</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation>&amp;Makros</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Makromanager</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation>Fehler beim Öffnen der Datei %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation>Scribus - Makro bearbeiten</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Dieser Makroname ist bereits vergeben.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Fehler beim Erstellen des Makros. Der Makromanager konnte das Makro nicht einrichten.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation>Das Makro &apos;%1&apos; hat einen geringfügigen Fehler zurückgegeben.
Der Fehler ist: %2
Eine komplette Rückverfolgung finden Sie hier:

%3</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation>Das Makro &apos;%1&apos; konnte nicht korrekt ausgeführt werden.
Der Fehler ist: %2
Eine komplette Rückverfolgung finden Sie hier:

%3</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation>Scribus - Neues Makro</translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation>Geben Sie einen Namen für das Makro ein:</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation>Scribus - Makromanager</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation>Fehler beim Umbenennen des Makros, weil der Name bereits verwendet wird.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation>Scribus - Makros verwalten</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Dieses Fenster ist die Makroverwaltung von Scribus. Hier können Sie Makros erstellen, löschen, bearbeiten usw. Alle Änderungen können Sie mit den Buttons auf der rechten Fensterseite vornehmen.&lt;/p&gt;
&lt;p&gt;Alle Änderungen werden sofort übernommen - Sie können Ihre Änderungen nicht zurücknehmen.
Die Übersicht in diesem Dialog zeigt Ihnen, welche Makros bereits geladen sind und einige zusätzliche Infos. Benutzen Sie &quot;What&apos;s this&quot;, um mehr zu erfahren.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation>Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Erstellt ein neues Makro&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Erstellt ein neues Makro, fragt nach einem Namen dafür und öffnet das Bearbeitungsfenster.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation>Makro</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation>Kürzel</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Beschreibung</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation>&lt;qt&gt;
&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; Name des Makros, wie er auch in der Titelleiste und an anderen Stellen in Scribus erscheint.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Bearbeiten:&lt;/b&gt; Wenn das Makro bearbeitet werden kann, erscheint in dieser Spalte das Wort &quot;Ja&quot;. Makros, die mit dem Befehl register_macro erstellt wurden, können Sie normalerweise nicht bearbeiten.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Kürzel:&lt;/b&gt; Zeigt das Tastenkürzel für dieses Makro an, falls vorhanden. Zum Beispiel bedeutet CTRL-F8, dass Sie genau diese Zeichenfolge drücken müssen, um das Makro in Scribus zu starten.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Beschreibung:&lt;/b&gt; Wenn das Makro einen &quot;docstring&quot; enthält, wird der hier angezeigt. Möglicherweise wird er auch gekürzt, wenn er zu lang für dieses Feld ist. Die volle Beschreibung sehen Sie, wenn Sie &quot;What&apos;s this&quot; auf den entsprechenden Eintrag anwenden.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation>Um&amp;benennen</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation>Alt+M</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation>Benennt das gewählte Makro um.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Benennt das gewählte Makro um. Sie werden nach einem neuen Namen gefragt.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Bearbeiten...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation>Alt+E</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Den Quelltext des gewählten Makros bearbeiten, falls der Quelltext verfügbar ist.&lt;/qt&gt;</translation>
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
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Löscht das gewählte Makro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Löscht das gewählte Makro. Ein Makro, das gelöscht wurde, kann nicht wiederhergestellt werden. Falls das Makro von einem Autostart-Script erstellt wurde, erscheint es nach dem Neustart von Scribus wieder.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation>&amp;Tastenkürzel festlegen</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Legt ein Tastenkürzel für das gewählte Makro fest.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Legt für das aktuelle Makro ein Tastenkürzel fest, mit dem es schnell aufgerufen werden kann. Sie werden nach einer Tastenkombination gefragt.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation>E&amp;xportieren</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation>Alt+X</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation>Exportiert das Makro in eine Datei.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation>&lt;qt&gt;Speichert das Makro in eine externe Datei. Diese Datei ist ein Pythonscript und enthält Befehle, um das Makro neu zu erstellen. Sie können ein so gespeichertes Makro wieder öffnen mit dem Befehl &lt;tt&gt;Erweiterungsscript laden&lt;/tt&gt; im &lt;tt&gt;Script&lt;/tt&gt;-Menü, oder mit dem Button Importieren im Makromanager.&lt;/p&gt;
&lt;p&gt;Wenn Sie eine menschenlesbare Version von Ihrem Makro erhalten wollen, wählen Sie lieber &lt;tt&gt;Bearbeiten&lt;/tt&gt; und nutzen Sie den Befehl &lt;tt&gt;Quelltext speichern&lt;/tt&gt; im Menü &lt;tt&gt;Bearbeiten&lt;/tt&gt;. Diese Version können Sie wieder aufrufen, indem Sie ein neues Makro erstellen und dann den Befehl &lt;tt&gt;Neu&lt;/tt&gt; und &lt;tt&gt;Quelltext laden&lt;/tt&gt; aufrufen.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation>&amp;Alle löschen</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation>Alt+A</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation>Alle Makros löschen.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Alle vorhandenen Makros löschen. Diese Aktion können Sie nicht rückgängig machen. Makros, die von Autostart-Scripts erzeugt werden, erscheinen nach dem nächsten Start wieder.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation>Makros aus einer Datei importieren.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Lädt Makros aus einer externen Datei.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation>Diesen Dialog schließen</translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation>Zu Scribus zurückkehren</translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Gewähltes Makro bearbeiten.&lt;/p&gt;
&lt;p&gt;Wenn dieser Button grau ist, ist entweder kein Makro ausgewählt oder der Quelltext ist für dieses Makro nicht verfügbar - dann steht in der Spalten Bearbeiten das Wort Nein.&lt;/p&gt;
&lt;p&gt;Ist der Quellcode nicht verfügbar, könnte das Makro von einem Script erzeugt worden sein.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation>Scribus Makros (*.pymacro)</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation>Datei öffnen</translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation>Wählen Sie das Makro aus, das Sie öffnen möchten.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation>Datei speichern</translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation>Alle Makros speichern</translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation>Scribus - Makro umbenennen</translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation>Geben Sie bitte einen neuen Name ein:</translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation>Scribus - Tastenkürzel festlegen</translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation>Geben Sie bitte ein neues Tastenkürzel ein:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Ja</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Nein</translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Mehrfach Duplizieren</translation>
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
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Fehler bei der Ausführung eines Scripts</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Falls das Script ein offizelles Scribus-Script ist, den Fehler bitte auf &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; melden.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">&amp;Konsole zeigen</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">&amp;Konsole verbergen</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Diese Meldung befindet sich auch in Ihrer Zwischenablage. Benutzen Sie Strg+V, um sie in den Bugtracker zu kopieren.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Change...</source>
        <translation>Ändern...</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importieren</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
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
        <source>Import Template</source>
        <translation>Vorlage importieren</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Seiten importieren</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Vom Dokument:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Seiten:</translation>
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
        <source> from 0</source>
        <translation>von 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Seiten erstellen</translation>
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
        <translation>am Ende des Dokuments</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>von %1</translation>
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
        <translation>Seiten verschieben von:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>bis:</translation>
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
        <translation>ans Dokumentende</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <source>Custom Spacing</source>
        <translation>Abstände</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Farbprofil:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Render Priorität:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ Farbmetrisch</translation>
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
        <translation>Gefast</translation>
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
        <source>Manual Kerning</source>
        <translation type="obsolete">Manuelles Kerning</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Tonwert:</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Name des selektierten Objekts</translation>
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
        <translation>Links-Oben als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Rechts-Oben als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Links-Unten als Ursprung setzen</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Rechts-Unten als Ursprung setzen</translation>
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
        <translation>Textumrissfarbe</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Textfarbe</translation>
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
        <translation>Ändert die Einstellungen für die linken und End-Punkte</translation>
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
        <translation>Benutzt die Bildproportionen an Stelle der Rahmenproportionen</translation>
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
        <translation>Wechsel zwischen Breite des Zwischenraums und der Spalten</translation>
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
        <translation>Zeigt an, auf welchem Level sich das Objekt befindet. 0 bedeutet im Vordergrund</translation>
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
        <source>&amp;Font Size:</source>
        <translation>Schrift&amp;größe:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Kerning:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>Zeilenabstan&amp;d:</translation>
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
        <translation>Für den Textfluss wird das umgebende Rechteck an Stelle der Form genutzt</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Für den Textfluss wird eine zweite Linie benutzt, die auf der originalen Form basiert</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Spracheinstellung für die Silbentrennung</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Rechts-nach-Links-Text</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation>Buchstabe&amp;nabstand:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manueller Buchstabenabstand</translation>
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
        <translation>Gefast</translation>
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
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation>Musterseiten bearbeiten</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Kopie von %1</translation>
    </message>
    <message>
        <source>New Template</source>
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
        <source>Name:</source>
        <translation>Name:</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anfügen</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
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
        <source>&amp;Close</source>
        <translation>Schl&amp;ießen</translation>
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
        <source>Do you really want to delete this Template?</source>
        <translation>Wollen Sie diese Vorlage wirklich löschen?</translation>
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
        <source>Options</source>
        <translation>Optionen</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation>Punkte (pts)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
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
        <source>Column Guides</source>
        <translation>Spalteneinteilung</translation>
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
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Seitengröße des Dokuments, entweder eine Standardgröße oder benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Ausrichtung der Dokument-Seite</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Breite der Seite, veränderbar, wenn &quot;Benutzerdefiniert&quot; ausgewählt ist</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Höhe der Seite , veränderbar, wenn &quot;Benutzerdefiniert&quot; ausgewählt ist</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Aktiviert doppelseitiges Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Macht die erste Seite zu der linken Seite des Dokuments</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der Abstand gewählt werden, um eine genaue Bindung zu ermöglichen</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der Abstand gewählt werden, um eine genaue Bindung zu ermöglichen</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Erste Seitennummer im Dokument</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Standard-Maßeinheit für das Dokument</translation>
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
        <translation>&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Lin&amp;ks:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Rechts:</translation>
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
        <source>F&amp;irst Page Number:</source>
        <translation>Nummer der &amp;ersten Seite:</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>A&amp;ußen:</translation>
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
        <translation>Linke Seite</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Rechte Seite</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Aktiviert bedeutet Koordinaten relativ zur Seite,
andernfalls relativ zum Objekt.</translation>
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
        <translation>Beabeitung &amp;beenden</translation>
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
        <translation>Bezierkurve oder Polygon öffnen</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Bezierkurve schließen</translation>
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
        <translation>Pfad horizontal nach links verschieben</translation>
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
        <translation>Pfad verkleinern um x %</translation>
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
        <translation>% vergrößern oder verkleinern</translation>
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
        <translation>Pfad horizontal nach rechts verschieben</translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="obsolete">Optionen für OpenOffice Textverarbeitung</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Absatzstile aktualisieren</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Wenn ein Absatzstil mit dem gleichen Namen bereits in Scribus existiert,
wird er entweder an den Stil aus dem OpenOffice.org Dokument angepasst
oder unverändert gelassen</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Name der Datei vor jeden Absatzstil anfügen</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Name des Dokuments als Suffix an die Namen der
Absatzstile anhängen</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Einstellungen speichern</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Sollen beim Import immer die aktuellen Einstellungen
verwendet werden, aktivieren Sie diese Option.
Sie werden dann nicht mehr um Bestätigung gebeten</translation>
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
        <source>Update paragraph Styles</source>
        <translation>Absatzstile aktualisieren</translation>
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
        <translation>Absatzstile nach Attributen zusammenfügen. Das verringert die Anzahl der Absatzstile, selbst wenn die im Originaldokument anders benannt sind.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Name des Dokuments an den Absatzstil in Scribus anhängen.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Diese Einstellungen zum Standard machen und nicht nochmal nachfragen beim Import von OASIS-Dateien.</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Script Konsole</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Export Range</source>
        <translation>Bereich</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Datei Optionen</translation>
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
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Allgemein</translation>
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
        <source>Fonts to embed:</source>
        <translation>Schriftart vollständig einbetten:</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Effekte</translation>
    </message>
    <message>
        <source> sec</source>
        <translation> sek</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Waagrecht</translation>
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
        <source>Passwords</source>
        <translation>Passwörter</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Berechtigungen</translation>
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
        <source>Solid Colors:</source>
        <translation>Farben:</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Profil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Render-Priorität:</translation>
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
        <source>PDF/X-3 Output Intent</source>
        <translation>PDF/X-3 Ausgabebedingung</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Trim Box</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Kein Effekt</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Jalusie</translation>
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
        <translation>Rollen</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Links nach Rechts</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>Oben nach Unten</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>Unten nach Oben</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Rechts nach Links</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Schräg von Links Oben</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Wahrnehmung</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Relativ Farbmetrisch</translation>
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
        <source>Image Settings</source>
        <translation>Bildeinstellungen</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automatisch</translation>
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
        <translation>Keine</translation>
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
        <source>Export all pages to PDF</source>
        <translation>Alle Seiten als PDF exportieren</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Eine Seitenbereich als PDF exportieren</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Zeit bis die nächste Seite angezeigt wird.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Länge des Effekts in Sekunden.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Gewählten Effekt auf alle Seiten anwenden.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Wählen Sie ein Masterpasswort, dass alle Sicherheitsfunktionen
in der exportierten Datei aktiviert</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Farbprofil für Dokumentfarben einbetten</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Farbprofil für Bilder einbetten</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Farbprofile der Quell-Bilder nicht benutzen</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Abzug für Beschnitt oben</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Abzug für Beschnitt unten</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Abzug für Beschnitt links</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Abzug für Beschnitt rechts</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Allgemein</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>Schrift&amp;arten</translation>
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
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Legt die PDF-Kompatiblität fest. Standard ist Acrobat 4.0, weil das die größte Kompatiblität ermöglicht.
Wählen Sie Acrobat 5.0, wenn Sie PDF 1.4-Funktionen wie Transparenz oder 128 bit-Verschlüsselung benötigen.
PDF/X-3 ist gedacht für gewerbliche Druckereien und nur möglich, wenn Sie das Farb-Management aktiviert haben.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Legt die Art des Bundes in der PDF-Datei fest. Lassen die Standard-
Einstellung (Links), wenn Sie das nicht verändern müssen.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Erzeugt Vorschau-Bilder für jede Seite der PDF-Datei.
Einige Programme nutzen diese Bilder zur Navigation.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Erzeugt PDF-Artikel, die für die Navigation in verketteten Textrahmen nützlich sind.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Lesezeichen im Dokument in die PDF-Datei einbetten.
Nützlich zur Navigation in langen PDF-Dateien.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Ausgabe-Auflösung für Text und Vektor-Grafiken.
Das beeinflusst nicht die Auflösung der Bitmap-Grafiken.</translation>
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
Braucht normalerweise nicht geändert werden.</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Bitmap-Bilder neu mit gewählten DPI rendern.
Ist die Option deaktiviert, wird die natürliche Auflösung der Bilder übernommen.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Punkte pro Zoll) für den Export von Bildern.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Schriftarten in PDF&apos;s einbetten. Das schützt das
Layout und Erscheinungsbild des Dokuments vor Veränderungen.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Aktiviert die Präsentationseffekte, wenn der Acrobat Reader im Vollbild-Modus ausgeführt wird.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Vorschau für aufgeführte Seiten anzeigen.</translation>
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
        <translation>Start-Position für die Effekte Einblenden und Teilen.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Richtung für die Effekte Jalusie und Teilen.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Aktiviert die Sicherheits-Funktionen in der exportierten PDF-Datei.
Wenn Acrobat 4.0 ausgewählt ist, wird die PDF-Datei mit 40 bit verschlüsselt.
Wenn Acrobat 5.0 ausgewählt ist, wird die PDF-Datei mit 128 bit verschlüsselt.
Hinweis: Die Verschlüsselung ist nicht so zuverlässig wie GPG oder PGP und hat einige Begrenzungen.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Farbmodell für die Ausgabe der PDF-Datei.
Wählen Sie Monitor/Internet für PDF-Dateien, die auf dem Monitor angezeigt oder mit herkömmlichen Druckern gedruckt werden sollen.
Wählen Sie Drucker, um die PDF-Datei auf einem CMYK-Drucker auszugeben.</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Farbprofil für Farben</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Render-Methode für Farben</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Farbprofil für Bilder</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Render-Methode für Bilder</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Ausgabe-Profil zum Drucken. Wenn möglich, benutzen Sie das Profil Ihres Druckers.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>Erforderliche Eingabe für PDF/X-3, ohne die das Dokument nicht PDF/X-3-konform ist. Benutzen Sie z.B. den Titel des Dokuments.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Kompressionsstärke: Minimal (25%), Niedrig (50%), Mittel (75%), Hoch (80%), Maximal (95%)</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Wählen Sie ein Password für Benutzer, um die PDF-Datei ansehen zu können.</translation>
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
        <translation>Kopieren von Text oder Bildern gestatten.
Wenn nicht ausgewählt, können weder Text von Bilder kopiert werden.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Hinzufügen von Anmerkungen gestatten.
Wenn nicht ausgewählt, wird das Hinzufügen von Anmerkungen verhindert.</translation>
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
        <source>Create PDF File</source>
        <translation type="obsolete">PDF-Datei erstellen</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Ausgabe in &amp;Datei:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ä&amp;ndern...</translation>
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
        <source>Compatibilit&amp;y:</source>
        <translation>&amp;Kompatibilität:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;Einband:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>&amp;Vorschaubilder erzeugen</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Verknüpfte Textrahmen als PDF-Arti&amp;kel speichern</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Lesezeichen integrieren</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Auf&amp;lösung:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Methode:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualität:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>Auflösung der Bilder &amp;reduzieren auf:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>Alle Schriftarten vollständig ein&amp;betten</translation>
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
        <translation>Seiten&amp;vorschau anzeigen</translation>
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
        <source>&amp;Apply Effect on all Pages</source>
        <translation>Effekt auf alle Seiten an&amp;wenden</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Verschlüsselung benutzen</translation>
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
        <source>Allow &amp;Printing the Document</source>
        <translation>Dokument &amp;drucken erlaubt</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Än&amp;derungen am Dokument erlaubt</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Kopieren von &amp;Text und Bildern gestatten</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Hinzufügen von &amp;Anmerkungen gestatten</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>&amp;Sicherheit</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Ausgabe &amp;vorgesehen für:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Benutzerdefinierte Rendering-Einstellungen benutzen</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Rendering-Einstellungen</translation>
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
        <translation>Runden</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Ellipse</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>ICC-Profile benutzen</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>&amp;Farbe</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Info-Text:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Ausgabe&amp;profil:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF-Dateien (*.pdf);;Alle Dateien (*.*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Diese erweiterte Optio ist normalerweise deaktiviert und sollte nur aktiviert werden,
wenn Ihre Druckerei es fordert und sie die notwendigen Informationen haben.
Andernfalls könnte ihre PDF-Datei fehlerhaft gedruckt werden und sie kann nicht plattformübergreifend verwendet werden.</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Text und Vektor&amp;grafiken komprimieren</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>Von allen Schriftarten nur &amp;benutzte Schriftzeichen einbetten</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Nur benutzte Schriftzeichen einbetten:</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>Präsentations&amp;effekte aktivieren</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Präsentation</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Seite(n) horizontal spiegeln</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Seite(n) vertikal spiegeln</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation>Als PDF speichern</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Farbmanagement muss aktiviert sein, um PDF/X-3 zu nutzen. Sie können das mit Einstellungen -&gt; Farbmanagement aktivieren.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF/X-3 wird unterstützt und ist aktiviert, kann aber mit der gewählten PDF-Version nicht genutzt werden. Um PDF/X-3 doch zu nutzen, müssen Sie es als Ihre PDF-Version einstellen (Kompatibilitätslevel).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF/X-3 wird durch diese Version von Scribus nicht unterstützt, weil sie ohne CMS kompiliert wurde.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation>Anmerkungen und Felder in der PDF-Datei zulassen.
Wenn nicht aktiviert, können Sie Anmerkungen und Felder nicht bearbeiten.</translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation>Sicherheitseinstellungen für PDF</translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation>Einstellungen für PDF/X-3</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;PDF/X-3 unterstützt keine Sicherheitsfunktionen. Sie müssen PDF/X-3 deaktiveren, wenn Sie Sicherheitsfunktionen nutzen wollen.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source> Lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility. Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Note: PDF/X-3 will convert all CMYK images to ICC based RGB colors. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
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
        <translation>Stellt den Text auf Kosten der Render-Geschwindigkeit glatter dar.
Funktioniert nur mit Type-1-Schriftarten</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Aktiviert Transparenz und transparente Objekt im Dokument. Erfordert Ghostscript 7.07 oder höher</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Benutzt zur Vorschau CMYK an Stelle der RGB-Farben</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Aktiviert den Cyan-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Aktiviert den Magenta-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Aktiviert den Yellow-Auszug</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Aktiviert den Black-Auszug</translation>
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
        <translation>Unterfarbenreduktion ersetzt einige Grautöne, die beim Mischen von Cyan, Magenta und
Yellow entstehen, durch Schwarz. Dadurch kann sich die Qualität von Bilder, die vor allem
neutrale Farbtöne oder Grautöne enthalten, verbessern. Allerdings sollte Sie da immer ein
bisschen experimentieren, wie Sie die besten Ergebnisse erhalten.
UFR reduziert auch die Gefahr einer Übersättigung durch die CMY-Farben.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Schriftarten (TrueType &amp;&amp; OpenType), EPS, PDF und Vektorgrafiken werden schöner dargestellt,
dafür erhöht sich benötigte Renderingzeit gerinfügig</translation>
    </message>
</context>
<context>
    <name>Page</name>
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
        <source>Picture</source>
        <translation>Bild</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Datei:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Textkette</translation>
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
        <translation>Ein</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Aus</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Text bearbeiten...</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>Das Programm</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>fehlt!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Kopie von</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Einf&amp;ügen</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;Ränder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Ra&amp;hmen zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>&amp;Bilder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Ra&amp;ster zeigen</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Magnetisches Ra&amp;ster</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Magnetische Hilfs&amp;linien</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>Original-PPI:</translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>Aktuelle PPI:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>&amp;Information</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>Bild &amp;laden...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>Bild &amp;sichtbar</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>Bilder ak&amp;tualisieren</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>Bild &amp;bearbeiten</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>Rahmen am Bild aus&amp;richten</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Text &amp;laden...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Text anfügen...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>Text &amp;bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>Beispielte&amp;xt einfügen</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>Ist ein PDF-&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>Ist eine PDF-&amp;Anmerkung</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>Eigenschaften der An&amp;merkung</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>Feld&amp;eigenschaften</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>&amp;PDF-Optionen</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>&amp;Sperren</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Ent&amp;sperren</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Objekt&amp;größe sperren</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Objektgröße &amp;freigeben</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>In &amp;Bibliothek speichern</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Auf andere &amp;Ebene verschieben</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Gruppieren</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Gruppe auf&amp;lösen</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>An&amp;ordnung</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
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
        <source>&amp;Picture Frame</source>
        <translation>&amp;Bildrahmen</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Poly&amp;gon</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Umrisse</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>&amp;Textrahmen</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>&amp;Bezierkurve</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>&amp;Umwandeln in</translation>
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
        <source>&amp;Delete</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>Inhalt lösc&amp;hen</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Eigenschaften an&amp;zeigen...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Eigenschaften &amp;verbergen...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Wollen Sie wirklich den ganzen Text löschen?</translation>
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
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Seite </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation> von %1</translation>
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
        <source>Pictures</source>
        <translation>Bilder</translation>
    </message>
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
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source>Polygon Properties</source>
        <translation>Polygon-Eigenschaften</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Ec&amp;ken:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fak&amp;tor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Anzahl der Ecken des Polygons</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grad der Drehung des Polygons</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Vorschau der gewählten Eigenschaften</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Faktor über&amp;nehmen</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Hier stellen Sie ein, wie stark konvex oder konkav das Polygon gezeichnet wird</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Ein negativer Wert steht für eine konkave (sternförmige) Figur,
ein positiver Wert für eine konkave Form</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Voreinstellungen</translation>
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
        <translation>Bibliothek</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Oberfläche</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Einheiten</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Punkte (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
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
        <source>Menus</source>
        <translation>Menüs</translation>
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
        <translation>Autom. Speichern</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Hilfslinien Einrichtung</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Hilfslinienfarben</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Platzierung</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Kapitälchen</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Andere</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>The quick brown fox jumps over the lazy dog</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Andere Optionen</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Vorschau</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Klein</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Mittel</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Zum Einstellen der Anzeigegröße das Lineal mit dem Regler anpassen.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>Externe Hilfsprogramme</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Versch.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Postscript Interpreter</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Bildbearbeitungsprogramm</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Drucken</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Auswahl der Standard-Fenster-Dekoration.
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
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Radius des Bereichs, in dem es möglich ist, die Objektbegrenzungen zu aktivieren</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Anzahl der zuletzt geöffneten Dokumenten im Datei-Menü</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Standard-Verzeichnis für Dokumente</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Standard-Verzeichnis für ICC-Profile</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Standard-Verzeichnis für Scripte</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Standard-Größe der Seite</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Standard-Ausrichtung der Seite in neuen Dokumente</translation>
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
        <translation>Aktiviert doppelseitiges Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Die erste Seite im Dokument soll eine linke Seite sein</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Scribus speichert im gegebenen Intervall selbstständig eine Backup-Kopie 
des Dokuments mit der Erweiterung .bak</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Abstand zwischen zwei automatischen Sicherungen</translation>
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
        <translation>Abstand, in dem ein Objekt an die Hilfslinien einrastet</translation>
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
        <source>Place the grid behind your page objects</source>
        <translation>Gitter im Hintergrund platzieren</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Gitter im Vordergrund platzieren</translation>
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
        <source>Text Frame Properties</source>
        <translation>Eigenschaften der Text-Rahmen</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Eigenschaften der Bilder-Rahmen</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Eigenschaften der Formen</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Eigenschaften der Zoom-Level</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Eigenschaften des Linien-Werkzeuges</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Eigenschaften des Polygon-Werkzeuges</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Schriftart für einen neuen Text-Rahmen</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Schriftgröße für einen neuen Textrahmen</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Schriftfarbe</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Anzahl der Spalten in einem neuen Text-Rahmen</translation>
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
        <translation>Bild-Rahmen ermöglicht das Skalieren auf eine beliebige Größe</translation>
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
        <translation>Bilder behalten automatisch ihre originalen Seitenverhältnisse</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Füllfarbe für Bild-Rahmen</translation>
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
        <translation>Linien-Stil der Form</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Linien-Breite der Form</translation>
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
        <translation>Abstand zwischen zwei Zoom-Stufen</translation>
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
        <translation>Linien-Stil</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Breite der Linien</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Anzahl der Ecken in Polygonen</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grad der Drehung des Polygons</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Vorschau der gewählten Eigenschaften</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Ändert die Größe der Vorschau im Bibliotheks-Fenster</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Wenn Doppelseiten aktiviert sind, werden zwei Seiten nebeneinander gezeigt</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Farbe des Seiten-Hintergrundes</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Farbe für die Seiten-Ränder</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Zeigt den nicht-druckbaren Bereich in der Farbe des Seiten-Randes</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Aktiviert die Transparenz-Funktion mit dem PDF 1.4-Export</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Legt das Standard-Zoom-Einstellung fest</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="obsolete">Pfad zum Ghostscript-Interpreter</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Text bei EPS- und PDF-Anzeige weichzeichnen</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Grafiken ebi EPS- und PDF-Anzeige weichzeichnen</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Objekte außerhalb der Seitenränder auf den gedruckten oder exportierten Seiten nicht zeigen</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Bibliothek nach jeder Veränderung erneut speichern</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Pfad zum Bildbearbeitungsprogramm</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Grundlinien-Raster</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Grundlinien-Raster nicht anzeigen</translation>
    </message>
    <message>
        <source> px</source>
        <translation>px</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Thema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Schrift&amp;größe:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>Einstellungen für die Maus</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Rad-Vorschub:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>Fang&amp;radius:</translation>
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
        <translation>&amp;Wechseln...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC-Profile:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Skripte:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Ä&amp;ndern...</translation>
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
        <translation>&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Linke Seite &amp;zuerst</translation>
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
        <translation>&amp;Aktiviert</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervall:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>&amp;Abstand der Teillinien:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Ab&amp;stand der Hauptlinien:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>&amp;Fangabstand der Hilfslinien:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>&amp;Farbe der Teillinien:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Fa&amp;rbe der Hauptlinien:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Farbe der &amp;Hilfslinien:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>&amp;Farbe des Grundlinienrasters:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>In den &amp;Vordergrund</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>E&amp;in</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>A&amp;us</translation>
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
        <source>D&amp;isplacement:</source>
        <translation>&amp;Versatz:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Ska&amp;lierung:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Grundlinen&amp;raster:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Grundlinien-&amp;Versatz:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>Automatischer &amp;Zeilenabstand:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>&amp;Schriftartvorgabe:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Größen&amp;vorgabe:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Text&amp;farbe:</translation>
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
        <source>&amp;Line Color:</source>
        <translation>Linien&amp;farbe:</translation>
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
        <source>&amp;Type of Line:</source>
        <translation>Linien&amp;art:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Linien&amp;breite:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mini&amp;mal:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Ma&amp;ximal:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>Schritt&amp;weite:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Freie Skalierung</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>&amp;Horizontale Skalierung:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>&amp;Vertikale Skalierung:</translation>
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
        <source>Corn&amp;ers:</source>
        <translation>Ec&amp;ken:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Drehung:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>Fa&amp;ktor:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Veränderungen beim &amp;Verlassen speichern</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Groß</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Seiten &amp;nebeneinander anzeigen</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Seitenfarben</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Hintergrund:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Ränder:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Nicht druck&amp;baren Rand in der Rahmenfarbe zeigen</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>PDF 1.4 &amp;Transparenzen aktivieren</translation>
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
        <translation>Auf Sei&amp;tenränder beschneiden</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>A&amp;ußen:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Unterfarben&amp;reduktion anwenden</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>&amp;Vorlagen:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Ä&amp;ndern...</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Zusätzliches Verzeichnis für Dokument-Vorlagen</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Faktor über&amp;nehmen</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Hier stellen Sie ein, wie stark konvex oder konkav das Polygon gezeichnet wird</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Ein negativer Wert steht für eine konkave (sternförmige) Figur,
ein positiver Wert für eine konkave Form</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Mit dieser Option werden ein paar Grautöne, die entstehen,
wenn Black aus Cyan, Magenta und Yellow gemischt wird, durch Schwarz ersetzt.
Hauptsächlich werden davon neutrale und dunkle Farbtöne beeinflusst,
die Grau sehr nahe stehen. Diese Option kann zu besseren Bildern führen,
allerdings müssen Sie von Fall zu Fall entscheiden, wie Sie bessere Ergebnisse
bekommen. Außerdem reduziert UFR die Gefahr einer Übersättigung mit CMY-Farben.</translation>
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
        <translation>US-Letter</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloid</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation>Standardverzeichnis für ICC-Profile. Diese Einstellung
kann nicht geändert werden, wenn Dokumente geöffnet sind.</translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation>Grundlinienraster anzeigen</translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation>Grundlinienraster ausblenden</translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation>Pfad zum Ghostscript-Interpreter</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation>Pfad zum Bildbearbeitungsprogramm. Wenn Sie gimp benutzen möchten
und es in Ihrer Distro enthalten ist, ist der Befehl gimp-remote empfehlenswert,
weil Sie Bilder in bereits laufenden Instanzen von gimp bearbeiten können.</translation>
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
        <translation>Eine Ebene nach oben</translation>
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
        <translation>Voransicht der Datei-Info</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Voransicht des Datei-Inhalts</translation>
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
        <translation>&amp;Öffnen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
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
        <translation>Nach &amp;Name sortieren</translation>
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
Überprüfen Sie Pfad und Dateinamen.</translation>
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
        <translation>&lt;h3&gt;Über Qt&lt;/h3&gt;&lt;p&gt;Dieses Programm verwendet Qt Version %1&lt;/p&gt;&lt;p&gt;Qt ist ein multi-platform Framework zum Erstellen von GUI-Programmen in C++.&lt;/p&gt;&lt;p&gt;Qt bietet Portierungsmöglichkeiten mit nur einer Quellcode-Basis auf MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux und allen anderen großen kommerziellen Unix-Versionen.&lt;br&gt;Qt ist auch für eingebettete Systeme erhältlich.&lt;/p&gt;&lt;p&gt;Qt ist ein Produkt von Trolltech. Weitere Informationen finden Sie unter &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt;.&lt;/p&gt;</translation>
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
        <translation>Dokument</translation>
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
        <source>Print Preview</source>
        <translation>Druckvorschau</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Onlinereferenz</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Python Scripte (*.py);; Alle Dateien (*)</translation>
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
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;cript</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Datei existiert bereits. Soll sie überschrieben werden?</translation>
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
        <source>Error writting the output file(s).</source>
        <translation>Fehler beim Schreiben der Datei(en).</translation>
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
        <translation>Magazine</translation>
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
        <translation>Neu von &amp;Vorlage...</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF-Präsentationen</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>&amp;Als Vorlage speichern...</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Sonderzeichen einfügen</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Als &amp;Bild speichern...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Schriftarten&amp;vorschau</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>D&amp;ruckvorschau</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Handbuch für den &amp;Scripter...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="obsolete">&amp;Scripts für Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">Script &amp;ausführen...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">&amp;Zuletzt verwendete Scripts</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Konsole &amp;zeigen</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Seite als &amp;SVG speichern...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>&amp;SVG importieren...</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>&amp;EPS/PS importieren...</translation>
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
        <source>Template: </source>
        <translation>Vorlage:</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
externe Links</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Die Schriftart %1 ist defekt und wird ab sofort ignoriert</translation>
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
        <translation>Holländisch</translation>
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
        <translation>Deutsch</translation>
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
        <source>Tried to set progress &gt; maximum progress</source>
        <translation>Fortschritt kann nicht mehr als der maximale Fortschritt betragen</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">Ü&amp;ber das Script...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">Über das Script</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>keine Farbbezeichnung angegeben.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation>Farbe nicht gefunden</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Umbenennen einer Farbe ohne Bezeichnung fehlgeschlagen.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="obsolete">Farbe nicht im Dokument gefunden</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="obsolete">Farbe nicht in der Liste der Standardfarben gefunden</translation>
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
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation>Öffnen des Dokuments fehlgeschlagen</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation>Speichern des Dokuments fehlgeschlagen</translation>
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
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="obsolete">Bild lässt sich nicht auf 0% skalieren</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Schriftart nicht gefunden</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann keine leeres Beispiel erstellen</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Datei nicht ohne Namen speichern</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="obsolete">Eine Ebene ohne Bezeichnung ist nicht erlaubt</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Ebene nicht gefunden</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="obsolete">Die letzte Ebene lässt sich nicht entfernen</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann keine Ebene ohne Bezeichnung erstellen</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation>Ein Objekt mit gleichem Namen existiert bereits</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation>Sie müssen mindestens zwei Punkte (vier Werte) angeben</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation>Sie müssen eine gerade Anzahl an Punkten eingeben</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation>Sie müssen mindestens drei Punkte (sechs Werte) angeben</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation>Sie müssen mindestens 4 Punkte (8 Werte) angeben</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation>Die Anzahl der Punkte muss ein Vielfaches von Sechs sein</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation>Objekt nicht gefunden</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation>Absatzstil nicht gefunden</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation>Kann Absatzstil nur auf Textrahmen anwenden</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation>Speichern der EPS-Datei fehlgeschlagen</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation>Seitenzahl außerhalb des Bereichs</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation>Falsche Argumente: Sie müssen eine Liste von Dezimalzahlen angeben</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation>Argumente enhalten nicht-numerische Werte: Sie dürfen nur Dezimalzahlen angeben</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation>Linienbreite muss zwischen 0 und 12 betragen</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Tonwert der Linie muss zwischen 0 und 100 liegen</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Tonwert der Füllung muss zwischen 0 und 100 liegen</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Eckradius muss positiv sein.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation>Linienstil nicht gefunden</translation>
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
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="obsolete">Index außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Ausrichtung nicht gültig. Verwenden Sie eine der scribus.ALIGN*-Konstanten.</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Text-Ausrichtung nur für Textrahmen festlegen</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="obsolete">Schriftgröße muss zwischen 1 und 512 betragen</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Schriftgröße nur in Textrahmen verändern</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Schriftart nur in Textrahmen verändern</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="obsolete">Zeilenabstand muss größer gleich 0.1 sein</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Zeilenabstand nur für Textrahmen festlegen</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="obsolete">Spaltenabstand muss positiv sein</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Spaltenabstand nur für Textrahmen festlegen</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Spaltenanzahl muss größer als 1 sein</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Spalten nur in Textrahmen zählen</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Auswahlindex außerhalb des gültigen Bereichs</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Text nur in Textrahmen auswählen</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Text nur aus Textrahmen löschen</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Füllfarbe des Textes nur für Textrahmen festlegen</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Farbe der Textumrandung nur für Textrahmen festlegen</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Tonwert des Textes nur für Textrahmen festlegen</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann nur Textrahmen miteinander verketten</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Zielrahmen muss leer sein</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Zielrahmen ist schon mit einem anderen Rahmen verbunden</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Zielrahmen ist schon Ziel eines anderen Textrahmens</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Quell- und Zielrahmen sind das gleiche Objekt</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann Verkettung nur von Textrahmen auflösen</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="obsolete">Objekt ist kein verketteter Textrahmen, also kann die Verkettung nicht gelöst werden.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann den letzten Rahmen einer Kette nicht abtrennen. Lösen Sie erst die Verkettung der anderen Rahmen.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="obsolete">Kann nur Textrahmen in Umrisse konvertieren</translation>
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
        <translation type="obsolete">Python-Interface für Scribus

Dieses Modul stellt das Python-Interface bereit - sie können
Scribus damit steuern und Objekte auf dem Arbeitsbereich
verändern. Jede Funktion ist im Folgenden extra dokumentiert.
Eine Dinge wiederholen sich immer wieder - die meisten 
Funktionen beziehen sich zum Beispiel auf Rahmen.
Rahmen werden durch ihren Namen eindeutig bestimmt.
Sie sind trotzdem keine nativen Python-Objekte. Viele Funktionen
benötigen einen zusätzlichen Parameter, den Namen des Rahmens.
Viele Fehler sind innerhalb der der Funktionen gleich, sie sind zur Zeit
nicht jedesmal mit in den Definitionen der Funktionen aufgeführt:
- viele Funktionen rufen den Fehler NoDocOpenError hervor, wenn Sie
sie benutzen, ohne vorher ein Dokument zu öffnen
- Wenn Sie keinen gültigen Rahmen angeben, wird die Funktion versuchen,
den aktuellen zu verwenden. Schlägt das fehl, erscheint der Fehler
NoValidObjectError
- Wenn Sie eine Operation auf einen Rahmen anwenden, die dafür nicht zulässig
ist, erhalten Sie den Fehler WrongFrameTypeError. Beispiel: Sie möchten die Textfarbe
für einen Bildrahmen festlegen - das macht wenig Sinn.
- Alle Fehler, die in der Python-API auftreten, werden ungefiltert von jeder auf-
geführten Funktion ausgegeben. Das bedeutet, dass die Liste der Fehler auf keinen
Fall Anspruch auf Vollständigkeit erhebt.
Weitergehende Informationen zu den Fehlern, die die einzelnen Funktionen hervorrufen
können, finden Sie in der Funktionsreferenz.</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>&amp;OpenOffice.org Zeichnung importieren...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice Zeichnung (*.sxd);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Dokumente von OpenOffice Textverarbeitung </translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Farbe nicht gefunden - Python-Fehler</translation>
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
        <translation>Scribus Python Interface

Dieses Modul ist das Python-Interface für Scribus. Es stellt Funktionen bereit, um Scribus
zu steuern und Objekte auf der Arbeitsfläche zu manipulieren. Jede Funktion ist im Detail 
beschrieben.

Einige Dinge sind überall gleich:

Die meisten Funktionen müssen auf Rahmen angewandt werden. Rahmen sind durch ihren
Namen (eine Zeichenfolge) eindeutig bestimmt, aber das sind keine realen Python-Objekte.
Bei vielen Funktionen können Sie optional einen Rahmennamen angeben.
Es gibt auch einige Ausnahmen, die durchweg gleich sind. Diese sind dann nicht immer einzeln
aufgelistet:
- Viele Funktionen geben den Fehler NoDocError zurück, wenn kein Dokument geöffnet ist.
- Wenn Sie keinen Rahmennamen angeben, wird der aktuell gewählte Rahmen verwendet. Andernfalls
tritt der Fehler NoValidObjectError auf.
- Viele Funktionen geben den Fehler WrongFrameTypeError zurück, wenn der Rahmentyp des angegebenen Objekts
nicht zum geforderten Rahmentyp passt. Es macht zum Beispiel keinen Sinn, die Textfarbe in einem Bildrahmen zu bestimmen.
- Fehler, die von der Python-API kommen, werden ungefiltert angezeigt. Deswegen ist die Auflistung der möglichen Fehlermeldungen
bei den einzelnen Funktionen immer unvollständig.

Details zu den Fehlern der einzelnen Funktionen finden Sie in der Funktionsbeschreibung.</translation>
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
        <translation>Short &amp;Words...</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Short Words untersucht den Text. Bitte haben Sie einen Moment Geduld...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Short Words ist fertig.</translation>
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
        <translation>Leere Objekte können nicht gerendet werden.</translation>
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
        <translation>Zeilenabstand ist muss größer gleich 0,1 sein.</translation>
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
        <translation>Text kann nur in einem Textrahmen selektiert werden</translation>
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
        <source>Object is not a linked text frame, cannot unlink.</source>
        <comment>python error</comment>
        <translation>Zielrahmen ist nicht verkettet, also kann die Verkettung auch nicht gelöst werden.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, cannot unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Der Rahmen ist das letzte Objekt einer Serie. Lösen Sie die Verkettung des vorhergehenden Rahmens.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Nur Textrahmen können in Umrisse konvertiert werden.</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian)</source>
        <translation>Portugiesisch (Brasilien)</translation>
    </message>
    <message>
        <source>Cannot get a colour with an empty name.</source>
        <comment>python error</comment>
        <translation>Farben ohne Namen sind ungültig.</translation>
    </message>
    <message>
        <source>Unable to save pixmap.</source>
        <comment>scripter error</comment>
        <translation>Pixmap kann nicht gespeichert werden.</translation>
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
        <translation>OpenDocument Textdateien</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisch</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
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
        <translation>Schl&amp;ießen</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Ränder</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Aktiviert das doppelseitige Layout</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Macht die erste Seite des Dokuments zu einer linken Seite</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem oberen Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Abstand zwischen dem unteren Seitenrand und der Papierkante</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem linken Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Abstand zwischen dem rechten Seitenrand und der Papierkante.
Wenn Doppelseiten aktiviert sind, kann hier der korrekte Bund-Abstand eingetragen werden</translation>
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
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>&amp;Doppelseiten</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Linke Seite &amp;zuerst</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Innen:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>A&amp;ußen:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Seitenformat</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Größe:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Benutzerdefiniert</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation>Ausrichtung:</translation>
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
        <source>Width:</source>
        <translation>Breite:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Höhe:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Nummer der &amp;ersten Seite:</translation>
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
        <translation>Keine</translation>
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
        <translation>Keine</translation>
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
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Kerning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Manuelles Kerning</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Zeicheneinstellungen bearbeiten</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation>Buchstabenabstand:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Manueller Buchstabenabstand</translation>
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
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Datei</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Suche nach Schriftarten</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">Auf Ihrem System sind keine Postscript-Fonts installiert</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Programm wird jetzt beendet</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Schwerwiegender Fehler</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Weiche Trennung</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Links ausrichten</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Rechts ausrichten</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Zentrieren</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Seitennummer einfügen</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Text auf Pfad setzen</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Zeige Ebenen</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Javascripte...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Rückgängig</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Zeige Seitenpalette</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Sperren/Entsperren</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Fester Zwischenraum</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Voreinstellungen werden geladen</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Silbentrennung wird initialisiert</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Tastaturkürzel werden gelesen</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Bibliothek wird geladen</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Plugins werden geladen</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Neu</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Öffnen...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Schließen</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Speichern</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Speichern unter...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Bild/Text laden...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Information...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Dokument einrichten...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Drucken...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Beenden</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Ausschneiden</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Kopieren</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Einfügen</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Alles auswählen</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Farben...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Stilvorlagen...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Musterseiten...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Schriften...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Neue Schriftart auswählen</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Duplizieren</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Mehrfach Duplizieren</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Löschen</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Gruppieren</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Gruppe auflösen</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Sperren</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>In den Hintergrund</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>In den Vordergrund</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Weiter nach unten</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Weiter nach oben</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Abstand/Ausrichtung...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Einfügen...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Löschen...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Verschieben...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Musterseite anwenden...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Hilfslinien bearbeiten...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Ganze Seite</translation>
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
        <source>Thumbnails</source>
        <translation>Miniaturen</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Ränder verbergen</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Rahmen verbergen</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Bilder verbergen</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Raster zeigen</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Am Raster ausrichten</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Eigenschaften</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Übersicht</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Bibliothek</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Bilder verwalten</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Silbentrennung</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Über Scribus</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Online-Hilfe...</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Stil</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Unterstrichen</translation>
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
        <source>Superscript</source>
        <translation>Hochgestellt</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Tiefgestellt</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Umrandung</translation>
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
        <source>Ready</source>
        <translation>Fertig</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Keine</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Bild laden...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Farbe</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertieren</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Text laden...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Schrift</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Größe</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Tonwert</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation>Entsperren</translation>
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
        <source>Loading...</source>
        <translation>Lade...</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Textdateien (*.txt);;Alle Dateien(*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Kann die Datei
%1
nicht speichern</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Speichern unter</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Dokumente (*.sla *.sla.gz *.scd *.scd.gz);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Speichere...</translation>
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
        <source>Scribus Manual</source>
        <translation>Scribus Handbuch</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Die folgenden Programme fehlen:</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Alle</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS-Bilder (*.eps);;Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Lade:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Passe Farben an</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Englisch</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Deutsch</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Spanisch</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italienisch</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Französisch</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russisch</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dänisch</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Slowakisch</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Ungarisch</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tschechisch</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holländisch</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Portugiesisch</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukrainisch</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polnisch</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griechisch</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Katalanisch</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finnisch</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Wählen Sie ein Verzeichnis</translation>
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
        <source>All Supported Formats</source>
        <translation>Alle unterstützten Formate</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irisch</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Neues Dokument anlegen</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Vorhandenes Dokument öffnen</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Aktives Dokument speichern</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Schließt das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Druckt das aktuelle Dokument</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Speichert das aktuelle Dokument als PDF</translation>
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
        <source>&amp;Item</source>
        <translation>&amp;Objekt</translation>
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
        <source>&amp;Tools</source>
        <translation>&amp;Werkzeuge</translation>
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
        <source>Show Baseline Grid</source>
        <translation>Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Grundlinien-Raster verbergen</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Einige Objekte sind gesperrt.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Alle sperren</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Alle entsperren</translation>
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
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
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
        <translation>Litauisch</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Schwedisch</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Slovenisch</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Farbmanagement...</translation>
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
        <source>Open &amp;Recent</source>
        <translation>&amp;Zuletzt verwendete öffnen</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>Sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>S&amp;peichern</translation>
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
        <translation>&amp;Für Ausgabe sammeln...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>Text/Bild &amp;laden...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>&amp;Text anfügen...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importieren</translation>
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
        <source>&amp;Export</source>
        <translation>E&amp;xportieren</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Dokument &amp;einrichten...</translation>
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
        <translation>&amp;Rückgängig machen</translation>
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
        <translation>&amp;Einfügen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Alles aus&amp;wählen</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Suchen &amp;&amp; Ersetzen...</translation>
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
        <translation>&amp;Linien-Stile...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Musterseiten...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Javascripts...</translation>
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
        <source>&amp;Lock</source>
        <translation>&amp;Sperren</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>In den &amp;Hintergrund</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
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
        <source>Distribute/&amp;Align...</source>
        <translation>Verteilen &amp;&amp; &amp;Ausrichten...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Form</translation>
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
        <source>C&amp;onvert to Outlines</source>
        <translation>In &amp;Pfade umwandeln</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>Ein&amp;fügen...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Löschen...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>Ver&amp;schieben...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Musterseite anwenden...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>&amp;Ganze Seite</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>&amp;Miniaturen</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Ra&amp;ster zeigen</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>&amp;Magnetische Hilfslinien</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>&amp;Grundlinien-Raster anzeigen</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Eigenschaften</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>Über&amp;sicht</translation>
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
        <source>P&amp;age Palette</source>
        <translation>Seiten&amp;palette</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Lesezeichen</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>Bilder &amp;verwalten</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Silbentrennung anwenden</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Tool&amp;tipps</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>&amp;PDF-Werkzeuge</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Tooltipps</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>Schrift&amp;arten...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Silbentrennung...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>Tasten&amp;kürzel...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>Ü&amp;ber Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Über &amp;Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>Scri&amp;bus Handbuch...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Stil</translation>
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
        <source>&amp;Other...</source>
        <translation>&amp;Andere...</translation>
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
        <source>&amp;Color</source>
        <translation>&amp;Farben</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Invertieren</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>Text &amp;laden...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>Schrift&amp;art</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Größe</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Effekte</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Ausrichtung</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>Ton&amp;wert</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabulatoren...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Ent&amp;sperren</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>&amp;Bilder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>&amp;Ränder zeigen</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Ra&amp;hmen zeigen</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>&amp;Hilfslinien zeigen</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript: Sie können keine EPS-Dateien einfügen</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Seiten &amp;importieren...</translation>
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
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Sie versuchen, mehr Seiten zu importieren als im aktiven Dokument vorhanden sind.&lt;/p&gt;Wählen Sie bitte aus:&lt;br&gt;&lt;ul&gt;&lt;li&gt;fehlende Seiten &lt;b&gt;erstellen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;Bis zur letzten vorhandenen Seite &lt;b&gt;einfügen&lt;/b&gt;&lt;/li&gt;&lt;li&gt;&lt;b&gt;Abbrechen&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Erstellen</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Importieren</translation>
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
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Ma&amp;gnetisches Raster</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>&amp;Voreinstellungen...</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>ICC-Profile werden geladen</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>&amp;Hilfslinien bearbeiten...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Größe:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>Ton&amp;wert:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>Dokument&amp;informationen...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>&amp;Rückgängig: Objekt löschen</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>&amp;Rückgängig: Objekt verschieben</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>&amp;Rückgängig: Objekt ändern</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Form bearbeiten</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>Die Datei %1 wurde nicht mit Scribus erstellt</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Fontsystem initialisiert...</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Portugiesisch (BR)</translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation>Auf dem System sind keine passenden Schriftarten verfügbar</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Farbmanagement wird unterstützt, ist aber zur Zeit nicht aktiviert. Sie müssen ICC-Profile installiert und den korrekten Profilpfad angegeben haben, um Farbmanagement aktivieren zu können.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Farbmanagement wird von dieser Version von Scribus nicht unterstützt (nicht kompiliert).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Kroatisch</translation>
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
        <source>All</source>
        <translation>Alle</translation>
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
        <translation>Kopie von</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
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
        <translation>&amp;Ignorieren</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Trotzdem sch&amp;ließen</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Jetzt speichern</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
</context>
<context>
    <name>ScripterCore</name>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>&amp;Scripts für Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>Script &amp;ausführen...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation>Python-Script von Datei laden.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation>Erweiterungsscript &amp;laden...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation>Lädt ein Pythonscript als Erweiterung. Wird genutzt, um Makros oder Python-Erweiterungen für Scribus zu laden.</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>&amp;Zuletzt verwendete Scripts</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>&amp;Konsole anzeigen</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation>Interaktive Python-Konsole anzeigen, in der Sie Python-Programme schreiben und ausführen können, die die Scripter Tools benutzen.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>Ü&amp;ber das Script...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;cripter</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation>&amp;Einstellungen für Script-Erweiterungen</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Öffnen</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Scripte (*.py);; Alle Dateien (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation>Fehler bei der Ausführung eines Scripts</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Falls das Script ein offizelles Scribus-Script ist, den Fehler bitte auf &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; melden.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Diese Meldung befindet sich auch in Ihrer Zwischenablage. Benutzen Sie Strg+V, um sie in den Bugtracker zu kopieren.</translation>
    </message>
    <message>
        <source>Scribus - Script Plugin</source>
        <translation>Script-Plugin für Scribus</translation>
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
        <translation>Die Funktion &apos;Script laden&apos; im Script-Plugin ist zur Zeit deaktivert.
Wenn Sie nur ein normales Script ausführen möchten, können Sie
den Befehl &apos;Script ausführen&apos; benutzen.

Wollen Sie eine Python-Erweiterung oder ein Makro laden, wählen Sie
Script -&gt; Einstellungen für Script-Erweiterungen und aktivieren Sie dort
die entsprechende Einstellung.

Bitte lesen Sie die Dokumentation, bevor Sie diese Einstellung aktivieren.</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>&amp;Konsole verbergen</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Über das Script</translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation>Scribus - Einstellungen für Script-Erweiterungen</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation>Script-Erweiterungen aktivieren</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation>Erweiterungen und Makros aktivieren</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Die Script-Erweiterungen aktivieren zusätzliche Funktionen inklusive Python-Makros und Startup-Script. Außerdem können Sie so auf die Funktion &lt;tt&gt;Erweiterungsscript laden&lt;/tt&gt; im Script-Menü zugreifen.&lt;/p&gt;
&lt;p&gt;
Nur Scripts, die für die Verwendung als Erweiterung vorgesehen sind, sollten über den Befehl &lt;tt&gt;Erweiterungsscript laden&lt;/tt&gt; oder als Startup-Scripts geöffnet werden. In der Scripter-Dokumentation finden Sie dazu weitere Informationen.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation>Startup-Script verwenden</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&apos;Startup-Script verwenden&apos; ist aktiviert. Das bedeutet, dass Scribus die gewählte Erweiterung bei jedem Start von Scribus ausführt. Stellen Sie sicher, dass das Script wirklich als Erweiterung geschrieben ist, andernfalls verursacht es möglicherweise Probleme.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;&apos;Startup-Script verwenden&apos; wird deaktiviert, wenn Sie die Script-Erweiterungen ausschalten, weil Erweiterungsscripte nicht ohne die zusätzlichen Funktionen für den Scripter aktiviert werden können.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation>Auswählen...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation>Ein neues Script auswählen</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ein neues Script auswählen&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation>Datei, die beim Start von Scribus geladen wird</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Die Datei mit dem Python-Script, die beim Start geladen werden soll.&lt;/p&gt;
&lt;p&gt;Beachten Sie dabei, dass zur Zeit der Ausführung Scribus noch nicht komplett geladen ist und zum Beispiel der Arbeitsbereich noch nicht zur Verfügung steht.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation>Schließen und Änderungen verwerfen</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation>Änderungen speichern und schließen</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation>Weitere Optionen</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation>Alle Namen beim Start importieren</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation>Führt &apos;from scribus import *&apos; beim Start in der Script-Konsole aus</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;Alle Namen beim Start importieren&lt;/tt&gt; ist eine Experteneinstellung. Sie sollten Sie aktiviert lassen, wenn Sie nicht genau wissen was Sie tun oder die Dokumentation nicht gelesen haben.&lt;/p&gt;
&lt;p&gt;Wenn Sie diese Option ausschalten, wird der Scripter nicht wie gewöhnlich &lt;tt&gt;from scribus import *&lt;tt&gt; ausführen, wenn er den Interpreter (für Scripts und Erweiterungen) beim Start initialisiert.&lt;/p&gt;
&lt;p&gt;Diese Änderung tritt erst in Kraft, wenn Sie Scribus neu starten.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation>Alte Aliase für Namen aktivieren</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Verwendet die alten Funktionsnamen&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Alte Aliase für Namen verwenden&lt;/tt&gt; ist eine Expertenoption. Sie sollten die Voreinstellung nicht verändern.&lt;/p&gt;
&lt;p&gt;Wenn aktiviert, erstellt der Scripter eine große Zahl an Funktionen und Konstanten, die kompatibel mit der Script-Version 1.2.0 sind. Die Option ist
standardmäßig aktiviert.&lt;/p&gt;
&lt;p&gt;Diese Änderung tritt erst in Kraft, wenn Sie Scribus neu starten.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation>Fake Stdin benutzen</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ersetzt sys.stdin mit einer Fake-Datei, damit Scribus nicht hängenbleibt, wenn ein Script von stdin lesen will.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Fake stdin benutzen&lt;/tt&gt; ist eine Expertenoption. Sie sollten die Voreinstellung nicht verändern.&lt;/p&gt;
&lt;p&gt;Normalerweise stellt Scribus eine Fake-Datei für Python bereit, damit Lesezugriffe auf stdin immer einen leeren String zurückgeben. Wenn das wirkliche &lt;tt&gt;sys.stdin&lt;/tt&gt; an Stelle dieser Datei wäre, würden Scripts, die davon versuchen zu lesen, die gesamte Scribus-Anwendung blockieren, bis auf stdin Daten ankommen. Für GUIs ist es ungewöhnlich, von stdin auf Input zu warten, deswegen würden die meisten User denken, Scribus wäre abgestürzt.&lt;/p&gt;
&lt;p&gt;Wenn Sie Input auf stdin akzeptieren wollen, können Sie diese Option deaktiveren. Normalerweise sollten Sie &lt;tt&gt;os.popen&lt;/tt&gt; oder eine andere Funktion nutzen, um eine Verbindung herzustellen.&lt;/p&gt;&lt;/qt&gt;</translation>
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
        <source>Show Template Names</source>
        <translation>Bezeichnungen der Vorlagen anzeigen</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Suchen &amp;&amp;Ersetzen</translation>
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
        <translation>Keine</translation>
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
        <source>OK</source>
        <translation>OK</translation>
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
        <translation>Schl&amp;ießen</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Löschen</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Seiten anordnen</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Verfügbare Vorlagen:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Seiten des Dokuments:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Doppelseiten</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Linke Seite zuerst</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Ziehen Sie die Seiten oder Vorlagen auf den Mülleimer, um sie zu löschen.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Hier sind Ihre Vorlagen, um eine neue Seite zu erzeugen
ziehen Sie eine Vorlage auf die Seitenansicht unten.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Vorschau aller Seiten des Dokuments.</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Kein Stil</translation>
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
        <translation>Warnung</translation>
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
        <translation>An&amp;fügen</translation>
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
        <translation>S&amp;peichern</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Wollen Sie diesen Absatzstil wirklich löschen?</translation>
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
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Wollen Sie wirklich alle Änderungen verlieren?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Wollen Sie wirklich den ganzen Text löschen?</translation>
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
        <translation>Textrahmen auffrischen</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Wollen Sie die Änderungen speichern?</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Änderungen übernehmen</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Änderungen ignorieren</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Neu</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Text neu vom Text-Rahmen</translation>
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
        <source>&amp;Insert Special...</source>
        <translation>Sonderzeichen &amp;einfügen...</translation>
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
        <translation>Text von Datei laden</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Text in Datei speichern</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Text aus Textrahmen neu laden</translation>
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
        <source>Clear all Text</source>
        <translation>Kompletten Text löschen</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Suchen und Ersetzen</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Intelligente Textmarkierung</translation>
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
        <source>Outline Text</source>
        <translation>Umrandet</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Durchgestrichen</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Absatzstile aktualisieren</translation>
    </message>
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
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Wenn ein Absatzstil mit dem gleichen Namen bereits in Scribus existiert,
wird er entweder an den Stil aus dem OpenOffice.org Dokument angepasst
oder unverändert gelassen</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Name des Dokuments als Suffix an die Namen der
Absatzstile anhängen</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Optionen für OpenOffice Textverarbeitung</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Sollen beim Import immer die aktuellen Einstellungen
verwendet werden, aktivieren Sie diese Option.
Sie werden dann nicht mehr um Bestätigung gebeten</translation>
    </message>
    <message>
        <source>Update Paragraph Styles</source>
        <translation>Absatzstile aktualisieren</translation>
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
        <translation>Diese Einstellungen zum Standard machen und nicht nochmal nachfragen beim Import von Dateien, die mit OpenOffice 1.x erstellt wurden.</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>in</translation>
    </message>
    <message>
        <source> p</source>
        <translation>p</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Position:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>&amp;Erste Zeile:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation>Ein&amp;zug Links:</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>Outline</source>
        <translation>Übersicht</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Objekt</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Typ</translation>
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
        <source>Font:</source>
        <translation>Schrift:</translation>
    </message>
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
        <source>Page</source>
        <translation>Seite</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warnung</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Der Name %1 ist nicht eindeutig.
Bitte wählen Sie einen anderen.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Gruppe</translation>
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
        <translation>&amp;OK</translation>
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
        <translation>Short Words</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Geschütztes Leerzeichen anwenden auf:</translation>
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
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation>&amp;Abbrechen</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation>&amp;Informationen und
Sprache</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation>Standardeinstellungen durch Benutzereinstellungen überschreiben</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation>Wenn die Datei mit den benutzerdefinierten Einstellungen existiert
(%1),
können Sie wählen, ob Sie Ihre Einstellungen an die globale Konfiguration
anhängen wollen.

Sie können mit dieser Einstellung auch vordefinierte Werte mit ihren 
eigenen Einstellungen ersetzen.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Die markierten Rahmen wurden verarbeitet.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Aktuelle Seite wurde verarbeitet.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Alle Objekte wurden verarbeitet.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation>Short Words für Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation>In folgenden Sprachen verfügbar</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation>Über Short Words</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Werkzeuge</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Objekte auswählen</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Textrahmen einfügen</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Bild einfügen</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Eigenschaften...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Polygone einfügen</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Linien einfügen</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Bezierlinien einfügen</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Freihand Linie einfügen</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Objekt drehen</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Rahmeninhalt bearbeiten</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Textrahmen verbinden</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Textrahmenverbindung lösen</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Vergrößern oder verkleinern</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Text mit dem Story Editor bearbeiten</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Zeichnet verschiedene Formen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Tabelle einfügen</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Entfernungen abmessen</translation>
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
        <translation>PDF Anmerkungen einfügen</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Zeichen auswählen:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Zeichen an der Cursorposition einfügen</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Gewählte Markierung(en) löschen.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Diesen Dialog schließen und zur Textbearbeitung zurückkehren.</translation>
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
        <translation>Schl&amp;ießen</translation>
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
        <source>Get text only</source>
        <translation type="obsolete">Nur Text importieren</translation>
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
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Speichert die Verknüpfung zwischen Dateierweiterung und
Importfilter. Sie werden nicht mehr nach einem Importer
für diesen Dateityp gefragt.</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Bitte entpacken Sie die Vorlage nach ~/.scribus/templates für den aktuellen Benutzer oder nach PREFIX/share/scribus/templates für alle Benutzer.</translation>
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
        <translation>Sie müssen sicherstellen, dass die Bilder und Fonts frei benutzt werden können. Wenn Sie Fonts nicht verbreiten dürfen, dann binden Sie diese beim Export als Vorlage nicht ein.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Wenn Sie Vorlagen erstellen, sollten Sie darauf achten, dass Sie die Informationen in dem Abschnitt &quot;Vorlagen installieren&quot; beachten. Das bedeutet, der Benutzer sollte in der Lage sein, die Vorlage einfach in das entsprechende Verzeichnis zu kopieren und zu benutzen.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Wenn Sie eine Vorlage von dieser Liste entfernen, wird nur der entsprechende Eintrag in der template.xml gelöscht, nicht die Vorlage selber. Sie können Vorlagen nur löschen, wenn Sie Schreibrechte auf template.xml besitzen.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Kopieren Sie die existierende template.xml nach template.lang_COUNTRY.xml (selber Ländercode wie bei den qm-Dateien), zum Beispiel template.fi.xml für Finnisch. Diese Kopie muss sich im selben Verzeichnis wie template.xml befinden.</translation>
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
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Abbrechen</translation>
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
        <translation>Worte</translation>
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
